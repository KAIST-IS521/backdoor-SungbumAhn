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
	os.system("/usr/bin/ocaml str.cma /home/SungbumAhn/KAIST/backdoor-SungbumAhn/compiler/compiler.ml ./%s ./out"%name)
	os.system("/home/SungbumAhn/KAIST/backdoor-SungbumAhn/interpreter/build/interpreter ./out")

	fail = puts("Failure\n") + end()
	success= puts("Success\n") + end()

	id_check = puts("User: ")
id_check += gets(0)
	id_check += strcmp(0,"user\x00") # result conatins r0

	pw_check = puts("Password: ")
pw_check += gets(0)
	pw_check += strcmp(0,"password\x00")


code = init()
	code += id_check
fail_adr = line_num(code)+2+line_num(pw_check)+line_num(success)
	code += "ite r0, %d,%d\n"%(line_num(code)+2,fail_adr)
	code += pw_check
	code += "ite r0, %d,%d\n"%(line_num(code)+2,fail_adr)
	code += success
	code += fail


	test(code,"login.mini")
