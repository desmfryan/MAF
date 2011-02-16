import os
import sys
import getopt

currentPathScript = os.path.split(os.path.realpath(__file__))[0]
modulesDir = os.path.abspath(os.path.join(currentPathScript, "..", ".."))

def run(param):
    scriptsDir = currentPathScript
    f = None
    try:
        f = open("TestListQA.txt")
    except:
        print "QA FAILED"
        print "Probem open TestListQA.txt"

    lines = f.readlines()
    python = "python "

    suffix = "QA.py"
    fileSuffix = "FilePattern.ini"

    resultDir = os.path.abspath(os.path.join(modulesDir, "QAResults" , "xml"))
    if not os.path.exists(resultDir):
        os.makedirs(resultDir)

    for line in lines:
        line = line.replace("\r", "").replace("\n", "")
        ruleGroup = line.replace(suffix,"")

        print "QA Running...", ruleGroup

        filePattern = ruleGroup + fileSuffix
  
        rulesFile = os.path.join(currentPathScript, "Rules", filePattern)
        r = open(rulesFile)
        linesRule = r.readlines()
        #  print linesRule
  
        sourceDir = str(linesRule[1]).rsplit("=")[1]
  
        command = python + line + " " + sourceDir + " " + resultDir + "/"
        os.system(command.replace("\"","").replace("\r", "").replace("\n", ""))

    print "QA SUCCESSFUL"


    if(param['LCOVCoverage']):
        baseDir = modulesDir
        externalScriptFile = os.path.join(currentPathScript, "ExternalScripts", "LCOVCoverageScript.py")

        for item in os.listdir(baseDir):
            if (os.path.isfile(os.path.join(baseDir, item))==False):
                if(item.find("maf") != -1):
                    os.system("python " + externalScriptFile + " " + item)
    if(param['cppcheck']):
        baseDir = modulesDir
        externalScriptFile = os.path.join(currentPathScript, "ExternalScripts", "cppcheckScript.py")
        print baseDir
        os.system("python " + externalScriptFile)
          
def usage():
    print "Usage: python ScriptLauncher.py [-h] [-l] [-c]"
    print "-h, --help                    show help (this)"
    print "-l, --enable-LCOVCoverage=    enable LCOV coverage"
    print "-c, --enable-cppcheck=       enable cppcheck tool"
    print 

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hlc", ["help","enable-LCOVCoverage","enable-cppcheck"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    LCOVCoverageFlag = False
    cppcheckFlag=False
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            return
        elif o in ("-l", "--enable-LCOVCoverage"):
            LCOVCoverageFlag = True
        elif o in ("-c", "--enable-cppcheck"):
            cppcheckFlag = True
        else:
            assert False, "unhandled option"

    param = {'LCOVCoverage':LCOVCoverageFlag, 'cppcheck':cppcheckFlag}
    run(param)
    
if __name__ == "__main__":
  main()
