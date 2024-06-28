import json
import hashlib
import os
import sys
import zipfile
import urllib.request
import re
from pathlib import Path
import subprocess

archive = zipfile.ZipFile(os.getenv('MOD_ID') + '.geode', 'r')
eee = archive.open('changelog.md')
eeee = []
for eeeee in eee:
	eeee.append(eeeee.decode('utf-8'))

eeeeee = ''.join(eeee)

changelog = ('## ' + eeeeee.split('##')[1]).replace((eeeeee.split('##')[1]).split('\r')[0], 'What\'s New This Update').replace('\n', "")
modjson = json.loads(archive.read('mod.json'))
versionString = modjson["version"].replace("v","")
modName = modjson["name"]


if os.getenv('GITHUB_OUTPUT'):
	with open(os.getenv('GITHUB_OUTPUT'), 'a') as file:
		file.write(f'cl={changelog}\n')
if os.getenv('GITHUB_OUTPUT'):
	with open(os.getenv('GITHUB_OUTPUT'), 'a') as file:
		file.write(f'version={versionString}\n')
if os.getenv('GITHUB_OUTPUT'):
	with open(os.getenv('GITHUB_OUTPUT'), 'a') as file:
		file.write(f'modname={modName}\n')