from antlr4 import *
from UserIntercationLexer import UserIntercationLexer
from UserIntercationParser import UserIntercationParser
from ExtractUserIntercationListener import ExtractUserIntercationListener
import time, subprocess

def main(input_):
	start_time = time.time()
	lexer = UserIntercationLexer(input_)
	tokens = CommonTokenStream(lexer)
	parser = UserIntercationParser(tokens)
	tree = parser.prog()
	# print(tree.toStringTree(recog=parser))
	walker = ParseTreeWalker()
	extractor = ExtractUserIntercationListener(parser)
	try:
		walker.walk(extractor, tree)
	except Exception as e:
		print(e)
	end_time = time.time()
	print('Time: ', end_time - start_time)
	return

if __name__ == '__main__':
	input_ = input('please input: ')
	while input_ != 'exit':
		if ('SET' in input_) and ('DRIVING PATTERN' in input_):
			file_name = input_.split(' ')[-1]
			input_ = FileStream(file_name)
			command = "rm pattern/*"
			try:
				subprocess.run(command, shell=True, check=True)
			except subprocess.CalledProcessError as e:
				pass
		else:
			input_ = InputStream(input_)
		main(input_) 
		input_ = input('please input: ')
