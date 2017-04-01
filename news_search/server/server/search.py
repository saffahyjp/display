#encoding=utf-8
from django.http import HttpResponse
from django.template import loader, Context
from django.views.decorators.csrf import csrf_exempt
from django.shortcuts import render_to_response
import sys
import re
import os
import codecs
import copy
import jieba

reload(sys)
sys.setdefaultencoding('utf-8')

f = codecs.open('word/data.txt', 'r', 'utf-8')
raw = f.read().split(u'\r\n\r\n\r\n\r\n')
rawlen = len(raw)
print rawlen
wordToText = [[]]
wordId = {}
idWord = [[]]
f.close()
for i in range(1, rawlen):
	word, text = raw[i].split(u'::::')
	idWord.append(word)
	wordId[word] = i
	tmp = text.split(u',')
	tmp.reverse()
	wordToText.append(tmp)
	if i % 10000 == 0:
		print word

textTitle = {}
textTitle2 = {}
textContent = {}
def readText(fileName):
	id = fileName.replace('.txt', '')
	f = file('text/' + fileName, 'r')
	textTitle[id] = f.readline()
	title2 = '\n'
	while title2 == '\n':
		title2 = f.readline()
	textTitle2[id] = title2
	content = f.read()
	textContent[id] = content
	f.close()

lst = os.listdir('text/')
lstlen = len(lst)
for i in xrange(lstlen):
	if i % 1000 == 0:
		print i, '/', lstlen, lst[i]
	readText(lst[i])

head = """
<html>
<head>
<style type="text/css">
body { font-family: '宋体'; background-color:#ffffee; }
div.dateSty { padding-left: 10%%; font-style:italic; color:#ba3925; }
a { font-family: '楷体'; text-decoration:none; font-size:200%%; font-style:normal; }
strong { font-family: '微软雅黑'; }
div.content { text-indent: 2em; padding-left: 10%%; padding-right: 5%%; }
</style>
<script type="text/javascript">
function prevPage()
{
var p=parseInt(document.getElementById("pageInput").value);
if(p>1)
{
document.getElementById("pageInput").value=p-1;
document.getElementById("searchForm").submit();
}
}
function nextPage()
{
var p=parseInt(document.getElementById("pageInput").value);
var t=parseInt(document.getElementById("totalHidden").value);
if(p<t)
{
document.getElementById("pageInput").value=p+1;
document.getElementById("searchForm").submit();
}
}
</script>
</head>
<body>
<form id="searchForm" method="post" action="/">
<center>
<input type="text" name="key" value="%s">
<select name="filter">
<option value="2"%s>不限时间</option>
<option value="201609"%s>这个月</option>
<option value="2016"%s>今年</option>
<option value="2015"%s>2015年</option>
<option value="2014"%s>2014年</option>
<option value="2013"%s>2013年</option>
<option value="2012"%s>2012年</option>
<option value="2011"%s>2011年</option>
</select>
<input type="submit" value="搜索">
</center>
"""

text = """
<center>
<input type="button" onclick="prevPage()" value="上一页">
第
<input type="text" id="pageInput" name="page" value="%d">
页，共%d页
<input type="button" onclick="nextPage()" value="下一页">
<input type="hidden" id="totalHidden" name="total" value="%d">
</center>
</form>
</body>
</html>
"""

