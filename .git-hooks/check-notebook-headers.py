#!/usr/bin/env python3
"""Check notebooks for required documentation sections.

Validates presence of: date header, summary/overview, prerequisites, and author.
See .git-hooks/README.md for details and usage examples.
"""

import re
import sys
from pathlib import Path
from typing import List, Tuple

from notebook_utils import (
    get_markdown_cells,
    HookResult,
    NotebookParseError,
    create_base_parser,
    get_notebooks_from_args,
    print_check_header,
)


# Define required elements and their patterns
REQUIRED_ELEMENTS = {
    "date_last_modified": {
        "patterns": [r"---\s*\ndate:\s*last-modified\s*\n---\s*"],
        "description": "Date last modified header (---\\ndate: last-modified\\n---)",
    },
    "summary_or_overview": {
        "patterns": [
            r"###\s*Summary\s*:?\s*\n",
            r"##\s*Summary\s*:?\s*\n",
            r"###\s*Overview\s*:?\s*\n",
            r"##\s*Overview\s*:?\s*\n",
        ],
        "description": "Summary or Overview section",
    },
    "prerequisites": {
        "patterns": [
            r"###\s*Prerequisites\s*:?\s*\n",
            r"##\s*Prerequisites\s*:?\s*\n",
        ],
        "description": "Prerequisites section",
    },
    "author": {
        "patterns": [
            r"###\s*Notebook Author[s]?\s?\/\s?Affiliation\s*:?\s*\n",
            r"##\s*Notebook Author[s]?\s?\/\s?Affiliation\s*:?\s*\n",
            r"###\s*Author\s*:?\s*\n",
            r"##\s*Author\s*:?\s*\n",
            r"###\s*Notebook Author\s*:?\s*\n",
            r"##\s*Notebook Author\s*:?\s*\n",
        ],
        "description": "Author section",
    },
}


def check_pattern_in_content(content: str, patterns: List[str]) -> bool:
    """Check if any of the regex patterns match the content."""
    for pattern in patterns:
        if re.search(pattern, content, re.MULTILINE):
            return True
    return False


def check_notebook_headers(notebook_path: Path) -> Tuple[bool, List[str]]:
    """Check if notebook contains all required header elements."""
    try:
        markdown_cells = get_markdown_cells(notebook_path)
    except NotebookParseError:
        raise

    if not markdown_cells:
        return True, ["No markdown cells found"]

    presence = {element: False for element in REQUIRED_ELEMENTS.keys()}

    for content in markdown_cells:
        for element_name, element_info in REQUIRED_ELEMENTS.items():
            if check_pattern_in_content(content, element_info["patterns"]):
                presence[element_name] = True

    missing = [
        REQUIRED_ELEMENTS[name]["description"] for name, found in presence.items() if not found
    ]

    has_errors = len(missing) > 0
    return has_errors, missing


def main():
    """Main entry point."""
    # Create parser with common arguments
    parser = create_base_parser(description="Check notebook structure for required header content")

    # Add script-specific arguments
    parser.add_argument(
        "files", nargs="*", help="Specific files to check (overrides --all-notebooks)"
    )

    args = parser.parse_args()

    # Get notebooks to check using common logic
    notebooks, mode = get_notebooks_from_args(args)

    if not notebooks:
        if not args.quiet:
            print(f"No notebooks to check ({mode}).")
        return 0

    # Print header
    print_check_header(len(notebooks), mode, "headers", args.quiet)

    # Check each notebook
    result = HookResult()

    for notebook in notebooks:
        if not notebook.exists():
            result.add_error(notebook, "File not found")
            if not args.quiet:
                print(f"⚠️  {notebook}: File not found")
            continue

        result.increment_checked()

        try:
            has_errors, missing = check_notebook_headers(notebook)

            if has_errors:
                result.add_error(
                    notebook,
                    f"Missing required elements: {', '.join(missing)}",
                    {"missing_elements": missing},
                )
                if not args.quiet:
                    print(f"❌ {notebook}")
                    print(f"   Missing: {', '.join(missing)}")
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
            print("Required elements:")
            for element_name, element_info in REQUIRED_ELEMENTS.items():
                print(f"  - {element_info['description']}")

    return result.exit_code()


if __name__ == "__main__":
    sys.exit(main())
