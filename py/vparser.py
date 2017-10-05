#!/usr/bin/env python2.7

# -*- coding: utf-8 -*-
# @Author: simon
# @Date:   2017-08-11 10:20:46
# @Last Modified by:   simon
# @Last Modified time: 2017-09-29 12:30:13

# coding=utf8
# the above tag defines encoding for this document and is for Python 2.x compatibility


import re, mmap, sys, os


def parse( file ):

    regex = r"((?!generate)\b[-_A-Za-z0-9]+)\s+([-_A-Za-z0-9]+)\s*\("
    with open(file, 'r+') as f:
        data = mmap.mmap(f.fileno(), 0)

    matches = re.finditer(regex, data)    
    f.close( );

    return matches


def checkStyle( stringerino ):

    regex = r"^(?!\/\/)\s+(input|output)\s+.*[a-zA-Z0-9_]+\s*"
    matches = re.finditer(regex, stringerino, re.MULTILINE)
    
    for match in matches:
        return"NEW"
    return "OLD"

def checkcomments( stringerino, type_ ):

    regex = (r"//-=-=-=-=-=-=-=-=-=-=-\s*\n"
    r"\s*//\s*\n"
    r"\s*//The following "+type_+"(.|\s)*?(?=generated)generated\s*\n"
    r"\s*//\s")


    comment = list()
    stringer = ""
    matches = re.finditer(regex, stringerino, re.MULTILINE)
    for match in matches:
        if match.group(0):           
            return match
    return ""

def getModuledefinition( name, matches ):
    for match in matches:
        if name in match.group(1):
            return  match
    return ""

def getinstantiations( stringerino, modulename, name ):
    
   # if "[" in name and "." in name:
    #    if len(name) > 2 :
     #       name = name.split(".")[1]
    tmp_name = name
    regex = r"(?!\/\/)\s*("+modulename+")[-+\s|\w`#(.),/\"]*?\s+("+name+")\s*\("
    if "." in name:
        name = name.split(".")
        regex = r"(?!\/\/)\s*"+name[0]+"\s*[\s|\S]*?\s("+modulename+")[\s|\w`#(.),/]*?\s+("+name[1]+")\s*\("
        name = name[1]

    #end_reg     = r"^[#(.)*,'=_a-zA-Z0-9\t -]*(\);).*$"
    end_reg     = r"^[\s\S]*?(\)\s*\;).*$"

    matches = re.finditer(regex, stringerino, re.MULTILINE)
    mid = re.finditer(end_reg, stringerino, re.MULTILINE)
    for match in matches:
        if modulename in match.group(1) and name in match.group(2):
            for match_mid in mid:
                if match_mid.start(1) > match.end(0):
                    return match, match_mid
                    break;
    print("NOT FOUND IN PYTHON: "+modulename +" "+ name + "**  "+stringerino)
    return "", ""

def find_wires(stringerino):
    inouts = r"^(?!\/\/|\n)(`ifdef.*|`ifndef.*)?\s*(input|output|inout)\s+.*$"
    wires  = r"^(?!\/\/|\n)(`ifdef.*|`ifndef.*)?\s*(wire)\s+.*$"

    matches_inouts = re.finditer(inouts, stringerino, re.MULTILINE )
    matches_wires  = re.finditer(wires, stringerino, re.MULTILINE )

    match_1 = match_2 = ""
    sio = eio = 0
    sw = ew = 0

    mtch = checkcomments( stringerino,"wires" )
    if mtch:
        return 0,0,mtch.end(0),mtch.end(0)

    i = 0
    for match_1 in matches_inouts:
        if i == 0:
            sio = match_1.start(0)
        i += 1
        eio = match_1.end(0)    

    j = 0
    for match_2 in matches_wires:
        if j == 0:
            sw = match_2.start(0)
        j += 1
        ew = match_2.end(0)

    return sio,eio,sw,ew


