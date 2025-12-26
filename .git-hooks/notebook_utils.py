"""Shared utilities for notebook pre-commit hooks.

Provides notebook parsing, argument handling, and result reporting.
Not intended to be run directly.
"""

import argparse
import json
import subprocess
from pathlib import Path
from typing import List, Set, Dict, Any, Optional


class NotebookParseError(Exception):
    """Raised when a notebook cannot be parsed."""

    pass


def parse_notebook(notebook_path: Path) -> Dict[str, Any]:
    """Parse a Jupyter notebook and return the JSON data."""
    try:
        with open(notebook_path, "r", encoding="utf-8") as f:
            return json.load(f)
    except json.JSONDecodeError as e:
        raise NotebookParseError(f"Invalid JSON in {notebook_path}: {e}")
    except Exception as e:
        raise NotebookParseError(f"Error reading {notebook_path}: {e}")


def get_markdown_cells(notebook_path: Path) -> List[str]:
    """Extract content from all markdown cells in a notebook."""
    notebook_data = parse_notebook(notebook_path)
    markdown_cells = []

    for cell in notebook_data.get("cells", []):
        if cell.get("cell_type") == "markdown":
            content = "".join(cell.get("source", []))
            markdown_cells.append(content)

    return markdown_cells


def get_code_cells(notebook_path: Path) -> List[str]:
    """Extract content from all code cells in a notebook."""
    notebook_data = parse_notebook(notebook_path)
    code_cells = []

    for cell in notebook_data.get("cells", []):
        if cell.get("cell_type") == "code":
            content = "".join(cell.get("source", []))
            code_cells.append(content)

    return code_cells


def get_staged_files(extension: Optional[str] = None) -> List[Path]:
    """Get list of staged files from git."""
    result = subprocess.run(
        ["git", "diff", "--cached", "--name-only", "--diff-filter=ACM"],
        capture_output=True,
        text=True,
        check=False,
    )

    if result.returncode != 0:
        return []

    files = []
    for line in result.stdout.strip().split("\n"):
        if not line:
            continue

        path = Path(line)

        if extension and not line.endswith(extension):
            continue

        if path.exists():
            files.append(path)

    return files


def find_all_notebooks(exclude_patterns: Optional[List[str]] = None) -> List[Path]:
    """Find all notebook files in the repository.

    Args:
        exclude_patterns: List of path patterns to exclude (e.g., ["archived", "additional_drafts"])

    Returns:
        List of Path objects for all notebooks found
    """
    if exclude_patterns is None:
        exclude_patterns = [".ipynb_checkpoints"]

    notebooks = []

    for notebook in Path(".").rglob("*.ipynb"):
        # Check if any exclude pattern is in the path
        if any(pattern in str(notebook) for pattern in exclude_patterns):
            continue

        # Skip hidden directories
        if any(part.startswith(".") for part in notebook.parts):
            continue

        notebooks.append(notebook)

    return sorted(notebooks)


class HookResult:
    """Container for pre-commit hook results."""

    def __init__(self):
        self.errors: List[Dict[str, Any]] = []
        self.warnings: List[Dict[str, Any]] = []
        self.checked_count: int = 0

    def add_error(self, file: Path, message: str, details: Optional[Dict] = None):
        """Add an error for a file."""
        self.errors.append({"file": file, "message": message, "details": details or {}})

    def add_warning(self, file: Path, message: str, details: Optional[Dict] = None):
        """Add a warning for a file."""
        self.warnings.append({"file": file, "message": message, "details": details or {}})

    def increment_checked(self):
        """Increment the count of files checked."""
        self.checked_count += 1

    def has_errors(self) -> bool:
        """Return True if any errors were found."""
        return len(self.errors) > 0

    def print_summary(self, verbose: bool = False):
        """Print a summary of the results."""
        print("=" * 70)

        if self.errors:
            print(f"\n❌ Found {len(self.errors)} error(s) in {self.checked_count} file(s)")
            for error in self.errors:
                print(f"\n  {error['file']}")
                print(f"    {error['message']}")
                if verbose and error.get("details"):
                    for key, value in error["details"].items():
                        print(f"      {key}: {value}")

        if self.warnings:
            print(f"\n⚠️  Found {len(self.warnings)} warning(s)")
            for warning in self.warnings:
                print(f"  {warning['file']}: {warning['message']}")

        if not self.errors and not self.warnings:
            print(f"✅ All {self.checked_count} file(s) passed")

    def exit_code(self) -> int:
        """Return appropriate exit code (1 for errors, 0 for success)."""
        return 1 if self.has_errors() else 0


