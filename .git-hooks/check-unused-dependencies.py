#!/usr/bin/env python3
"""Check for dependencies declared in pyproject.toml but not imported in notebooks.

Identifies potentially unused packages to keep dependencies lean.
See .git-hooks/README.md for details and usage examples.
"""

import sys

try:
    import tomllib  # Python 3.11+
except ImportError:
    import tomli as tomllib
from pathlib import Path
from typing import Set, Dict, List

from notebook_utils import (
    get_code_cells,
    HookResult,
    NotebookParseError,
    create_base_parser,
    find_all_notebooks,
    extract_package_name,
    get_import_aliases,
)

# Packages that are necessary but may not be directly imported
INFRASTRUCTURE_PACKAGES = {
    "jupyter",
    "ipykernel",
    "ipython",
    "notebook",
    "jupyterlab",
    "quarto",
    "pre-commit",
    "tomli",
    "tomllib",
}


def load_declared_packages() -> Dict[str, Set[str]]:
    """Load package names from pyproject.toml, organized by group.

    Returns:
        Dict with keys: 'main', 'optional:<group_name>'
    """
    pyproject_path = Path("pyproject.toml")

    if not pyproject_path.exists():
        print("Warning: pyproject.toml not found")
        return {}

    with open(pyproject_path, "rb") as f:
        pyproject = tomllib.load(f)

    packages_by_group = {}

    # Main dependencies
    dependencies = pyproject.get("project", {}).get("dependencies", [])
    packages_by_group["main"] = {extract_package_name(dep).lower() for dep in dependencies}

    # Optional dependencies
    optional_deps = pyproject.get("project", {}).get("optional-dependencies", {})
    for group_name, group_deps in optional_deps.items():
        key = f"optional:{group_name}"
        packages_by_group[key] = {extract_package_name(dep).lower() for dep in group_deps}

    return packages_by_group


def extract_all_imports_from_code(code: str) -> Set[str]:
    """Extract import statements from Python code."""
    import ast

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


def collect_all_imports(notebooks: List[Path]) -> Set[str]:
    """Collect all unique imports across all notebooks."""
    all_imports = set()

    for notebook in notebooks:
        try:
            code_cells = get_code_cells(notebook)
            for code in code_cells:
                imports = extract_all_imports_from_code(code)
                all_imports.update(imports)
        except NotebookParseError:
            continue  # Skip problematic notebooks

    return all_imports


def find_unused_packages(
    declared_packages: Dict[str, Set[str]], used_imports: Set[str]
) -> Dict[str, List[str]]:
    """Find packages declared but not imported.

    Returns:
        Dict of {group_name: [unused_packages]}
    """
    unused_by_group = {}

    for group_name, packages in declared_packages.items():
        unused = []

        for package in sorted(packages):
            # Skip infrastructure packages
            if package.lower() in INFRASTRUCTURE_PACKAGES:
                continue

            # Check if package or any of its aliases are imported
            possible_imports = get_import_aliases(package.lower())
            if not any(imp in used_imports for imp in possible_imports):
                unused.append(package)

        if unused:
            unused_by_group[group_name] = unused

    return unused_by_group


def main():
    """Main entry point."""
    # Create parser
    parser = create_base_parser(
        description="Check for declared dependencies not imported in any notebook"
    )
    parser.add_argument(
        "--include-optional", action="store_true", help="Also check optional dependency groups"
    )

    args = parser.parse_args()

    # Always check all notebooks for this analysis
    exclude_patterns = [".ipynb_checkpoints"]
    if args.exclude_archived:
        exclude_patterns.append("archived")
    if args.exclude_additional_drafts:
        exclude_patterns.append("additional_drafts")

    notebooks = find_all_notebooks(exclude_patterns=exclude_patterns)

    if not notebooks:
        if not args.quiet:
            print("No notebooks found to analyze.")
        return 0

    if not args.quiet:
        exclusions = []
        if args.exclude_archived:
            exclusions.append("archived")
        if args.exclude_additional_drafts:
            exclusions.append("drafts")
        mode = "all" if not exclusions else f"all, excluding {', '.join(exclusions)}"
        print(f"Analyzing imports from {len(notebooks)} notebook(s) ({mode})...")
        print()

    # Load declared packages
    declared_packages = load_declared_packages()

    if not declared_packages:
        print("No dependencies found in pyproject.toml")
        return 0

    # Collect all imports
    if not args.quiet:
        print("Scanning notebooks for imports...")
    used_imports = collect_all_imports(notebooks)

    if args.verbose and not args.quiet:
        print(f"Found {len(used_imports)} unique imports across all notebooks")
        print()

    # Find unused packages
    groups_to_check = ["main"]
    if args.include_optional:
        groups_to_check.extend([k for k in declared_packages.keys() if k.startswith("optional:")])

    declared_to_check = {k: v for k, v in declared_packages.items() if k in groups_to_check}
    unused_packages = find_unused_packages(declared_to_check, used_imports)

    # Report results
    result = HookResult()

    if unused_packages:
        if not args.quiet:
            print("=" * 70)
            print("\n⚠️  Found potentially unused dependencies:\n")

        for group_name, packages in unused_packages.items():
            display_name = (
                "Main dependencies"
                if group_name == "main"
                else f"Optional: {group_name.split(':')[1]}"
            )

            if not args.quiet:
                print(f"{display_name}:")
                for package in packages:
                    print(f"  - {package}")
                print()

            # Add as warnings, not errors (they might be used indirectly)
            for package in packages:
                result.add_warning(
                    Path("pyproject.toml"),
                    f"{package} declared but not imported",
                    {"group": group_name},
                )

        if not args.quiet:
            print("NOTE: These packages may still be needed as:")
            print("  - Transitive dependencies")
            print("  - Runtime requirements")
            print("  - Development tools")
            print()
            print("Review before removing. To remove:")
            print("  uv remove package-name")
    else:
        if not args.quiet:
            print("=" * 70)
            print("✅ All declared dependencies are imported in notebooks")

    # For this check, warnings don't cause failure (informational only)
    return 0


if __name__ == "__main__":
    sys.exit(main())