def getContent(id, wordsC):
	wordsC.sort(key=lambda x: -len(x))
	rx = '|'.join(wordsC)
	rx = rx.replace(r'$', r'\$')
	rx = rx.replace(r'(', r'\(')
	rx = rx.replace(r')', r'\)')
	rx = rx.replace(r'*', r'\*')
	rx = rx.replace(r'+', r'\+')
	rx = rx.replace(r'.', r'\.')
	rx = rx.replace(r'[', r'\[')
	rx = rx.replace(r']', r'\]')
	rx = rx.replace(r'?', r'\?')
	rx = rx.replace(r'^', r'\^')
	rx = rx.replace(r'{', r'\{')
	rx = rx.replace(r'}', r'\}')
	html = re.sub(rx, lambda x: r'<font color="#FF0000"><strong>' + x.group() + r'</strong></font>', textContent[id].decode('utf8'))
	# html = html.replace('\n', '~')
	html = re.sub(r'^[^<>]{32,}', lambda x: "……" + x.group()[-12:], html)
	html = re.sub(r'[^<>]{32,}$', lambda x: x.group()[:12] + "……", html)
	html = re.sub(r'[^<>]{32,}', lambda x: x.group()[:12] + "……" + x.group()[-12:], html)
	html = html.replace('\n', '</p><p>')
	return '<p><div class="content">' + html.encode('utf8') + '</div></p>'

@csrf_exempt
def search(request):
	response = HttpResponse(content_type="text/html")
	if request.POST.has_key('key'):
		keys = request.POST['key']
		page = int(request.POST['page'])
		filter = request.POST['filter']
		response.write(head % (keys, 'selected="selected"' if filter == '2' else '', 'selected="selected"' if filter == '201609' else '', 'selected="selected"' if filter == '2016' else '', 'selected="selected"' if filter == '2015' else '', 'selected="selected"' if filter == '2014' else '', 'selected="selected"' if filter == '2013' else '', 'selected="selected"' if filter == '2012' else '', 'selected="selected"' if filter == '2011' else ''))
		# print 'filter', filter
		ITEMS_PER_PAGE = 10
		ss = None
		wordsC = []
		for key in keys.split(' '):
			wordsC.extend(jieba.cut(key))
		for key in wordsC:
			id = wordId.get(key, None)
			# print key
			# print id
			if id:
				if ss == None:
					ss = set(wordToText[id])
				else:
					ss &= set(wordToText[id])
		#
		if ss:
			l = list(ss)
			l.sort()
			l.reverse()
			total = 0
			# print 'page', page
			t = loader.get_template('pagelist.html')
			for x in l[(page - 1) * ITEMS_PER_PAGE:][:10]:
				if x.startswith(filter):
					response.write(t.render({'pages' : [{"id" : x, "title" : textTitle[x], "year" : x[0:4], "month" : x[4:6], "day" : x[6:8]}]}))
					response.write(getContent(x, wordsC) + '</br></br>')
			for x in l:
				if x.startswith(filter):
					total += 1
			if total % 10 == 0:
				total = total // 10
			else:
				total = total // 10 + 1
			if page > total:
				page = 1
			# print len(l)
		else:
			total = 1
			page = 1
	else:
		keys = ''
		page = 1
		total = 1
		filter = '2'
		response.write(head % (keys, 'selected="selected"' if filter == '2' else '', 'selected="selected"' if filter == '201609' else '', 'selected="selected"' if filter == '2016' else '', 'selected="selected"' if filter == '2015' else '', 'selected="selected"' if filter == '2014' else '', 'selected="selected"' if filter == '2013' else '', 'selected="selected"' if filter == '2012' else '', 'selected="selected"' if filter == '2011' else ''))
	response.write(text % (page, total, total))
	return HttpResponse(response)

def static(request):
	reResult = re.search('\d{23}', request.path)
	id = reResult.group()
	html = """
<html><head><style type="text/css">
body { background-color:#ffffee; font-family:'微软雅黑'; }
h1 { color:#ba3925; font-family:'楷体'; }
div.contents { color:#110; font-size:120%; text-indent: 2em; padding-left: 10%; padding-right: 10%; }
</style>
"""
	html += '<title>' + textTitle[id] + '</title>'
	html += '</head><body><center><h1>' + textTitle2[id] + '</h1></center><div class="contents"><p>'
	content = textContent[id]
	content = content.replace('\n', '</p><p>')
	content = content.replace(' ', '&nbsp;')
	html += content + '</div></p></body></html>'
	return HttpResponse(html)
