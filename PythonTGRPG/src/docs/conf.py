project = 'Game Night'
copyright = '2024, Amazomic & Chuckmon'
author = 'Amazomic & Chuckmon'
release = '1.0'

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
    'sphinx.ext.viewcode',
    'sphinx.ext.todo'
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

import os
import sys

sys.path.insert(0, os.path.abspath('..'))

html_theme = 'sphinx_rtd_theme'

html_logo = '../img/logo.png'

autodoc_mock_imports = ["aiohttp", "aiogram"]
