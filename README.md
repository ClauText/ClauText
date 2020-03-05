[![Codacy Badge](https://api.codacy.com/project/badge/Grade/836839ad06e340f2aaa42b32aad03c19)](https://app.codacy.com/app/vztpv/ClauText?utm_source=github.com&utm_medium=referral&utm_content=ClauText/ClauText&utm_campaign=Badge_Grade_Dashboard)
# ClauText [![Total alerts](https://img.shields.io/lgtm/alerts/g/ClauText/ClauText.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/ClauText/ClauText/alerts/)[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/ClauText/ClauText.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/ClauText/ClauText/context:cpp)[![Build status](https://ci.appveyor.com/api/projects/status/f3o54347ns0a3xda?svg=true)](https://ci.appveyor.com/project/vztpv/clautext)


  My Own Language for Editing? "Paradox Game(Clausewitz Engine) Data"
# Feature
  1. Parallel Load Data From File (cf JSON Parser)
  2. Linux is supported?, but some has bug, is limited(especially $edit_mode)
  3. fixed some bugs. (after? wiz/ClauText)
  4. speed up. (for replace function, ToBool4(in ClauText API?))
  5. Load Clausewitz Engine Data File, Edit Data, and Save Data.
  6. In Sequential Load Data Function, Line Comments are also saved! (but not exactly!)
  7. Shell Mode?
  8. Edit Mode
# Known Bug (but, maybe slow improvement?)?
  1. Fixed -  /./test/../test/../test <- is no ok, and .. is ok in just front? nested case. ( /./../../test )
  2. Load Data API? do not check error(syntax, and etc...) totally.?? 
  
# ToDo / Done
  1. REMOVE - Now Position for Data? Next Position for Data? Move Up? or Move Down? for fast linear search?
  2. DONE - "riterate and some condition => remove data" function?
  3. DONE - riterate ( iterate begin -> last, riterate last -> begin? ) ?
  4. DONE? - in iterate function, for example, $iterate = { dir = { /./eu4Game/provinces/$/history } events = { A B C } },
        A -> end or A -> B -> end or A -> B -> C -> end ?   if event returns FALSE (default TRUE?) -> stop events??
        before_value?, get ut ??
  5. DONE - Copy function!
  6. DOING - std::map -> wiz::ArrayMap
  7. DONE - makefile added!
  8. TODO - with LGTM, fix warnings. https://lgtm.com/projects/g/ClauText/ClauText/
  9. NOTYET - UTF-8 Support? (with C++20?)
  10. $iterate is very slow. - maybe need big change in code.. or use ClauToCpp
  11. Update Parallel Parser. - see https://github.com/ClauText/ClauParser