def checkforgenerate( stringerino, modulename, name ):

    if "." in name:
        name = name.split(".")[1]
    regex = r"genvar.+?(?=endgenerate)"
    regex1 = r"("+modulename+").*?("+name+")"
    matches = re.finditer(regex, stringerino, re.DOTALL)
    start = 0
    end = 0
    ret = list()
    for match in matches:
        if match.group( 0 ):
            start = match.start(0)
            end = match.end(0)
            tmp = re.finditer(regex1, str(match.group(0)), re.DOTALL)
            for mtch in tmp:
                return match
    return ""

### expects the path of a module and extrace all moduledefinitions
def getModules( file ):

    '''
   
        module modulename(  <-  start_reg is looking for strings which look like this
        .
        .                    <- sector 1
        .
        . 
        );                  <- mid_re is looking for ");"
        .
        .                    <- sector 2
        .   
        .   
        endmodule           <- end_reg is looking for the string endmodule
    
        in sector 1 we place the inputs and outputs (optional in section 2, depends on the style of the file)
        in sector 2 we search for instantiations and we place ios and/or wires

    '''
    start_reg   = r"^(?!\/\/)module\s+([a-zA-Z0-9_-]+)\s*(#\((\s|\S)*\))*\s*\("
    mid_reg     = r"^[#(.)*,'=_a-zA-Z0-9\t -]*(\);).*$"
    end_reg     = r"^[#(.)*,'=_a-zA-Z0-9\t -]*(endmodule).*$"

    data = ""
    with open(file, 'r+') as f:
        data = mmap.mmap(f.fileno(), 0)

    modules = re.finditer(start_reg , data, re.MULTILINE)
    mid = re.finditer(mid_reg       , data, re.MULTILINE)
    end = re.finditer(end_reg       , data, re.MULTILINE)


    '''
        save the names of the modules in the dictionary

    '''

    modulelist = dict()
    for match in modules:
        modulelist[match.group(1)] = list()
        modulelist[match.group(1)].append(match.start(0))
        modulelist[match.group(1)].append(match.end(0))
        for match_mid in mid:
            if match_mid.start(1) > match.start(0):
                modulelist[match.group(1)].append(match_mid.start(0))
                modulelist[match.group(1)].append(match_mid.end(0))
                for match_end in end:
                    if match_end.start(1) > match_mid.start(1):
                        modulelist[match.group(1)].append(match_end.start(0))
                        modulelist[match.group(1)].append(match_end.end(0))
                        break
                break

    f.close( );
    return data, modulelist

