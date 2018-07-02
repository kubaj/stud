#!/usr/bin/env bash

# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# IPP - cha - doplňkové testy - 2016/2017
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Činnost: 
# - vytvoří výstupy studentovy úlohy v daném interpretu na základě sady testů
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
# Popis (README):
#
# Struktura skriptu _stud_tests.sh (v kódování UTF-8):
# Každý test zahrnuje až 4 soubory (vstupní soubor, případný druhý vstupní 
# soubor, výstupní soubor, soubor logující chybové výstupy *.err vypisované na 
# standardní chybový výstup (pro ilustraci) a soubor logující návratový kód 
# skriptu *.!!!). Pro spuštění testů je nutné do stejného adresáře zkopírovat i 
# váš skript. V komentářích u jednotlivých testů jsou uvedeny dodatečné 
# informace jako očekávaný návratový kód.
# Tyto doplňující testy obsahují i několik testů rozšíření (viz konec skriptu).
#
# Proměnné ve skriptu _stud_tests.sh pro konfiguraci testů:
#  INTERPRETER - využívaný interpret 
#  EXTENSION - přípona souboru s vaším skriptem (jméno skriptu je dáno úlohou) 
#  LOCAL_IN_PATH/LOCAL_OUT_PATH - testování různých cest ke vstupním/výstupním
#    souborům
#  
# Další soubory archivu s doplňujícími testy:
# V adresáři ref-out najdete referenční soubory pro výstup (*.out nebo *.xml), 
# referenční soubory s návratovým kódem (*.!!!) a pro ukázku i soubory s 
# logovaným výstupem ze standardního chybového výstupu (*.err). Pokud některé 
# testy nevypisují nic na standardní výstup nebo na standardní chybový výstup, 
# tak může odpovídající soubor v adresáři chybět nebo mít nulovou velikost.
# V adresáři s tímto souborem se vyskytuje i soubor cha_options 
# pro nástroj JExamXML, který doporučujeme používat na porovnávání XML souborů. 
# Další tipy a informace o porovnávání souborů XML najdete na Wiki IPP (stránka 
# https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IPP:ProjectNotes&id=9999#XML_a_jeho_porovnávání).
#
# Doporučení a poznámky k testování:
# Tento skript neobsahuje mechanismy pro automatické porovnávání výsledků vašeho 
# skriptu a výsledků referenčních (viz adresář ref-out). Pokud byste rádi 
# využili tohoto skriptu jako základ pro váš testovací rámec, tak doporučujeme 
# tento mechanismus doplnit.
# Dále doporučujeme testovat různé varianty relativních a absolutních cest 
# vstupních a výstupních souborů, k čemuž poslouží proměnné začínající 
# LOCAL_IN_PATH a LOCAL_OUT_PATH (neomezujte se pouze na zde uvedené triviální 
# varianty). 
# Výstupní soubory mohou při spouštění vašeho skriptu již existovat a pokud není 
# u zadání specifikováno jinak, tak se bez milosti přepíší!           
# Výstupní soubory nemusí existovat a pak je třeba je vytvořit!
# Pokud běh skriptu skončí s návratovou hodnotou různou od nuly, tak není 
# vytvoření souboru zadaného parametrem --output nutné, protože jeho obsah 
# stejně nelze považovat za validní.
# V testech se jako pokaždé určitě najdou nějaké chyby nebo nepřesnosti, takže 
# pokud nějakou chybu najdete, tak na ni prosím upozorněte ve fóru příslušné 
# úlohy (konstruktivní kritika bude pozitivně ohodnocena). Vyhrazujeme si také 
# právo testy měnit, opravovat a případně rozšiřovat, na což samozřejmě 
# upozorníme na fóru dané úlohy.
#
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

TASK=cha
#INTERPRETER="php5.6"
#EXTENSION=php
INTERPRETER="python3.6"
EXTENSION=py

