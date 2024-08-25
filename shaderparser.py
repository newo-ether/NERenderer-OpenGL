# shaderparser.py

from re import match
from os.path import (dirname, exists, samefile)
from sys import exit

mainShaderPath = "src/shader/render/main.glsl"
outputShaderPath = "src/shader/render.glsl"

included = []

def parse(shader):
    file = open(shader, "r")
    lines = file.readlines()
    for i in range(0, len(lines)):
        include = match("#include \"(.+)\"", lines[i])
        if include:
            relativePath = include.group(1)
            folder = dirname(shader)
            includePath = folder + '/' + relativePath
            if not exists(includePath):
                print("[Error] Cannot find include file \""
                      + includePath
                      + "\" while parsing \""
                      + shader
                      + "\".\n")
                file.close()
                exit(-1)

            isIncluded = False
            for item in included:
                if samefile(includePath, item):
                    isIncluded = True

            if isIncluded:
                lines[i] = ""
            else:
                included.append(includePath)
                lines[i] = parse(includePath)

    file.close()
    return ''.join(lines) + '\n'

def main():
    if not exists(mainShaderPath):
        print("[Error] Cannot find main shader file "
              + mainShaderPath
              + ".\n")
        exit(-1)

    file = open(outputShaderPath, "w")
    included.append(mainShaderPath)
    file.write(parse(mainShaderPath))
    print("Successfully generated " + outputShaderPath + ".")
    file.close()

main()
