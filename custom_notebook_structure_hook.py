#!/usr/bin/env python3
"""Check the structure of Jupyter notebooks."""

import re
import sys
import os
import json


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


def _contains_any_substring(text, substrings, case_sensitive=False, strip_spaces=True):
    """Checks if a string contains any of the substrings in a list (Case insensitive).

    Args:
      text: The string to search in.
      substrings: A list of substrings to search for.

    Returns:
      True if the string contains at least one of the substrings, False otherwise.
    """
    if not case_sensitive:
        text = text.lower()
    if strip_spaces:
        text = re.sub(r" +", "", text).strip()

    for substring in substrings:
        this_substring = substring
        if not case_sensitive:
            this_substring = this_substring.lower()
        if strip_spaces:
            this_substring = re.sub(r" +", "", this_substring).strip()

        if this_substring in text:
            return True
    return False


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
        if content == "---\ndate: last-modified\n---":
            presence_of["date_last_modified"] = True

        if _contains_any_substring(
            content,
            [
                "### Summary\n",
                "## Summary\n",
                "### Overview\n",
                "## Overview\n",
            ],
        ):
            presence_of["summary_or_overview"] = True

        if _contains_any_substring(content, ["### Prerequisites\n", "## Prerequisites\n"]):
            presence_of["prerequisites"] = True

        if _contains_any_substring(
            content,
            [
                "### Notebook Author / Affiliation\n",
                "## Notebook Author / Affiliation\n",
                "### Author\n",
                "## Author\n",
                "### Notebook Author\n",
                "## Notebook Author\n",
            ],
        ):
            presence_of["author"] = True

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
