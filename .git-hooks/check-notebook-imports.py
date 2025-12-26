# !/usr/bin/env python3
"""Check notebook imports against pyproject.toml dependencies.

Ensures all imported packages are declared in project dependencies.
See .git-hooks/README.md for details and usage examples.
"""

import ast
import sys

try:
    import tomllib  # Python 3.11+
except ImportError:
    import tomli as tomllib  # Python <3.11
from pathlib import Path
from typing import Set

from notebook_utils import (
    get_code_cells,
    HookResult,
    NotebookParseError,
    create_base_parser,
    get_notebooks_from_args,
    print_check_header,
    extract_package_name,
    get_import_aliases,
)


def load_allowed_packages() -> Set[str]:
    """Load package names from pyproject.toml."""
    pyproject_path = Path("pyproject.toml")

    if not pyproject_path.exists():
        print("Warning: pyproject.toml not found")
        return set()

    with open(pyproject_path, "rb") as f:
        pyproject = tomllib.load(f)

    packages = set()

    # Main dependencies
    dependencies = pyproject.get("project", {}).get("dependencies", [])
    for dep in dependencies:
        package_name = extract_package_name(dep)
        packages.add(package_name.lower())
        packages.update(get_import_aliases(package_name.lower()))

    # Optional dependencies
    optional_deps = pyproject.get("project", {}).get("optional-dependencies", {})
    for group_deps in optional_deps.values():
        for dep in group_deps:
            package_name = extract_package_name(dep)
            packages.add(package_name.lower())
            packages.update(get_import_aliases(package_name.lower()))

    # Standard library (already lowercase, but be explicit)
    packages.update(get_stdlib_modules())

    return packages


def get_stdlib_modules() -> Set[str]:
    """Return Python standard library module names."""
    stdlib_modules = set()

    # Python 3.10+ has a built-in list
    try:
        import sys

        if hasattr(sys, "stdlib_module_names"):
            stdlib_modules = set(sys.stdlib_module_names)
    except (ImportError, AttributeError):
        pass

    return stdlib_modules


def extract_imports_from_code(code: str) -> Set[str]:
    """Extract import statements from Python code."""
    lines = [line for line in code.split("\n") if not line.strip().startswith(("%", "!"))]
    code = "\n".join(lines)

    imports = set()

    try:
        tree = ast.parse(code)
        for node in ast.walk(tree):
            if isinstance(node, ast.Import):
                for alias in node.names:
                    imports.add(alias.name.split(".")[0].lower())
            elif isinstance(node, ast.ImportFrom):
                if node.module:
                    imports.add(node.module.split(".")[0].lower())
    except SyntaxError:
        pass

    return imports


def check_notebook_imports(notebook_path: Path, allowed_packages: Set[str]) -> Set[str]:
    """Check notebook imports against allowed packages."""
    try:
        code_cells = get_code_cells(notebook_path)
    except NotebookParseError:
        raise

    all_imports = set()
    for code in code_cells:
        all_imports.update(extract_imports_from_code(code))

    unauthorized = all_imports - allowed_packages
    return unauthorized


def main():
    """Main entry point."""
    # Create parser with common arguments
    parser = create_base_parser(description="Check notebook imports against pyproject.toml")

    # This script doesn't need additional arguments beyond the common ones
    args = parser.parse_args()

    # Get notebooks to check using common logic
    notebooks, mode = get_notebooks_from_args(args)

    if not notebooks:
        if not args.quiet:
            print(f"No notebooks to check ({mode}).")
        return 0

    # Print header
    print_check_header(len(notebooks), mode, "imports", args.quiet)

    # Load allowed packages
    allowed_packages = load_allowed_packages()
    if args.verbose and not args.quiet:
        print(
            f"Allowed packages: {sorted(allowed_packages)[:10]}... ({len(allowed_packages)} total)"
        )
        print()

    # Check each notebook
    result = HookResult()

    for notebook in notebooks:
        result.increment_checked()

        try:
            unauthorized = check_notebook_imports(notebook, allowed_packages)

            if unauthorized:
                result.add_error(
                    notebook,
                    f"Unauthorized imports: {', '.join(sorted(unauthorized))}",
                    {"unauthorized_packages": sorted(unauthorized)},
                )
                if not args.quiet:
                    print(f"❌ {notebook}")
                    print(f"   Unauthorized: {', '.join(sorted(unauthorized))}")
            elif args.verbose:
                print(f"✅ {notebook}")

        except NotebookParseError as e:
            result.add_error(notebook, f"Parse error: {e}")
            if not args.quiet:
                print(f"⚠️  {notebook}: {e}")

        except Exception as e:
            result.add_error(notebook, f"Unexpected error: {e}")
            if not args.quiet:
                print(f"⚠️  {notebook}: {e}")

    # Print summary
    if not args.quiet:
        print()
        result.print_summary(verbose=args.verbose)

        if result.has_errors():
            print()
            print("To fix:")
            print("  1. Add missing packages: uv add <package-name>")
            print("  2. Or remove the imports from notebooks")
            print("  3. Or add to optional dependencies if specialized")

    return result.exit_code()


if __name__ == "__main__":
    sys.exit(main())
