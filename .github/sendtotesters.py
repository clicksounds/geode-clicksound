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
        'all': '1217241657548865556'
        'windows': '1217241755578142780'
        'mac': '1217241792106205284'
        'android': '1217241827912978462'
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
