import json
import hashlib
import os
import sys
import zipfile
import urllib.request
import re


archive = zipfile.ZipFile('beat.click-sound.geode', 'r')
modjson = json.loads(archive.read('mod.json'))
changelog = archive.read('changelog.md')
file_list = archive.namelist()	



def send_webhook(modjson, changelog):
	import requests
	import json
	import os

	thePing = "\n## Testers: <@"

	leping = {
        'all': '1210765582685966387',
        'windows': '1210765242502615120',
        'mac': '1209630974787452998',
        'android': '1210765328519536704'
	}
	if not 'none' in os.getenv('PING'):
		thePing += leping[os.getenv('PING')] + ">"
	else:
		thePing = ""

	changelog = changelog.split("##")[1]
	changelogsplit = changelog.split("\n")
	changelog = "\n".join(changelogsplit[1:(len(changelogsplit) - 1)])

	data = {
		'content': "# New Test for v" + modjson["version"].replace("v","") + thePing + "\n## What to test\n- " + os.getenv('DESC').replace("\\n", "\n") + "\n## Changelog: \n" + changelog
	}
	requests.post(os.getenv('DISCORD_WEBHOOK_URL'), data=data, files={"file": open('beat.click-sound.geode', "rb")})

send_webhook(modjson, changelog)
