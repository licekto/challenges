#!/bin/bash

python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
pytest -n 16
#deactivate
#rm -rf venv