# cesty ke vstupním a výstupním souborům
LOCAL_IN_PATH="./" # (simple relative path)
LOCAL_IN_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_IN_PATH3="`pwd`/" #Alternative 2 (absolute path)
LOCAL_OUT_PATH="./" # (simple relative path)
LOCAL_OUT_PATH2="" #Alternative 1 (primitive relative path)
LOCAL_OUT_PATH3="`pwd`/" #Alternative 2 (absolute path)
# cesta pro ukládání chybového výstupu studentského skriptu
LOG_PATH="./"


# test01: Cannot open the output file; Expected output: test01; Expected return code: 3
$INTERPRETER $TASK.$EXTENSION --output=/hopefully-no-write-permissions > ${LOCAL_OUT_PATH}test01.out 2> ${LOG_PATH}test01.err
echo -n $? > test01.!!!

# test02: Non-existent input file; Expected output: test02; Expected return code: 2
$INTERPRETER $TASK.$EXTENSION --input=/path/to/a/hopefully/nonexistent/file > ${LOCAL_OUT_PATH}test02.out 2> ${LOG_PATH}test02.err
echo -n $? > test02.!!!

# test03: Analysis of a trivial header file; Expected output: test03; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}basic/trivial.h --output=${LOCAL_OUT_PATH}test03.out 2> ${LOG_PATH}test03.err
echo -n $? > test03.!!!

# test04: Analysis of functions which are not declared as inline; Expected output: test04; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --output=${LOCAL_OUT_PATH2}test04.out --input=${LOCAL_IN_PATH2}basic/subdir/subsubdir/ --no-inline --pretty-xml=0 2> ${LOG_PATH}test04.err
echo -n $? > test04.!!!

# test05: Analysis of the whole current directory; Expected output: test05; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --pretty-xml > ${LOCAL_OUT_PATH}test05.out 2> ${LOG_PATH}test05.err
echo -n $? > test05.!!!

# test06: Analysis of functions with at most one parameter; Expected output: test06; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input="${LOCAL_IN_PATH3}basic/trivial.h" --output="${LOCAL_OUT_PATH3}test06.out" --max-par=1 --pretty-xml=3 2> ${LOG_PATH}test06.err
echo -n $? > test06.!!!

# test07: Analysis of a file where a function is declared more than once; Expected output: test07; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}basic/more.h --output=${LOCAL_OUT_PATH}test07.out --no-duplicates 2> ${LOG_PATH}test07.err
echo -n $? > test07.!!!

# test08: Reduction of the number of whitespaces in types; Expected output: test08; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --input=${LOCAL_IN_PATH}basic/subdir/subsubdir/whitespace.h --remove-whitespace --output="${LOCAL_OUT_PATH3}test08.out" 2> ${LOG_PATH}test08.err
echo -n $? > test08.!!!

# test09: Unknown program argument; Expected output: test09; Expected return code: 1
$INTERPRETER $TASK.$EXTENSION --unknown-argument > ${LOCAL_OUT_PATH}test09.out 2> ${LOG_PATH}test09.err
echo -n $? > test09.!!!

# test10: Display the program help; Expected output: test10; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --help > ${LOCAL_OUT_PATH}test10.out 2> ${LOG_PATH}test10.err
echo -n $? > test10.!!!

# test11: Analysis of functions with rather complex data types (the FUN extension); Expected output: test11; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --pretty-xml --output=${LOCAL_OUT_PATH2}test11.out --input="${LOCAL_IN_PATH3}ext/FUN.h" 2> ${LOG_PATH}test11.err
echo -n $? > test11.!!!

# test12: Analysis of functions with missing parameter names (the PAR extension); Expected output: test12; Expected return code: 0
$INTERPRETER $TASK.$EXTENSION --pretty-xml --output=${LOCAL_OUT_PATH}test12.out --input=${LOCAL_IN_PATH2}ext/PAR.h 2> ${LOG_PATH}test12.err
echo -n $? > test12.!!!

