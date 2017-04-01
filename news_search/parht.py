#coding=utf-8
import glob
import os
from HTMLParser import HTMLParser

class MyHTMLParser(HTMLParser):
	
	def parse(self, html):
		self.parseTitle = ""
		self.parseContent = ""
		self.isTitle = False
		self.isContent = False
		self.feed(html)
		return (self.parseTitle, self.parseContent)
	
	def handle_starttag(self, tag, attrs):
		if tag == "title":
			self.isTitle = True
		elif tag == "article":
			self.isContent = True
		elif (tag == "p" or tag == "br") and self.isContent:
			self.parseContent += '\n'
				
		# print "Start tag:", tag

	def handle_endtag(self, tag):
		if tag == "title":
			self.isTitle = False
		elif tag == "article":
			self.isContent = False
		# print "End tag	:", tag

	def handle_data(self, data):
		if self.isTitle:
			self.parseTitle += data
		if self.isContent:
			self.parseContent += data
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

def parseCore(html):
	parser = MyHTMLParser()
	try:
		return parser.parse(html)
	except Exception, e:
		return ("", "")

def parse(id):
	f = file("html/" + id + ".html", 'r')
	html = f.read()
	f.close()
	f = file("text/" + id + ".txt", 'w')
	title, content = parseCore(html)
	f.write(title + '\n')
	f.write(content)
	f.close()

lst = os.listdir('html/')
lstlen = len(lst)
for i in xrange(lstlen):
	print i, '/', lstlen, lst[i]
	parse(lst[i][:23])
# parse('20110225225332156981012')
