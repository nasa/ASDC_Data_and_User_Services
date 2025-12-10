#!/usr/bin/env python3
"""Check the structure of Jupyter notebooks."""

import re
import sys
import os
import json


# Regex patterns for validating required notebook sections.
# Each heading pattern matches markdown headings (## or ###) with optional whitespace and colons.
PATTERNS = {
    "date_last_modified": [
        r"---\ndate: last-modified\n---",  # Exact YAML frontmatter for last-modified date
    ],
    "summary_or_overview": [
        r"#{2,3}\s*Summary\s*:?\s*\n",  # Matches "## Summary:" or "### Summary"
        r"#{2,3}\s*Overview\s*:?\s*\n",  # Matches "## Overview:" or "### Overview"
    ],
    "prerequisites": [
        r"#{2,3}\s*Prerequisites\s*:?\s*\n",  # Matches "## Prerequisites:" or "### Prerequisites"
    ],
    "author": [
        r"#{2,3}\s*Notebook Author / Affiliation\s*:?\s*\n",  # Full form with affiliation
        r"#{2,3}\s*Notebook Author\s*:?\s*\n",  # "Notebook Author" variant
        r"#{2,3}\s*Author\s*:?\s*\n",  # Short "Author" form
    ],
}


def _parse_notebook(notebook_path):
    """Parses a Jupyter Notebook and extracts the content of Markdown cells.

    Args:
        notebook_path (str): The path to the Jupyter Notebook file (.ipynb).

    Returns:
        A list of strings, where each string is the content of a Markdown cell.
        Returns an empty list if the file is not found or if an error occurs during parsing.
    """
    markdown_cells_content = []

    with open(notebook_path, "r", encoding="utf-8") as f:
        notebook_data = json.load(f)

    for cell in notebook_data["cells"]:
        if cell["cell_type"] == "markdown":
            markdown_cells_content.append("".join(cell["source"]))

    return markdown_cells_content


def check_all_expected_items_present(filename, contents):
    """Inspects Markdown for whether it includes specific items.

    Args:
        filename (str): The path to the Jupyter Notebook file (.ipynb).
        contents (list): A list of strings, where each string is a Markdown cell.

    Returns:
        Whether errors were found, i.e., False if the notebook includes all specific items, True otherwise.
    """
    presence_of = {
        "date_last_modified": False,
        "summary_or_overview": False,
        "prerequisites": False,
        "author": False,
    }

    for content in contents:
        # Check for presence of each regex-defined pattern
        for pattern_name, patterns in PATTERNS.items():
            for pattern in patterns:
                if re.search(pattern, content):
                    presence_of[pattern_name] = True
                    break

    error_found = False
    items_missing = []
    for k, v in presence_of.items():
        if v is False:
            items_missing.append(k)
            error_found = True

    return error_found, items_missing


def main():
    """Main function."""
    filenames = sys.argv[1:]
    errors_found = False

    for filename in filenames:
        if not os.path.exists(filename):
            print(f"Error: File not found: {filename}")
            errors_found = True
            break

        try:
            markdown_contents = _parse_notebook(filename)
        except Exception as e:
            print(f"Error parsing {filename}: {e}")
            return 1

        if markdown_contents:
            errors_found, items_missing = check_all_expected_items_present(
                filename, markdown_contents
            )
            if items_missing:
                print(f"{', '.join(items_missing)} not found in {filename}.")
        else:
            print("No markdown cells found.")

    if errors_found:
        return 1
    else:
        return 0


if __name__ == "__main__":
    sys.exit(main())
