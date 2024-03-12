import json
import hashlib
import os
import sys
import zipfile
import urllib.request
import re


archive = zipfile.ZipFile('beat.click-sound.geode', 'r')
e = archive.open('changelog.md')
file_list = archive.namelist()	



def send_webhook(eee):
	from urllib import request
	import requests
	import json
	import os

	req = request.Request(os.getenv('DISCORD_WEBHOOK_URL'), method='POST')
	req.add_header('User-Agent', 'python urllib')
	req.add_header('Content-Type', 'application/json')
	data = {
		'content': "# New Test"
	}
	# request.urlopen(req, data=json.dumps(data).encode('utf-8'))
	requests.post(os.getenv('DISCORD_WEBHOOK_URL'), data=data, files={"file": open('beat.click-sound.geode', "rb")})

send_webhook(e)
