      subroutine duStepWaveGen2d4rc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   src,
     *   dx,dy,dt,cc,beta,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer ndf4a,ndf4b,nComp
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real src  (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,0:*)
      real dx,dy,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        real t1001
        real t1003
        real t1005
        real t1007
        real t1009
        real t101
        real t1011
        real t1013
        real t1014
        real t1018
        real t1019
        real t1020
        real t1022
        real t1026
        real t1028
        real t103
        real t1032
        real t1039
        real t104
        real t1045
        real t1053
        real t1055
        real t1059
        real t1060
        real t1061
        real t107
        real t108
        real t1082
        integer t1083
        real t1085
        real t1089
        real t11
        real t110
        real t1103
        real t1111
        real t1114
        real t1118
        real t112
        real t1120
        real t1121
        real t1130
        real t1131
        real t1135
        real t1146
        real t1148
        real t1149
        real t115
        real t1151
        real t1153
        real t1154
        real t1155
        real t1157
        real t1161
        real t1164
        real t1166
        real t1167
        real t1171
        real t1173
        real t1175
        real t1177
        real t1179
        real t118
        real t1181
        real t1183
        real t1186
        real t1188
        real t1189
        real t119
        real t1191
        real t1192
        real t1194
        real t1196
        real t1199
        real t12
        real t1200
        real t1203
        real t1204
        real t1206
        real t1208
        real t121
        real t1211
        real t1213
        real t1215
        real t1217
        real t122
        real t1221
        real t1222
        real t1223
        real t1225
        real t1229
        real t1232
        real t1237
        real t1241
        real t1244
        real t1246
        real t125
        real t126
        real t128
        real t1283
        real t1286
        real t1288
        real t1291
        real t1293
        real t1297
        integer t13
        real t130
        real t131
        real t1339
        real t1344
        real t135
        real t1354
        real t1356
        real t1358
        real t1360
        real t1362
        real t1364
        real t1366
        real t1368
        real t1369
        real t1371
        real t1374
        real t1375
        real t1377
        real t1378
        real t1380
        real t1383
        real t1384
        real t1391
        real t1393
        real t1394
        real t1396
        real t1398
        real t14
        real t140
        real t1400
        real t1407
        real t1408
        real t1409
        real t1411
        real t1413
        real t1415
        real t1422
        real t1428
        real t143
        real t1430
        real t1434
        real t1440
        real t1441
        real t1448
        real t1449
        real t145
        real t1450
        real t1452
        real t1463
        integer t1464
        real t1466
        real t147
        real t1470
        real t1471
        real t1475
        real t1478
        real t148
        real t1480
        real t149
        integer t15
        real t151
        real t1510
        real t1512
        real t152
        real t1521
        real t1522
        real t1526
        real t153
        real t1541
        real t1543
        real t1544
        real t1546
        real t1548
        real t155
        real t1551
        real t1555
        real t1558
        real t1560
        real t157
        real t159
        real t1597
        real t16
        real t1600
        real t1602
        real t1605
        real t1607
        real t161
        real t1611
        real t162
        real t163
        real t165
        real t1653
        real t1666
        real t1668
        real t167
        real t1670
        real t1672
        real t1674
        real t1677
        real t1679
        real t168
        real t1689
        real t169
        real t1696
        real t17
        real t1702
        real t1706
        real t1708
        real t1709
        real t171
        real t1711
        real t1712
        real t172
        real t1721
        real t1723
        real t173
        real t1733
        real t1734
        real t1740
        real t1741
        real t1749
        real t175
        real t1750
        real t1756
        real t1757
        real t177
        real t179
        real t181
        real t183
        real t186
        real t187
        real t188
        real t189
        real t19
        real t191
        real t192
        real t194
        real t195
        real t197
        integer t198
        real t199
        real t2
        real t200
        real t201
        real t203
        real t204
        real t206
        real t209
        integer t21
        real t210
        real t212
        real t214
        real t215
        real t216
        real t22
        real t222
        integer t223
        real t224
        real t225
        real t226
        real t227
        real t229
        real t23
        real t230
        real t232
        real t233
        real t235
        integer t236
        real t237
        real t238
        real t239
        real t241
        real t242
        real t244
        real t248
        real t25
        real t250
        real t251
        real t252
        real t254
        real t256
        real t257
        real t258
        real t265
        real t267
        real t268
        real t269
        real t27
        real t270
        real t272
        real t273
        real t275
        real t276
        real t278
        real t279
        integer t28
        real t280
        real t281
        real t283
        real t284
        real t286
        real t289
        real t29
        real t290
        real t292
        real t294
        real t295
        real t296
        real t302
        real t303
        real t304
        real t305
        real t307
        real t308
        real t310
        real t311
        real t313
        real t314
        real t315
        real t316
        real t318
        real t319
        real t32
        real t321
        real t325
        real t327
        real t328
        real t329
        real t331
        real t333
        real t334
        real t335
        integer t34
        real t342
        real t344
        real t345
        real t346
        real t348
        real t349
        real t35
        real t350
        real t352
        real t354
        real t355
        real t357
        real t359
        real t362
        real t364
        real t365
        real t366
        real t367
        real t368
        real t369
        real t372
        real t375
        real t378
        real t380
        real t381
        real t383
        real t385
        real t388
        real t389
        real t391
        real t392
        real t394
        real t396
        real t399
        real t4
        real t404
        real t407
        integer t41
        real t410
        real t413
        real t414
        real t417
        real t423
        real t425
        real t426
        real t427
        real t429
        real t43
        real t430
        real t431
        real t433
        real t435
        real t438
        real t439
        real t442
        real t443
        real t445
        real t447
        real t45
        real t450
        real t452
        real t454
        real t456
        real t457
        real t458
        real t459
        real t462
        real t463
        real t464
        real t465
        real t466
        real t469
        real t471
        real t472
        integer t48
        real t488
        real t489
        real t491
        real t494
        real t496
        real t5
        real t500
        real t501
        real t513
        real t519
        real t528
        real t530
        real t531
        real t547
        real t548
        real t55
        real t550
        real t553
        real t555
        real t559
        real t560
        real t57
        real t583
        real t587
        real t589
        real t59
        real t592
        real t594
        real t595
        real t6
        real t60
        real t601
        real t605
        real t609
        real t61
        real t613
        real t624
        real t628
        real t63
        real t637
        real t639
        real t64
        real t641
        real t643
        real t644
        real t646
        real t648
        real t65
        real t650
        real t652
        real t654
        real t656
        real t657
        real t660
        real t67
        real t671
        real t675
        real t676
        real t677
        real t678
        real t680
        real t681
        real t683
        real t686
        real t688
        real t69
        real t692
        real t693
        real t694
        real t696
        real t697
        real t699
        real t7
        real t701
        real t702
        real t704
        real t706
        real t708
        real t709
        real t71
        real t711
        real t713
        real t715
        real t721
        real t728
        real t73
        real t734
        real t736
        real t74
        real t743
        real t744
        real t745
        real t746
        real t753
        real t755
        real t759
        real t760
        real t766
        real t77
        real t771
        integer t774
        real t776
        real t78
        real t788
        real t796
        real t797
        real t799
        real t8
        real t80
        real t802
        real t804
        real t808
        real t809
        real t821
        real t827
        integer t83
        real t836
        real t839
        real t84
        real t840
        real t85
        real t852
        real t860
        real t861
        real t863
        real t866
        real t868
        real t87
        real t872
        real t873
        real t89
        real t9
        real t90
        real t900
        real t903
        real t907
        real t91
        real t912
        real t913
        real t917
        real t920
        real t928
        real t93
        real t932
        real t936
        real t94
        real t940
        real t95
        real t951
        real t955
        real t964
        real t967
        real t97
        real t971
        real t99
        real t991
        real t995
        real t999
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = beta ** 2
        t5 = sqrt(0.3E1)
        t6 = t5 / 0.6E1
        t7 = 0.1E1 / 0.2E1 + t6
        t8 = t7 ** 2
        t9 = t4 * t8
        t10 = dt ** 2
        t11 = t10 * dx
        t12 = cc ** 2
        t13 = i + 3
        t14 = ut(t13,j,n)
        t15 = i + 2
        t16 = ut(t15,j,n)
        t17 = t14 - t16
        t19 = 0.1E1 / dx
        t21 = i + 1
        t22 = ut(t21,j,n)
        t23 = t16 - t22
        t25 = t12 * t23 * t19
        t27 = (t12 * t17 * t19 - t25) * t19
        t28 = j + 1
        t29 = ut(t15,t28,n)
        t32 = 0.1E1 / dy
        t34 = j - 1
        t35 = ut(t15,t34,n)
        t41 = n + 1
        t43 = src(t15,j,nComp,n)
        t45 = 0.1E1 / dt
        t48 = n - 1
        t55 = t22 - t2
        t57 = t12 * t55 * t19
        t59 = (t25 - t57) * t19
        t60 = ut(t21,t28,n)
        t61 = t60 - t22
        t63 = t12 * t61 * t32
        t64 = ut(t21,t34,n)
        t65 = t22 - t64
        t67 = t12 * t65 * t32
        t69 = (t63 - t67) * t32
        t71 = src(t21,j,nComp,n)
        t73 = (src(t21,j,nComp,t41) - t71) * t45
        t74 = t73 / 0.2E1
        t77 = (t71 - src(t21,j,nComp,t48)) * t45
        t78 = t77 / 0.2E1
        t80 = cc * (t59 + t69 + t74 + t78)
        t83 = i - 1
        t84 = ut(t83,j,n)
        t85 = t2 - t84
        t87 = t12 * t85 * t19
        t89 = (t57 - t87) * t19
        t90 = ut(i,t28,n)
        t91 = t90 - t2
        t93 = t12 * t91 * t32
        t94 = ut(i,t34,n)
        t95 = t2 - t94
        t97 = t12 * t95 * t32
        t99 = (t93 - t97) * t32
        t101 = src(i,j,nComp,n)
        t103 = (src(i,j,nComp,t41) - t101) * t45
        t104 = t103 / 0.2E1
        t107 = (t101 - src(i,j,nComp,t48)) * t45
        t108 = t107 / 0.2E1
        t110 = cc * (t89 + t99 + t104 + t108)
        t112 = (t80 - t110) * t19
        t115 = t11 * ((cc * (t27 + (t12 * (t29 - t16) * t32 - t12 * (t16
     # - t35) * t32) * t32 + (src(t15,j,nComp,t41) - t43) * t45 / 0.2E1 
     #+ (t43 - src(t15,j,nComp,t48)) * t45 / 0.2E1) - t80) * t19 / 0.2E1
     # + t112 / 0.2E1)
        t118 = beta * t7
        t119 = dt * dx
        t121 = u(t15,j,n)
        t122 = u(t13,j,n) - t121
        t125 = u(t21,j,n)
        t126 = t121 - t125
        t128 = t12 * t126 * t19
        t130 = (t12 * t122 * t19 - t128) * t19
        t131 = u(t15,t28,n)
        t135 = u(t15,t34,n)
        t140 = (t12 * (t131 - t121) * t32 - t12 * (t121 - t135) * t32) *
     # t32
        t143 = t125 - t1
        t145 = t12 * t143 * t19
        t147 = (t128 - t145) * t19
        t148 = u(t21,t28,n)
        t149 = t148 - t125
        t151 = t12 * t149 * t32
        t152 = u(t21,t34,n)
        t153 = t125 - t152
        t155 = t12 * t153 * t32
        t157 = (t151 - t155) * t32
        t159 = cc * (t147 + t157 + t71)
        t161 = (cc * (t130 + t140 + t43) - t159) * t19
        t162 = u(t83,j,n)
        t163 = t1 - t162
        t165 = t12 * t163 * t19
        t167 = (t145 - t165) * t19
        t168 = u(i,t28,n)
        t169 = t168 - t1
        t171 = t12 * t169 * t32
        t172 = u(i,t34,n)
        t173 = t1 - t172
        t175 = t12 * t173 * t32
        t177 = (t171 - t175) * t32
        t179 = cc * (t167 + t177 + t101)
        t181 = (t159 - t179) * t19
        t183 = t119 * (t161 - t181)
        t186 = dt * cc
        t187 = dx ** 2
        t188 = t126 * t19
        t189 = t143 * t19
        t191 = (t188 - t189) * t19
        t192 = t163 * t19
        t194 = (t189 - t192) * t19
        t195 = t191 - t194
        t197 = t12 * t195 * t19
        t198 = i - 2
        t199 = u(t198,j,n)
        t200 = t162 - t199
        t201 = t200 * t19
        t203 = (t192 - t201) * t19
        t204 = t194 - t203
        t206 = t12 * t204 * t19
        t209 = t147 - t167
        t210 = t209 * t19
        t212 = t12 * t200 * t19
        t214 = (t165 - t212) * t19
        t215 = t167 - t214
        t216 = t215 * t19
        t222 = dy ** 2
        t223 = j + 2
        t224 = u(i,t223,n)
        t225 = t224 - t168
        t226 = t225 * t32
        t227 = t169 * t32
        t229 = (t226 - t227) * t32
        t230 = t173 * t32
        t232 = (t227 - t230) * t32
        t233 = t229 - t232
        t235 = t12 * t233 * t32
        t236 = j - 2
        t237 = u(i,t236,n)
        t238 = t172 - t237
        t239 = t238 * t32
        t241 = (t230 - t239) * t32
        t242 = t232 - t241
        t244 = t12 * t242 * t32
        t248 = t12 * t225 * t32
        t250 = (t248 - t171) * t32
        t251 = t250 - t177
        t252 = t251 * t32
        t254 = t12 * t238 * t32
        t256 = (t175 - t254) * t32
        t257 = t177 - t256
        t258 = t257 * t32
        t265 = t186 * (t167 - t187 * ((t197 - t206) * t19 + (t210 - t216
     #) * t19) / 0.24E2 + t177 - t222 * ((t235 - t244) * t32 + (t252 - t
     #258) * t32) / 0.24E2 + t101)
        t267 = t118 * t265 / 0.2E1
        t268 = t10 * cc
        t269 = t23 * t19
        t270 = t55 * t19
        t272 = (t269 - t270) * t19
        t273 = t85 * t19
        t275 = (t270 - t273) * t19
        t276 = t272 - t275
        t278 = t12 * t276 * t19
        t279 = ut(t198,j,n)
        t280 = t84 - t279
        t281 = t280 * t19
        t283 = (t273 - t281) * t19
        t284 = t275 - t283
        t286 = t12 * t284 * t19
        t289 = t59 - t89
        t290 = t289 * t19
        t292 = t12 * t280 * t19
        t294 = (t87 - t292) * t19
        t295 = t89 - t294
        t296 = t295 * t19
        t302 = ut(i,t223,n)
        t303 = t302 - t90
        t304 = t303 * t32
        t305 = t91 * t32
        t307 = (t304 - t305) * t32
        t308 = t95 * t32
        t310 = (t305 - t308) * t32
        t311 = t307 - t310
        t313 = t12 * t311 * t32
        t314 = ut(i,t236,n)
        t315 = t94 - t314
        t316 = t315 * t32
        t318 = (t308 - t316) * t32
        t319 = t310 - t318
        t321 = t12 * t319 * t32
        t325 = t12 * t303 * t32
        t327 = (t325 - t93) * t32
        t328 = t327 - t99
        t329 = t328 * t32
        t331 = t12 * t315 * t32
        t333 = (t97 - t331) * t32
        t334 = t99 - t333
        t335 = t334 * t32
        t342 = t268 * (t89 - t187 * ((t278 - t286) * t19 + (t290 - t296)
     # * t19) / 0.24E2 + t99 - t222 * ((t313 - t321) * t32 + (t329 - t33
     #5) * t32) / 0.24E2 + t104 + t108)
        t344 = t9 * t342 / 0.4E1
        t345 = u(t83,t28,n)
        t346 = t345 - t162
        t348 = t12 * t346 * t32
        t349 = u(t83,t34,n)
        t350 = t162 - t349
        t352 = t12 * t350 * t32
        t354 = (t348 - t352) * t32
        t355 = src(t83,j,nComp,n)
        t357 = cc * (t214 + t354 + t355)
        t359 = (t179 - t357) * t19
        t362 = t119 * (t181 / 0.2E1 + t359 / 0.2E1)
        t364 = t118 * t362 / 0.4E1
        t365 = t4 * beta
        t366 = t8 * t7
        t367 = t365 * t366
        t368 = t10 * dt
        t369 = t368 * cc
        t372 = t12 * (t147 + t157 - t167 - t177) * t19
        t375 = t12 * (t167 + t177 - t214 - t354) * t19
        t378 = t148 - t168
        t380 = t12 * t378 * t19
        t381 = t168 - t345
        t383 = t12 * t381 * t19
        t385 = (t380 - t383) * t19
        t388 = t12 * (t385 + t250 - t167 - t177) * t32
        t389 = t152 - t172
        t391 = t12 * t389 * t19
        t392 = t172 - t349
        t394 = t12 * t392 * t19
        t396 = (t391 - t394) * t19
        t399 = t12 * (t167 + t177 - t396 - t256) * t32
        t404 = t12 * (t71 - t101) * t19
        t407 = t12 * (t101 - t355) * t19
        t410 = src(i,t28,nComp,n)
        t413 = t12 * (t410 - t101) * t32
        t414 = src(i,t34,nComp,n)
        t417 = t12 * (t101 - t414) * t32
        t423 = t369 * ((t372 - t375) * t19 + (t388 - t399) * t32 + (t404
     # - t407) * t19 + (t413 - t417) * t32 + (t103 - t107) * t45)
        t425 = t367 * t423 / 0.12E2
        t426 = ut(t83,t28,n)
        t427 = t426 - t84
        t429 = t12 * t427 * t32
        t430 = ut(t83,t34,n)
        t431 = t84 - t430
        t433 = t12 * t431 * t32
        t435 = (t429 - t433) * t32
        t438 = (src(t83,j,nComp,t41) - t355) * t45
        t439 = t438 / 0.2E1
        t442 = (t355 - src(t83,j,nComp,t48)) * t45
        t443 = t442 / 0.2E1
        t445 = cc * (t294 + t435 + t439 + t443)
        t447 = (t110 - t445) * t19
        t450 = t11 * (t112 / 0.2E1 + t447 / 0.2E1)
        t452 = t9 * t450 / 0.8E1
        t454 = t119 * (t181 - t359)
        t456 = t118 * t454 / 0.24E2
        t457 = 0.1E1 / 0.2E1 - t6
        t458 = t457 ** 2
        t459 = t12 * t458
        t462 = t10 * (t147 + t157 + t71 - t167 - t177 - t101) * t19
        t464 = t459 * t462 / 0.2E1
        t465 = t458 * t457
        t466 = t12 * t465
        t469 = t368 * (t59 + t69 + t74 + t78 - t89 - t99 - t104 - t108) 
     #* t19
        t471 = t466 * t469 / 0.6E1
        t472 = beta * t457
        t488 = u(t21,t223,n)
        t489 = t488 - t148
        t491 = t149 * t32
        t494 = t153 * t32
        t496 = (t491 - t494) * t32
        t500 = u(t21,t236,n)
        t501 = t152 - t500
        t463 = t12 * t32
        t513 = (t463 * t489 - t151) * t32
        t519 = (-t463 * t501 + t155) * t32
        t528 = t186 * (t147 - t187 * ((t12 * ((t122 * t19 - t188) * t19 
     #- t191) * t19 - t197) * t19 + ((t130 - t147) * t19 - t210) * t19) 
     #/ 0.24E2 + t157 - t222 * ((t12 * ((t32 * t489 - t491) * t32 - t496
     #) * t32 - t12 * (t496 - (-t32 * t501 + t494) * t32) * t32) * t32 +
     # ((t513 - t157) * t32 - (t157 - t519) * t32) * t32) / 0.24E2 + t71
     #)
        t530 = t472 * t528 / 0.2E1
        t531 = t4 * t458
        t547 = ut(t21,t223,n)
        t548 = t547 - t60
        t550 = t61 * t32
        t553 = t65 * t32
        t555 = (t550 - t553) * t32
        t559 = ut(t21,t236,n)
        t560 = t64 - t559
        t587 = t268 * (t59 - t187 * ((t12 * ((t17 * t19 - t269) * t19 - 
     #t272) * t19 - t278) * t19 + ((t27 - t59) * t19 - t290) * t19) / 0.
     #24E2 + t69 - t222 * ((t12 * ((t32 * t548 - t550) * t32 - t555) * t
     #32 - t12 * (t555 - (-t32 * t560 + t553) * t32) * t32) * t32 + (((t
     #463 * t548 - t63) * t32 - t69) * t32 - (t69 - (-t463 * t560 + t67)
     # * t32) * t32) * t32) / 0.24E2 + t74 + t78)
        t589 = t531 * t587 / 0.4E1
        t592 = t119 * (t161 / 0.2E1 + t181 / 0.2E1)
        t594 = t472 * t592 / 0.4E1
        t595 = t365 * t465
        t601 = t131 - t148
        t583 = t12 * t19
        t605 = (t583 * t601 - t380) * t19
        t609 = t135 - t152
        t613 = (t583 * t609 - t391) * t19
        t624 = src(t21,t28,nComp,n)
        t628 = src(t21,t34,nComp,n)
        t637 = t369 * ((t12 * (t130 + t140 - t147 - t157) * t19 - t372) 
     #* t19 + (t12 * (t605 + t513 - t147 - t157) * t32 - t12 * (t147 + t
     #157 - t613 - t519) * t32) * t32 + (t12 * (t43 - t71) * t19 - t404)
     # * t19 + (t12 * (t624 - t71) * t32 - t12 * (t71 - t628) * t32) * t
     #32 + (t73 - t77) * t45)
        t639 = t595 * t637 / 0.12E2
        t641 = t531 * t115 / 0.8E1
        t643 = t472 * t183 / 0.24E2
        t644 = -t9 * t115 / 0.8E1 + t118 * t183 / 0.24E2 - t267 - t344 -
     # t364 - t425 - t452 - t456 - t464 - t471 - t530 - t589 + t594 - t6
     #39 + t641 - t643
        t646 = t472 * t265 / 0.2E1
        t648 = t531 * t342 / 0.4E1
        t650 = t472 * t362 / 0.4E1
        t652 = t595 * t423 / 0.12E2
        t654 = t531 * t450 / 0.8E1
        t656 = t472 * t454 / 0.24E2
        t657 = t12 * t8
        t660 = t12 * t366
        t671 = t12 * t457
        t675 = dt * (t270 - dx * t276 / 0.24E2)
        t676 = t671 * t675
        t677 = t457 * dt
        t678 = dx * t289
        t680 = t677 * t678 / 0.24E2
        t681 = t12 * t7
        t683 = t7 * dt
        t686 = t646 + t648 + t650 + t652 + t654 + t656 + t657 * t462 / 0
     #.2E1 + t660 * t469 / 0.6E1 + t118 * t528 / 0.2E1 + t9 * t587 / 0.4
     #E1 - t118 * t592 / 0.4E1 + t367 * t637 / 0.12E2 - t676 + t680 + t6
     #81 * t675 - t683 * t678 / 0.24E2
        t688 = (t644 + t686) * t5
        t692 = -t457 * t688 + t464 + t471 + t530 + t589 - t594 + t639 - 
     #t641 + t643 - t646 - t648 - t650
        t693 = cc * t2
        t694 = cc * t22
        t696 = (-t693 + t694) * t19
        t697 = cc * t84
        t699 = (t693 - t697) * t19
        t701 = (t696 - t699) * t19
        t702 = cc * t16
        t704 = (-t694 + t702) * t19
        t706 = (t704 - t696) * t19
        t708 = (t706 - t701) * t19
        t709 = cc * t279
        t711 = (-t709 + t697) * t19
        t713 = (t699 - t711) * t19
        t715 = (t701 - t713) * t19
        t721 = t187 * (t701 - dx * (t708 - t715) / 0.12E2) / 0.24E2
        t728 = (((cc * t14 - t702) * t19 - t704) * t19 - t706) * t19
        t734 = t187 * (t706 - dx * (t728 - t708) / 0.12E2) / 0.24E2
        t736 = t696 / 0.2E1
        t743 = dx * (t704 / 0.2E1 + t736 - t187 * (t728 / 0.2E1 + t708 /
     # 0.2E1) / 0.6E1) / 0.4E1
        t744 = t694 / 0.2E1
        t745 = t693 / 0.2E1
        t746 = t699 / 0.2E1
        t753 = dx * (t736 + t746 - t187 * (t708 / 0.2E1 + t715 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t755 = dx * t209 / 0.24E2
        t759 = t12 * (t189 - dx * t195 / 0.24E2)
        t760 = -t652 - t654 - t656 + t676 - t680 - t721 + t734 - t743 + 
     #t744 - t745 - t753 - t755 + t759
        t766 = t10 * (t167 + t177 + t101 - t214 - t354 - t355) * t19
        t771 = t368 * (t89 + t99 + t104 + t108 - t294 - t435 - t439 - t4
     #43) * t19
        t774 = i - 3
        t776 = t199 - u(t774,j,n)
        t788 = (-t583 * t776 + t212) * t19
        t796 = u(t83,t223,n)
        t797 = t796 - t345
        t799 = t346 * t32
        t802 = t350 * t32
        t804 = (t799 - t802) * t32
        t808 = u(t83,t236,n)
        t809 = t349 - t808
        t821 = (t463 * t797 - t348) * t32
        t827 = (-t463 * t809 + t352) * t32
        t836 = t186 * (t214 - t187 * ((t206 - t12 * (t203 - (-t19 * t776
     # + t201) * t19) * t19) * t19 + (t216 - (t214 - t788) * t19) * t19)
     # / 0.24E2 + t354 - t222 * ((t12 * ((t32 * t797 - t799) * t32 - t80
     #4) * t32 - t12 * (t804 - (-t32 * t809 + t802) * t32) * t32) * t32 
     #+ ((t821 - t354) * t32 - (t354 - t827) * t32) * t32) / 0.24E2 + t3
     #55)
        t839 = ut(t774,j,n)
        t840 = t279 - t839
        t852 = (-t583 * t840 + t292) * t19
        t860 = ut(t83,t223,n)
        t861 = t860 - t426
        t863 = t427 * t32
        t866 = t431 * t32
        t868 = (t863 - t866) * t32
        t872 = ut(t83,t236,n)
        t873 = t430 - t872
        t900 = t268 * (t294 - t187 * ((t286 - t12 * (t283 - (-t19 * t840
     # + t281) * t19) * t19) * t19 + (t296 - (t294 - t852) * t19) * t19)
     # / 0.24E2 + t435 - t222 * ((t12 * ((t32 * t861 - t863) * t32 - t86
     #8) * t32 - t12 * (t868 - (-t32 * t873 + t866) * t32) * t32) * t32 
     #+ (((t463 * t861 - t429) * t32 - t435) * t32 - (t435 - (-t463 * t8
     #73 + t433) * t32) * t32) * t32) / 0.24E2 + t439 + t443)
        t903 = u(t198,t28,n)
        t907 = u(t198,t34,n)
        t912 = (t12 * (t903 - t199) * t32 - t12 * (t199 - t907) * t32) *
     # t32
        t913 = src(t198,j,nComp,n)
        t917 = (t357 - cc * (t788 + t912 + t913)) * t19
        t920 = t119 * (t359 / 0.2E1 + t917 / 0.2E1)
        t928 = t345 - t903
        t932 = (-t583 * t928 + t383) * t19
        t936 = t349 - t907
        t940 = (-t583 * t936 + t394) * t19
        t951 = src(t83,t28,nComp,n)
        t955 = src(t83,t34,nComp,n)
        t964 = t369 * ((t375 - t12 * (t214 + t354 - t788 - t912) * t19) 
     #* t19 + (t12 * (t932 + t821 - t214 - t354) * t32 - t12 * (t214 + t
     #354 - t940 - t827) * t32) * t32 + (t407 - t12 * (t355 - t913) * t1
     #9) * t19 + (t12 * (t951 - t355) * t32 - t12 * (t355 - t955) * t32)
     # * t32 + (t438 - t442) * t45)
        t967 = ut(t198,t28,n)
        t971 = ut(t198,t34,n)
        t991 = t11 * (t447 / 0.2E1 + (t445 - cc * (t852 + (t12 * (t967 -
     # t279) * t32 - t12 * (t279 - t971) * t32) * t32 + (src(t198,j,nCom
     #p,t41) - t913) * t45 / 0.2E1 + (t913 - src(t198,j,nComp,t48)) * t4
     #5 / 0.2E1)) * t19 / 0.2E1)
        t995 = t119 * (t359 - t917)
        t999 = t459 * t766 / 0.2E1
        t1001 = t466 * t771 / 0.6E1
        t1003 = t472 * t836 / 0.2E1
        t1005 = t531 * t900 / 0.4E1
        t1007 = t472 * t920 / 0.4E1
        t1009 = t595 * t964 / 0.12E2
        t1011 = t531 * t991 / 0.8E1
        t1013 = t472 * t995 / 0.24E2
        t1014 = t657 * t766 / 0.2E1 + t660 * t771 / 0.6E1 - t118 * t836 
     #/ 0.2E1 - t9 * t900 / 0.4E1 - t118 * t920 / 0.4E1 - t367 * t964 / 
     #0.12E2 - t9 * t991 / 0.8E1 - t118 * t995 / 0.24E2 - t999 - t1001 +
     # t1003 + t1005 + t1007 + t1009 + t1011 + t1013
        t1018 = dt * (t273 - dx * t284 / 0.24E2)
        t1019 = t671 * t1018
        t1020 = dx * t295
        t1022 = t677 * t1020 / 0.24E2
        t1026 = t267 + t344 - t364 + t425 - t452 + t456 - t646 - t648 + 
     #t650 - t652 + t654 - t656 - t1019 + t1022 + t681 * t1018 - t683 * 
     #t1020 / 0.24E2
        t1028 = (t1014 + t1026) * t5
        t1032 = -t1028 * t457 + t1001 - t1003 - t1005 - t1007 - t1009 - 
     #t1011 - t1013 + t646 + t648 - t650 + t999
        t1039 = (t713 - (t711 - (-cc * t839 + t709) * t19) * t19) * t19
        t1045 = t187 * (t713 - dx * (t715 - t1039) / 0.12E2) / 0.24E2
        t1053 = dx * (t746 + t711 / 0.2E1 - t187 * (t715 / 0.2E1 + t1039
     # / 0.2E1) / 0.6E1) / 0.4E1
        t1055 = dx * t215 / 0.24E2
        t1059 = t12 * (t192 - dx * t204 / 0.24E2)
        t1060 = t697 / 0.2E1
        t1061 = t652 - t654 + t656 + t721 - t1045 - t1053 - t1055 + t105
     #9 - t1060 + t1019 - t1022 + t745 - t753
        t1082 = (t12 * (t488 - t224) * t19 - t12 * (t224 - t796) * t19) 
     #* t19
        t1083 = j + 3
        t1085 = u(i,t1083,n) - t224
        t1089 = (t1085 * t12 * t32 - t248) * t32
        t1103 = src(i,t223,nComp,n)
        t1111 = (src(i,t28,nComp,t41) - t410) * t45
        t1114 = (t410 - src(i,t28,nComp,t48)) * t45
        t1118 = t369 * ((t12 * (t605 + t513 - t385 - t250) * t19 - t12 *
     # (t385 + t250 - t932 - t821) * t19) * t19 + (t12 * (t1082 + t1089 
     #- t385 - t250) * t32 - t388) * t32 + (t12 * (t624 - t410) * t19 - 
     #t12 * (t410 - t951) * t19) * t19 + (t12 * (t1103 - t410) * t32 - t
     #413) * t32 + (t1111 - t1114) * t45)
        t1120 = t595 * t1118 / 0.12E2
        t1121 = t10 * dy
        t1130 = ut(i,t1083,n)
        t1131 = t1130 - t302
        t1135 = (t1131 * t12 * t32 - t325) * t32
        t1146 = t60 - t90
        t1148 = t12 * t1146 * t19
        t1149 = t90 - t426
        t1151 = t12 * t1149 * t19
        t1153 = (t1148 - t1151) * t19
        t1154 = t1111 / 0.2E1
        t1155 = t1114 / 0.2E1
        t1157 = cc * (t1153 + t327 + t1154 + t1155)
        t1161 = (t1157 - t110) * t32
        t1164 = t1121 * ((cc * ((t12 * (t547 - t302) * t19 - t12 * (t302
     # - t860) * t19) * t19 + t1135 + (src(i,t223,nComp,t41) - t1103) * 
     #t45 / 0.2E1 + (t1103 - src(i,t223,nComp,t48)) * t45 / 0.2E1) - t11
     #57) * t32 / 0.2E1 + t1161 / 0.2E1)
        t1166 = t531 * t1164 / 0.8E1
        t1167 = dt * dy
        t1171 = cc * (t385 + t250 + t410)
        t1173 = (cc * (t1082 + t1089 + t1103) - t1171) * t32
        t1175 = (t1171 - t179) * t32
        t1177 = t1167 * (t1173 - t1175)
        t1179 = t472 * t1177 / 0.24E2
        t1181 = cc * (t396 + t256 + t414)
        t1183 = (t179 - t1181) * t32
        t1186 = t1167 * (t1175 / 0.2E1 + t1183 / 0.2E1)
        t1188 = t472 * t1186 / 0.4E1
        t1189 = t64 - t94
        t1191 = t12 * t1189 * t19
        t1192 = t94 - t430
        t1194 = t12 * t1192 * t19
        t1196 = (t1191 - t1194) * t19
        t1199 = (src(i,t34,nComp,t41) - t414) * t45
        t1200 = t1199 / 0.2E1
        t1203 = (t414 - src(i,t34,nComp,t48)) * t45
        t1204 = t1203 / 0.2E1
        t1206 = cc * (t1196 + t333 + t1200 + t1204)
        t1208 = (t110 - t1206) * t32
        t1211 = t1121 * (t1161 / 0.2E1 + t1208 / 0.2E1)
        t1213 = t531 * t1211 / 0.8E1
        t1215 = t1167 * (t1175 - t1183)
        t1217 = t472 * t1215 / 0.24E2
        t1221 = dt * (t305 - dy * t311 / 0.24E2)
        t1222 = t671 * t1221
        t1223 = dy * t328
        t1225 = t677 * t1223 / 0.24E2
        t1229 = -t1120 + t1166 - t1179 + t1188 + t1213 + t1217 - t1222 +
     # t1225 + t681 * t1221 - t683 * t1223 / 0.24E2 - t267 - t344 - t425
     # + t646 + t648 + t652
        t1232 = t10 * (t385 + t250 + t410 - t167 - t177 - t101) * t32
        t1237 = t368 * (t1153 + t327 + t1154 + t1155 - t89 - t99 - t104 
     #- t108) * t32
        t1241 = t378 * t19
        t1244 = t381 * t19
        t1246 = (t1241 - t1244) * t19
        t1283 = t186 * (t385 - t187 * ((t12 * ((t19 * t601 - t1241) * t1
     #9 - t1246) * t19 - t12 * (t1246 - (-t19 * t928 + t1244) * t19) * t
     #19) * t19 + ((t605 - t385) * t19 - (t385 - t932) * t19) * t19) / 0
     #.24E2 + t250 - t222 * ((t12 * ((t1085 * t32 - t226) * t32 - t229) 
     #* t32 - t235) * t32 + ((t1089 - t250) * t32 - t252) * t32) / 0.24E
     #2 + t410)
        t1286 = t29 - t60
        t1288 = t1146 * t19
        t1291 = t1149 * t19
        t1293 = (t1288 - t1291) * t19
        t1297 = t426 - t967
        t1339 = t268 * (t1153 - t187 * ((t12 * ((t1286 * t19 - t1288) * 
     #t19 - t1293) * t19 - t12 * (t1293 - (-t1297 * t19 + t1291) * t19) 
     #* t19) * t19 + (((t12 * t1286 * t19 - t1148) * t19 - t1153) * t19 
     #- (t1153 - (-t12 * t1297 * t19 + t1151) * t19) * t19) * t19) / 0.2
     #4E2 + t327 - t222 * ((t12 * ((t1131 * t32 - t304) * t32 - t307) * 
     #t32 - t313) * t32 + ((t1135 - t327) * t32 - t329) * t32) / 0.24E2 
     #+ t1154 + t1155)
        t1344 = t1167 * (t1173 / 0.2E1 + t1175 / 0.2E1)
        t1354 = t118 * t1186 / 0.4E1
        t1356 = t9 * t1211 / 0.8E1
        t1358 = t118 * t1215 / 0.24E2
        t1360 = t459 * t1232 / 0.2E1
        t1362 = t466 * t1237 / 0.6E1
        t1364 = t472 * t1283 / 0.2E1
        t1366 = t531 * t1339 / 0.4E1
        t1368 = t472 * t1344 / 0.4E1
        t1369 = t657 * t1232 / 0.2E1 + t660 * t1237 / 0.6E1 + t118 * t12
     #83 / 0.2E1 + t9 * t1339 / 0.4E1 - t118 * t1344 / 0.4E1 + t367 * t1
     #118 / 0.12E2 - t9 * t1164 / 0.8E1 + t118 * t1177 / 0.24E2 - t1354 
     #- t1356 - t1358 - t1360 - t1362 - t1364 - t1366 + t1368
        t1371 = (t1229 + t1369) * t5
        t1374 = cc * t90
        t1375 = t1374 / 0.2E1
        t1377 = -t1371 * t457 + t1120 - t1166 + t1179 - t1188 - t1213 - 
     #t1217 + t1222 - t1225 + t1375 - t646 - t648
        t1378 = cc * t302
        t1380 = (-t1374 + t1378) * t32
        t1383 = (-t693 + t1374) * t32
        t1384 = t1383 / 0.2E1
        t1391 = (t1380 - t1383) * t32
        t1393 = (((cc * t1130 - t1378) * t32 - t1380) * t32 - t1391) * t
     #32
        t1394 = cc * t94
        t1396 = (t693 - t1394) * t32
        t1398 = (t1383 - t1396) * t32
        t1400 = (t1391 - t1398) * t32
        t1407 = dy * (t1380 / 0.2E1 + t1384 - t222 * (t1393 / 0.2E1 + t1
     #400 / 0.2E1) / 0.6E1) / 0.4E1
        t1408 = t1396 / 0.2E1
        t1409 = cc * t314
        t1411 = (-t1409 + t1394) * t32
        t1413 = (t1396 - t1411) * t32
        t1415 = (t1398 - t1413) * t32
        t1422 = dy * (t1384 + t1408 - t222 * (t1400 / 0.2E1 + t1415 / 0.
     #2E1) / 0.6E1) / 0.4E1
        t1428 = t222 * (t1398 - dy * (t1400 - t1415) / 0.12E2) / 0.24E2
        t1430 = dy * t251 / 0.24E2
        t1434 = t12 * (t227 - dy * t233 / 0.24E2)
        t1440 = t222 * (t1391 - dy * (t1393 - t1400) / 0.12E2) / 0.24E2
        t1441 = -t652 - t1407 - t1422 - t1428 - t1430 + t1434 + t1440 - 
     #t745 + t1360 + t1362 + t1364 + t1366 - t1368
        t1448 = dt * (t308 - dy * t319 / 0.24E2)
        t1449 = t671 * t1448
        t1450 = dy * t334
        t1452 = t677 * t1450 / 0.24E2
        t1463 = (t12 * (t500 - t237) * t19 - t12 * (t237 - t808) * t19) 
     #* t19
        t1464 = j - 3
        t1466 = t237 - u(i,t1464,n)
        t1470 = (-t12 * t1466 * t32 + t254) * t32
        t1471 = src(i,t236,nComp,n)
        t1475 = (t1181 - cc * (t1463 + t1470 + t1471)) * t32
        t1478 = t1167 * (t1183 / 0.2E1 + t1475 / 0.2E1)
        t1480 = t472 * t1478 / 0.4E1
        t1510 = t369 * ((t12 * (t613 + t519 - t396 - t256) * t19 - t12 *
     # (t396 + t256 - t940 - t827) * t19) * t19 + (t399 - t12 * (t396 + 
     #t256 - t1463 - t1470) * t32) * t32 + (t12 * (t628 - t414) * t19 - 
     #t12 * (t414 - t955) * t19) * t19 + (t417 - t12 * (t414 - t1471) * 
     #t32) * t32 + (t1199 - t1203) * t45)
        t1512 = t595 * t1510 / 0.12E2
        t1521 = ut(i,t1464,n)
        t1522 = t314 - t1521
        t1526 = (-t12 * t1522 * t32 + t331) * t32
        t1541 = t1121 * (t1208 / 0.2E1 + (t1206 - cc * ((t12 * (t559 - t
     #314) * t19 - t12 * (t314 - t872) * t19) * t19 + t1526 + (src(i,t23
     #6,nComp,t41) - t1471) * t45 / 0.2E1 + (t1471 - src(i,t236,nComp,t4
     #8)) * t45 / 0.2E1)) * t32 / 0.2E1)
        t1543 = t531 * t1541 / 0.8E1
        t1544 = t1188 + t1213 - t1217 - t1449 + t1452 + t681 * t1448 - t
     #683 * t1450 / 0.24E2 + t267 + t344 + t425 - t646 - t648 - t652 + t
     #1480 + t1512 + t1543
        t1546 = t1167 * (t1183 - t1475)
        t1548 = t472 * t1546 / 0.24E2
        t1551 = t368 * (t89 + t99 + t104 + t108 - t1196 - t333 - t1200 -
     # t1204) * t32
        t1555 = t389 * t19
        t1558 = t392 * t19
        t1560 = (t1555 - t1558) * t19
        t1597 = t186 * (t396 - t187 * ((t12 * ((t19 * t609 - t1555) * t1
     #9 - t1560) * t19 - t12 * (t1560 - (-t19 * t936 + t1558) * t19) * t
     #19) * t19 + ((t613 - t396) * t19 - (t396 - t940) * t19) * t19) / 0
     #.24E2 + t256 - t222 * ((t244 - t12 * (t241 - (-t1466 * t32 + t239)
     # * t32) * t32) * t32 + (t258 - (t256 - t1470) * t32) * t32) / 0.24
     #E2 + t414)
        t1600 = t35 - t64
        t1602 = t1189 * t19
        t1605 = t1192 * t19
        t1607 = (t1602 - t1605) * t19
        t1611 = t430 - t971
        t1653 = t268 * (t1196 - t187 * ((t12 * ((t1600 * t19 - t1602) * 
     #t19 - t1607) * t19 - t12 * (t1607 - (-t1611 * t19 + t1605) * t19) 
     #* t19) * t19 + (((t12 * t1600 * t19 - t1191) * t19 - t1196) * t19 
     #- (t1196 - (-t12 * t1611 * t19 + t1194) * t19) * t19) * t19) / 0.2
     #4E2 + t333 - t222 * ((t321 - t12 * (t318 - (-t1522 * t32 + t316) *
     # t32) * t32) * t32 + (t335 - (t333 - t1526) * t32) * t32) / 0.24E2
     # + t1200 + t1204)
        t1666 = t10 * (t167 + t177 + t101 - t396 - t256 - t414) * t32
        t1668 = t459 * t1666 / 0.2E1
        t1670 = t466 * t1551 / 0.6E1
        t1672 = t472 * t1597 / 0.2E1
        t1674 = t531 * t1653 / 0.4E1
        t1677 = t1548 + t660 * t1551 / 0.6E1 - t118 * t1597 / 0.2E1 - t9
     # * t1653 / 0.4E1 - t118 * t1478 / 0.4E1 - t367 * t1510 / 0.12E2 - 
     #t9 * t1541 / 0.8E1 - t118 * t1546 / 0.24E2 - t1668 - t1670 + t1672
     # + t1674 + t657 * t1666 / 0.2E1 - t1354 - t1356 + t1358
        t1679 = (t1544 + t1677) * t5
        t1689 = (t1413 - (t1411 - (-cc * t1521 + t1409) * t32) * t32) * 
     #t32
        t1696 = dy * (t1408 + t1411 / 0.2E1 - t222 * (t1415 / 0.2E1 + t1
     #689 / 0.2E1) / 0.6E1) / 0.4E1
        t1702 = t222 * (t1413 - dy * (t1415 - t1689) / 0.12E2) / 0.24E2
        t1706 = t12 * (t230 - dy * t242 / 0.24E2)
        t1708 = -t1679 * t457 - t1188 - t1213 + t1217 + t1449 - t1452 - 
     #t1696 - t1702 + t1706 + t646 + t648 + t652
        t1709 = t1394 / 0.2E1
        t1711 = dy * t257 / 0.24E2
        t1712 = -t1709 - t1422 + t1428 - t1711 + t745 - t1480 - t1512 - 
     #t1543 - t1548 + t1668 + t1670 - t1672 - t1674
        t1721 = src(i,j,nComp,n + 2)
        t1723 = (src(i,j,nComp,n + 3) - t1721) * t5
        t1733 = t759 + t676 + t464 - t755 + t471 - t680 + t744 + t530 - 
     #t743 + t589 - t594 + t734
        t1734 = t639 - t641 + t643 - t745 - t646 - t753 - t648 - t650 - 
     #t721 - t652 - t654 - t656
        t1740 = t1059 + t1019 + t999 - t1055 + t1001 - t1022 + t745 + t6
     #46 - t753 + t648 - t650 + t721
        t1741 = t652 - t654 + t656 - t1060 - t1003 - t1053 - t1005 - t10
     #07 - t1045 - t1009 - t1011 - t1013
        t1749 = t1434 + t1222 + t1360 - t1430 + t1362 - t1225 + t1375 + 
     #t1364 - t1407 + t1366 - t1368 + t1440
        t1750 = t1120 - t1166 + t1179 - t745 - t646 - t1422 - t648 - t11
     #88 - t1428 - t652 - t1213 - t1217
        t1756 = t1706 + t1449 + t1668 - t1711 + t1670 - t1452 + t745 + t
     #646 - t1422 + t648 - t1188 + t1428
        t1757 = t652 - t1213 + t1217 - t1709 - t1672 - t1696 - t1674 - t
     #1480 - t1702 - t1512 - t1543 - t1548

        unew(i,j) = t1 + dt * t2 + (t688 * t10 / 0.6E1 + (t692 + t7
     #60) * t10 / 0.2E1 - t1028 * t10 / 0.6E1 - (t1032 + t1061) * t10 / 
     #0.2E1) * t19 + (t1371 * t10 / 0.6E1 + (t1377 + t1441) * t10 / 0.2E
     #1 - t1679 * t10 / 0.6E1 - (t1708 + t1712) * t10 / 0.2E1) * t32 + t
     #1723 * t10 / 0.6E1 + (-t1723 * t457 + t1721) * t10 / 0.2E1

        utnew(i,j) = t2
     # + (t688 * dt / 0.2E1 + (t1733 + t1734) * dt - t688 * t677 - t1028
     # * dt / 0.2E1 - (t1740 + t1741) * dt + t1028 * t677) * t19 + (t137
     #1 * dt / 0.2E1 + (t1749 + t1750) * dt - t1371 * t677 - t1679 * dt 
     #/ 0.2E1 - (t1756 + t1757) * dt + t1679 * t677) * t32 + t1723 * dt 
     #/ 0.2E1 + t1721 * dt - t1723 * t677

        return
      end
