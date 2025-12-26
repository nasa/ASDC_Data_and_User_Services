# .git-hooks/test_notebook_utils.py
"""Tests for notebook_utils module."""

import pytest
from notebook_utils import extract_package_name, get_import_aliases


def test_extract_package_name():
    """Test package name extraction."""
    assert extract_package_name("pandas>=2.0.0") == "pandas"
    assert extract_package_name("numpy") == "numpy"
    assert extract_package_name("scikit-learn>=1.3.0") == "scikit-learn"
    assert extract_package_name("package[extra]>=1.0") == "package"


def test_get_import_aliases():
    """Test import alias mapping."""
    assert "sklearn" in get_import_aliases("scikit-learn")
    assert "PIL" in get_import_aliases("pillow")
    assert len(get_import_aliases("pandas")) == 0  # No aliases


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