def should_skip_notebook(notebook_path: Path, skip_patterns: List[str]) -> bool:
    """Check if a notebook should be skipped based on patterns."""
    path_str = str(notebook_path)
    return any(pattern in path_str for pattern in skip_patterns)


def create_base_parser(description: str) -> argparse.ArgumentParser:
    """Create an ArgumentParser with common notebook checking arguments.

    Args:
        description: Description of what this specific checker does

    Returns:
        ArgumentParser configured with common arguments
    """
    parser = argparse.ArgumentParser(
        description=description, formatter_class=argparse.RawDescriptionHelpFormatter
    )

    # Common selection arguments
    selection_group = parser.add_argument_group("Notebook selection")
    selection_group.add_argument(
        "--all-notebooks",
        action="store_true",
        help="Check all notebooks in repository (not just staged)",
    )
    selection_group.add_argument(
        "--exclude-archived",
        action="store_true",
        help="Exclude notebooks in 'archived' directories",
    )
    selection_group.add_argument(
        "--exclude-additional-drafts",
        action="store_true",
        help="Exclude notebooks in 'additional_drafts' directories",
    )

    # Common output arguments
    output_group = parser.add_argument_group("Output options")
    output_group.add_argument(
        "--verbose",
        "-v",
        action="store_true",
        help="Show detailed information including passing files",
    )
    output_group.add_argument(
        "--quiet", "-q", action="store_true", help="Suppress all output except errors"
    )

    return parser


def get_notebooks_from_args(args: argparse.Namespace) -> tuple[List[Path], str]:
    """Get list of notebooks to check based on parsed arguments.

    Args:
        args: Parsed arguments from ArgumentParser

    Returns:
        Tuple of (list of notebook paths, mode description string)
    """
    # Check if 'files' argument exists and is populated
    if hasattr(args, "files") and args.files:
        notebooks = [Path(f) for f in args.files if f.endswith(".ipynb")]
        mode = "specified"
    elif args.all_notebooks:
        # Build exclusion list based on flags
        exclude_patterns = [".ipynb_checkpoints"]

        if args.exclude_archived:
            exclude_patterns.append("archived")

        if args.exclude_additional_drafts:
            exclude_patterns.append("additional_drafts")

        notebooks = find_all_notebooks(exclude_patterns=exclude_patterns)
        mode = "all"

        # Add info about what was excluded to mode string
        exclusions = []
        if args.exclude_archived:
            exclusions.append("archived")
        if args.exclude_additional_drafts:
            exclusions.append("drafts")
        if exclusions:
            mode = f"all, excluding {', '.join(exclusions)}"
    else:
        notebooks = get_staged_files(extension=".ipynb")
        mode = "staged"

    return notebooks, mode


def print_check_header(count: int, mode: str, check_name: str, quiet: bool = False):
    """Print a header for the check being performed.

    Args:
        count: Number of notebooks being checked
        mode: Mode string ("staged", "all", "specified")
        check_name: Name of the check being performed
        quiet: Whether to suppress output
    """
    if not quiet:
        print(f"Checking {check_name} in {count} notebook(s) ({mode})...")
        print()


def extract_package_name(dependency_spec: str) -> str:
    """Extract package name from dependency specification.

    Examples:
        "pandas>=2.0.0" -> "pandas"
        "scikit-learn" -> "scikit-learn"
        "numpy[extra]>=1.24" -> "numpy"
    """
    package_name = dependency_spec.split("[")[0].split(">=")[0].split("==")[0]
    package_name = package_name.split("<")[0].split(">")[0].split("!")[0]
    return package_name.strip()


def get_import_aliases(package_name: str) -> Set[str]:
    """Map package names to their common import names.

    Args:
        package_name: Package name as it appears in pyproject.toml (should be lowercase)

    Returns:
        Set of possible import names for this package (includes the package name itself)
    """
    aliases = {
        "scikit-learn": {"sklearn"},
        "pillow": {"pil"},
        "python-dateutil": {"dateutil"},
        "beautifulsoup4": {"bs4"},
        "pyyaml": {"yaml"},
        "opencv-python": {"cv2"},
        "harmony-py": {"harmony"},
    }
    # Always include the package name itself as a possible import
    result = aliases.get(package_name, set())
    result.add(package_name)
    return result
