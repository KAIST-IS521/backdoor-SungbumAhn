import os,sys
"""
r100 = 0
r101 = 1
"""
def error(msg):
	print msg
	sys.exit(-1)

	def line_num(code):
		return len(code.split("\n"))-1
		def init():
			ret = ""
			ret += "puti r101, 1\n"
			return ret

			def end():
				return "halt\n"

				def puts(src,adr=0):
					src+="\x00"
					ret = ""
					ret += "puti r0, %d\n"%adr
					ret += "move r2,r0\n"
					for x in src:
					ret += "puti r1,%d\n"%ord(x)
					ret += "store r0,r1\n"
					ret += "add r0,r0,r101\n"
					ret += "puts r2\n"
					return ret

					def gets(adr):
						ret = ""
						if adr == 0:
						ret += "gets r100\n"
						elif adr<0x100:
						ret += "puti r0, %d\n"%adr
						ret += "gets r0\n"
						else:
						error("nono...")
						return ret

						def strcmp(adr,s):
							ret = ""
							ret += "puti r1, %d\n"%adr
							ret += "move r0, r100\n"
							for x in s:
							ret += "load r2, r1\n"
							ret += "puti r3, %d\n"%ord(x)
							ret += "eq r4,r2,r3\n"
							ret += "add r1, r1, r101\n"
							ret += "add r0,r0,r4\n"
							ret += "puti r1,%d\n"%(len(s) )
							ret += "eq r0, r0,r1\n"
							return ret

							def test(code,name="tmp"):
							with open(name, "wb") as f:
f.write(code)
	os.system("/usr/bin/ocaml str.cma /home/daramg/KAIST/backdoor-DaramG/compiler/compiler.ml ./%s ./out"%name)
	os.system("/home/daramg/KAIST/backdoor-DaramG/interpreter/build/interpreter ./out")

	put1 = puts("1\n")
	put2 = puts("2\n")
	put3 = puts("3\n")


code = init()
	code += puts("Input:\n")
	code += "store r100,r100\n"
	code += "store r101,r100\n"
code += gets(0)
# tmp = buf[0] + buf[1]
	code += "load r50,r100\n"
	code += "load r51,r101\n"
	code += "add r53,r50,r51\n"
	code += "sub r52,r50,r51\n"
	code += "store r100, r52\n"
	code += "puts r100\n"
	code += puts("\n")

	code += "puti r54,65\n"
	code += "eq r0,r53,r54\n"
	code += "ite r0,%d,%d\n"%(line_num(code)+2,line_num(code)+line_num(put1)+2)
	code += put1

	code += "puti r54,65\n"
	code += "gt r0,r53,r54\n"
	code += "ite r0,%d,%d\n"%(line_num(code)+2,line_num(code)+line_num(put2)+2)
	code += put2

	code += "puti r54,32\n"
	code += "ge r0,r53,r54\n"
	code += "ite r0,%d,%d\n"%(line_num(code)+2,line_num(code)+line_num(put3)+2)
	code += put3

	code += "jump %d\n"%(line_num(code)+1)
code += end()

	test(code,"test.mini")

