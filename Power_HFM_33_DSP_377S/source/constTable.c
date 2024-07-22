
const int i16SineTable[]=
{
    0     ,
    59    ,
    118   ,
    177   ,
    235   ,
    294   ,
    353   ,
    411   ,
    470   ,
    528   ,
    586   ,
    644   ,
    702   ,
    759   ,
    816   ,
    874   ,
    930   ,
    987   ,
    1043  ,
    1099  ,
    1154  ,
    1209  ,
    1264  ,
    1319  ,
    1373  ,
    1426  ,
    1480  ,
    1532  ,
    1584  ,
    1636  ,
    1688  ,
    1738  ,
    1788  ,
    1838  ,
    1887  ,
    1936  ,
    1984  ,
    2031  ,
    2078  ,
    2124  ,
    2169  ,
    2214  ,
    2258  ,
    2302  ,
    2344  ,
    2386  ,
    2428  ,
    2468  ,
    2508  ,
    2547  ,
    2585  ,
    2623  ,
    2660  ,
    2695  ,
    2730  ,
    2765  ,
    2798  ,
    2831  ,
    2862  ,
    2893  ,
    2923  ,
    2952  ,
    2980  ,
    3007  ,
    3033  ,
    3059  ,
    3083  ,
    3107  ,
    3129  ,
    3151  ,
    3171  ,
    3191  ,
    3210  ,
    3228  ,
    3244  ,
    3260  ,
    3275  ,
    3288  ,
    3301  ,
    3313  ,
    3324  ,
    3333  ,
    3342  ,
    3350  ,
    3357  ,
    3362  ,
    3367  ,
    3370  ,
    3373  ,
    3374  ,
    3375  ,
    3374  ,
    3373  ,
    3370  ,
    3367  ,
    3362  ,
    3357  ,
    3350  ,
    3342  ,
    3333  ,
    3324  ,
    3313  ,
    3301  ,
    3288  ,
    3275  ,
    3260  ,
    3244  ,
    3228  ,
    3210  ,
    3191  ,
    3171  ,
    3151  ,
    3129  ,
    3107  ,
    3083  ,
    3059  ,
    3033  ,
    3007  ,
    2980  ,
    2952  ,
    2923  ,
    2893  ,
    2862  ,
    2831  ,
    2798  ,
    2765  ,
    2730  ,
    2695  ,
    2660  ,
    2623  ,
    2585  ,
    2547  ,
    2508  ,
    2468  ,
    2428  ,
    2386  ,
    2344  ,
    2302  ,
    2258  ,
    2214  ,
    2169  ,
    2124  ,
    2078  ,
    2031  ,
    1984  ,
    1936  ,
    1887  ,
    1838  ,
    1788  ,
    1738  ,
    1688  ,
    1636  ,
    1584  ,
    1532  ,
    1480  ,
    1426  ,
    1373  ,
    1319  ,
    1264  ,
    1209  ,
    1154  ,
    1099  ,
    1043  ,
    987   ,
    930   ,
    874   ,
    816   ,
    759   ,
    702   ,
    644   ,
    586   ,
    528   ,
    470   ,
    411   ,
    353   ,
    294   ,
    235   ,
    177   ,
    118   ,
    59    ,
    0     ,
    -59   ,
    -118  ,
    -177  ,
    -235  ,
    -294  ,
    -353  ,
    -411  ,
    -470  ,
    -528  ,
    -586  ,
    -644  ,
    -702  ,
    -759  ,
    -816  ,
    -874  ,
    -930  ,
    -987  ,
    -1043 ,
    -1099 ,
    -1154 ,
    -1209 ,
    -1264 ,
    -1319 ,
    -1373 ,
    -1426 ,
    -1480 ,
    -1532 ,
    -1584 ,
    -1636 ,
    -1688 ,
    -1738 ,
    -1788 ,
    -1838 ,
    -1887 ,
    -1936 ,
    -1984 ,
    -2031 ,
    -2078 ,
    -2124 ,
    -2169 ,
    -2214 ,
    -2258 ,
    -2302 ,
    -2344 ,
    -2386 ,
    -2428 ,
    -2468 ,
    -2508 ,
    -2547 ,
    -2585 ,
    -2623 ,
    -2660 ,
    -2695 ,
    -2730 ,
    -2765 ,
    -2798 ,
    -2831 ,
    -2862 ,
    -2893 ,
    -2923 ,
    -2952 ,
    -2980 ,
    -3007 ,
    -3033 ,
    -3059 ,
    -3083 ,
    -3107 ,
    -3129 ,
    -3151 ,
    -3171 ,
    -3191 ,
    -3210 ,
    -3228 ,
    -3244 ,
    -3260 ,
    -3275 ,
    -3288 ,
    -3301 ,
    -3313 ,
    -3324 ,
    -3333 ,
    -3342 ,
    -3350 ,
    -3357 ,
    -3362 ,
    -3367 ,
    -3370 ,
    -3373 ,
    -3374 ,
    -3375 ,
    -3374 ,
    -3373 ,
    -3370 ,
    -3367 ,
    -3362 ,
    -3357 ,
    -3350 ,
    -3342 ,
    -3333 ,
    -3324 ,
    -3313 ,
    -3301 ,
    -3288 ,
    -3275 ,
    -3260 ,
    -3244 ,
    -3228 ,
    -3210 ,
    -3191 ,
    -3171 ,
    -3151 ,
    -3129 ,
    -3107 ,
    -3083 ,
    -3059 ,
    -3033 ,
    -3007 ,
    -2980 ,
    -2952 ,
    -2923 ,
    -2893 ,
    -2862 ,
    -2831 ,
    -2798 ,
    -2765 ,
    -2730 ,
    -2695 ,
    -2660 ,
    -2623 ,
    -2585 ,
    -2547 ,
    -2508 ,
    -2468 ,
    -2428 ,
    -2386 ,
    -2344 ,
    -2302 ,
    -2258 ,
    -2214 ,
    -2169 ,
    -2124 ,
    -2078 ,
    -2031 ,
    -1984 ,
    -1936 ,
    -1887 ,
    -1838 ,
    -1788 ,
    -1738 ,
    -1688 ,
    -1636 ,
    -1584 ,
    -1532 ,
    -1480 ,
    -1426 ,
    -1373 ,
    -1319 ,
    -1264 ,
    -1209 ,
    -1154 ,
    -1099 ,
    -1043 ,
    -987  ,
    -930  ,
    -874  ,
    -816  ,
    -759  ,
    -702  ,
    -644  ,
    -586  ,
    -528  ,
    -470  ,
    -411  ,
    -353  ,
    -294  ,
    -235  ,
    -177  ,
    -118  ,
    -59   

};


