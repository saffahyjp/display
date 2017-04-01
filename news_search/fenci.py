#coding=utf-8
import jieba
import sys
import os

reload(sys)
sys.setdefaultencoding('utf-8')

wordToText = [['', []]]
_totId = 0
_wordId = {}
def wordId(word):
	global _totId
	id = _wordId.get(word, None)
	if id:
		return id
	wordToText.append([word, []])
	_totId += 1
	_wordId[word] = _totId
	return _totId

def indexCore(textId, str):
	for id in list(set(map(wordId, jieba.cut_for_search(str)))):
		wordToText[id][1].append(textId)

def index(id):
	f = file("text/" + id + ".txt", 'r')
	str = f.read()
	f.close()
	indexCore(id, str)

lst = os.listdir('text/')
lstlen = len(lst)
for i in xrange(lstlen):
	print i, '/', lstlen, lst[i]
	index(lst[i][:23])
f = file("word/data.txt", 'w')
f.write('\n\n\n\n'.join(map(lambda x: x[0] + '::::' + (','.join(x[1])), wordToText)))
f.close()
