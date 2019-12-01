import requests
import json
from time import localtime, sleep
now = localtime()
date = 10000*now.tm_year + 100*now.tm_mon + now.tm_mday
if(now.tm_hour == 0):
	pre_hour = 23
else:
	pre_hour = now.tm_hour - 1
if(pre_hour < 10):
	hour = '0' + str(100*pre_hour)
else:
	hour = str(100*pre_hour)
print('Date: {}, Time: {}h'.format(date, hour))
key = 'h%2FumCB%2FLJnv4A0%2BREpVRxjLrPbo7%2BrIqBFZqP%2FR7cydJT8quEDnX2KI5PQgr9GLEVbcOhOXyVayOb6Q97295iQ%3D%3D'
url = 'http://newsky2.kma.go.kr/service/SecndSrtpdFrcstInfoService2/ForecastGrib?serviceKey=' + str(key) + '&base_date=' + str(date) + '&base_time=' + hour + '&nx=89&ny=91&numOfRows=10&pageSize=10&pageNo=1&startPage=1&_type=json'
w = requests.get(url)
w = w.json()
try:
	p = w["response"]["body"]["items"]["item"][0]["obsrValue"]
except TypeError:
        print("Don't have the date in API")
else:
        if(p == 0):
                print('No Rain No Snow')
        elif(p == 1):
                print('Rain')
        elif(p == 2):
                print('Rain and Snow')
        elif(p == 3):
                print('Snow')
        else:
                print('Failed to search the weather')
with open('weather.txt', 'w') as f:
	result = str(p)
	f.write(result)
