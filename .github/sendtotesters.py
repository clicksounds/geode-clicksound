import json
import hashlib
import os
import sys
import zipfile
import urllib.request
import re


archive = zipfile.ZipFile('beat.click-sound.geode', 'r')
e = json.loads(archive.read('mod.json'))
file_list = archive.namelist()	



def send_webhook(eee):
	import requests
	import json
	import os

	thePing = "\n## Testers: <@&"

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

	data = {
		'content': "# New Test for v" + eee["version"].replace("v","") + thePing + "\n" + os.getenv('DESC')
	}
	requests.post(os.getenv('DISCORD_WEBHOOK_URL'), data=data, files={"file": open('beat.click-sound.geode', "rb")})

send_webhook(e)
