# Data Tutorials and User Services for the ASDC

How-to guides, tutorials, and scripts for working with data from missions
archived and distributed by NASA's
[Atmospheric Science Data Center (ASDC)](https://www.earthdata.nasa.gov/centers/asdc-daac).
The site covers CALIPSO, CERES, DSCOVR, MAIA, MISR, MOPITT, POWER, PREFIRE,
SAGE III-ISS, STAQS, TEMPO, and TOLNet.

**Browse the rendered site: <https://nasa.github.io/ASDC_Data_and_User_Services/>**

*This site is under active, open development. Stay tuned for more and ever-evolving content!*

## Getting started (contributors)

You need [Python 3.10+](https://www.python.org/),
[uv](https://docs.astral.sh/uv/), and
[Quarto](https://quarto.org/docs/get-started/).

```bash
git clone https://github.com/nasa/ASDC_Data_and_User_Services.git
cd ASDC_Data_and_User_Services

uv sync              # install Python dependencies
pre-commit install   # enable local quality checks on commit
quarto preview       # live-preview the site at localhost
```

See the [Contributing Guide](https://nasa.github.io/ASDC_Data_and_User_Services/contributing.html)
for notebook style guidelines, content requirements, pre-commit hooks, and data
file policy.

## Questions or issues?

Open an [issue](https://github.com/nasa/ASDC_Data_and_User_Services/issues) or
reach out via the [Earthdata Forum](https://forum.earthdata.nasa.gov/).
