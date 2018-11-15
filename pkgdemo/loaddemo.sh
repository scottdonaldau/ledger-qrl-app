#!/usr/bin/env bash
pip install -U setuptools pyledgerqrl>=0.5.1

SCRIPT_DIR=$(cd $(dirname $0) && pwd)
python -m ledgerblue.loadApp --appFlags 0x00 --tlv --targetId 0x31100003 --delete --fileName ${SCRIPT_DIR}/app.hex --appName QRL --appVersion 0.2.0 --icon 0100000000ffffff00ffffffff1ff8cff367e777ec97edb7e927e6eff6eff7dbdbc3c3ffffffffffff --dataSize 0x000033c0