def sendback( fdw, module, inst, instend, data, generate ):

    style           = "NEW" 
    comment         = ""
    comment_inst    = ""
    comment_mod     = ""
    comment_wire    = ""
    gen             = ""
    start_gen       = 0
    end_gen         = 0
    start_inst      = 0
    end_inst        = 0
    start_mod       = 0
    end_mod         = 0
    start_mod_old   = 0
    end_mod_old     = 0
    start_com       = 0
    end_com         = 0
    start_wires     = 0
    end_wires       = 0
    tmp1            = ""


    if module and len(module) > 4:
        start_mod       = module[1]
        end_mod         = module[3]
        start_mod_old   = module[3]
        end_mod_old     = module[4]
        tmp1 = checkcomments(data[start_mod:end_mod], type_ = "IO's")
        tmp2 = checkcomments(data[start_mod_old:end_mod_old], type_ = "IO's")
        style = checkStyle(data[start_mod:end_mod])

        if tmp1:
            comment_mod = "YES"
            start_mod += tmp1.end(0)
            end_mod += tmp1.end(0)

            if tmp2:
                start_mod_old += tmp2.end(0)
                end_mod_old += tmp2.end(0)

    if inst:
        try:
            start_inst  = inst.end(0) + module[0]
            end_inst    = instend.start(1) + module[0]
            tmp1 = checkcomments(data[start_inst:end_inst], type_ ="connection")
        except:
            print("Couldn't find instance ")

        if tmp1:
            comment_inst = "YES"
            start_inst += tmp1.end(0)
            end_inst += tmp1.end(0)
        if generate:
            gen = "GEN"


    sio = eio = sw = ew = 0
    sio,eio,sw,ew = find_wires( data[start_mod_old: end_mod_old] )
    if sio == 0 and eio == 0:
        comment_wire = "YES"
    sio += start_mod_old
    eio += start_mod_old
    sw += start_mod_old
    ew += start_mod_old


    module_send = "[Module]:({start}-{end},{start_}-{end_})[comment]{comment}[style]{style}".format(start = start_mod, end = end_mod, start_ = start_mod_old, end_ = end_mod_old, style = style, comment = comment_mod)
    Instan_send = "\n[Instantiation]:({start}-{end},{start_}-{end_})[comment]{comment}".format(start = start_inst, end = end_inst, start_ = 0, end_ = 0, comment = comment_inst)
    Genera_send = "\n[Generate]:({start}-{end},{start_}-{end_})[generate]{generate}".format(start = start_gen, end = end_gen, start_ = 0, end_ = 0, generate = gen)
    Wires_send  = "\n[Wires]:({start}-{end},{start_}-{end_})[comment]{comment_wire}".format(start = sio, end = eio, start_ = sw, end_ = ew,comment_wire = comment_wire)

    msg = module_send + Instan_send + Genera_send + Wires_send + "\n\0"

    os.write(fdw,""+msg+"")

def read_( matches, fdr, fdw, path ):

    bufferstr = ""
    data = ""
    name = ""
    inst = ""
    generate = ""
    modulelist = dict()
    while "\nEOF\n" not in bufferstr:
        bufferstr = os.read( fdr, 2048 )
        if not bufferstr:
            continue
        print("BUFFERSTR: "+bufferstr)
        print(path)
        if "\nEOF\n" in bufferstr:
            break
        if "LOAD:" in bufferstr:
            pth = (bufferstr.split(":"))
            path = pth[1].split(";")[0]                     
            data, modulelist = getModules( path )

            if not bufferstr:
                continue

            tt =  bufferstr.split(";")
            
            if( len(tt) < 2 ):
                continue
            bufferstr = tt[1]

            if "|" not in bufferstr:
                continue

        
        ar = bufferstr.split("|")

        ### Instances

        if len(ar) == 3 and "(null)" not in ar:

            try:
                print(modulelist)
                moddef = data[ modulelist[ar[2]][0] : modulelist[ar[2]][5] ]
                instdef, instend = getinstantiations( moddef, ar[0], ar[1] )
                name = ar[2] 
                generate = checkforgenerate( moddef, ar[0], ar[1] )
            except KeyError:
                print("KeyError: Couldn't find "+name+" in file: "+path+"\nPiped in was: "+bufferstr+"\n")
            except IndexError:
                print("IndexError: Couldn't find "+name+" in file: "+path)
                print("Please check the original file")

        ### Modules
        else:
            name = ar[1]
            instdef = list()
            instend = ""
            
        for key, value in modulelist.iteritems():
            if len(value) < 6:
                print("ERROR :")
                print( data[value[0]:value[1]])
                print( path )
                print("_____________________________")

        module = ""
        try:
            module = modulelist[name]
        except KeyError:
            print("Couldn't find "+name+" in file: "+path)
            

        
        
        sendback( fdw, module , instdef, instend, data, generate )
      
    return

def main( argv ):
    print("vparser.py")
    r = int(argv[1])
    w = int(argv[0])
    fdw = os.fdopen(w,'w')
    fdr = os.fdopen(r,'r')
    matches = ""
    read_( matches, r, w, argv[2] )

    os.write( w, "|shutdown|\0")
    fdw.close()
    fdr.close()
    
if __name__ == "__main__":
   main(sys.argv[1:])
