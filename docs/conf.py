extensions = [
project = 'aimbot'
author = 'George Yu'
copyright = '2026, George Yu'
language = 'en'

extensions = [
    'sphinx.ext.intersphinx',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.mathjax',
    'sphinx.ext.viewcode',
    'sphinx_rtd_theme',
]

templates_path = ['_templates']
source_suffix = {'.rst': 'restructuredtext'}
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
master_doc = 'index'
pygments_style = 'default'
intersphinx_mapping = {
    'sphinx': ('https://www.sphinx-doc.org/en/master/', None),
}
html_theme = 'sphinx_rtd_theme'
html_theme_options = {
    'navigation_depth': 5,
}
html_static_path = ['_static']
html_logo = None
html_show_sourcelink = True
html_favicon = None