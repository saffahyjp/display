import urllib
import urllib2
import re
import time
from HTMLParser import HTMLParser

URL_ROOT = 'http://news.tsinghua.edu.cn'
URL_HOME = '/publish/thunews/index.html'

class MyHTMLParser(HTMLParser):
	
	def parse(self, html):
		self.parseResult = []
		self.feed(html)
		return self.parseResult
	
	def handle_starttag(self, tag, attrs):
		if tag != "a":
			return
		# print "Start tag:", tag
		for attr in attrs:
			if attr[0] == "href" and attr[1].startswith("/publish/thunews/"):
				self.parseResult.append(attr[1])

	def handle_endtag(self, tag):
		pass
		# print "End tag	:", tag

	def handle_data(self, data):
		pass
		# print "Data	  :", data

	def handle_comment(self, data):
		pass
		# print "Comment  :", data

	def handle_entityref(self, name):
		pass
		# c = unichr(name2codepoint[name])
		# print "Named ent:", c

	def handle_charref(self, name):
		pass
		# if name.startswith('x'):
			# c = unichr(int(name[1:], 16))
		# else:
			# c = unichr(int(name))
		# print "Num ent  :", c

	def handle_decl(self, data):
		pass
		# print "Decl	  :", data

def parse(html):
	parser = MyHTMLParser()
	try:
		return parser.parse(html)
	except Exception, e:
		return []

def open(url):
	try:
		user_agent = 'Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36'
		headers = { 'User-Agent' : user_agent}
		# headers = {}
		values = {}
		# values = {'q' : 'python'}
		data = urllib.urlencode(values)
		req = urllib2.Request(URL_ROOT + url, data, headers)
		req.get_method = lambda: 'GET'	
		response = urllib2.urlopen(req)
		return response.read()
	except Exception, e:
		return ''

visitedPages = set([URL_HOME])
waitPages = [URL_HOME]

def papa():
	while len(waitPages) > 0:
		time.sleep(0.1)
		url = waitPages.pop()
		print len(waitPages), url
		html = open(url)
		reResult = re.search('\d{23}', url)
		if reResult:
			id = reResult.group()
			f = file("html/" + id + ".html", 'w')
			f.write(html)
			f.close()
		sons = parse(html)
		for son in sons:
			if son not in visitedPages:
				visitedPages.add(son)
				waitPages.append(son)

papa()
