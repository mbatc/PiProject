import requests
import json
import lxml

img_xpath = '//*[@id="rg_s"]/div[IMG_ID]/a/img/@src'
img_xpath = img_xpath.replace('IMG_ID','1')

url = "https://www.google.com.au/search?site=imghp&tbm=isch&source=hp&q="
_search='francis'
_search=_search.replace(" ","+")

with requests.session() as s:
	r = s.get(url + _search)
	tree = lxml.etree.fromstring()
	image = tree.xpath(img_xpath)
	print(image)
