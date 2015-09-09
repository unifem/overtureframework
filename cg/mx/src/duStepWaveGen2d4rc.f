      subroutine duStepWaveGen2d4rc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,
     *   dx,dy,dt,cc,beta,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer i,j,n

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real dx,dy,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t10
        integer t1007
        real t1009
        real t1021
        real t1030
        real t1033
        real t1035
        real t1038
        real t1040
        real t1044
        real t1070
        real t1071
        real t1083
        real t1092
        real t1095
        real t11
        real t1103
        real t1107
        real t1109
        real t1111
        real t1114
        real t1131
        real t1134
        real t1146
        real t1150
        real t1153
        real t1157
        real t1161
        real t1163
        real t1166
        real t1168
        real t1169
        real t1171
        real t1172
        real t1174
        real t1176
        real t1178
        real t1180
        real t1183
        real t1185
        real t1186
        real t1188
        real t1190
        real t1192
        real t1194
        real t1196
        real t1198
        real t12
        real t1200
        real t1202
        real t1204
        real t1206
        real t1208
        real t1210
        real t1212
        real t1216
        real t1217
        real t1218
        real t1220
        real t1224
        real t1226
        real t1229
        real t1230
        real t1231
        real t1233
        real t1236
        real t1237
        real t124
        real t1244
        real t1246
        real t1247
        real t1249
        real t1251
        real t1253
        real t126
        real t1260
        real t1261
        real t1263
        real t1265
        real t1267
        real t127
        real t1273
        real t1275
        real t1279
        real t128
        real t1285
        real t1287
        real t1294
        real t1295
        real t1296
        integer t13
        real t130
        real t1301
        real t1306
        real t131
        real t1310
        real t1313
        real t1315
        integer t1336
        real t1338
        real t134
        real t135
        real t1350
        real t1359
        real t1362
        real t1364
        real t1367
        real t1369
        real t137
        real t1373
        real t139
        real t1399
        real t14
        real t140
        real t1400
        real t1412
        real t1421
        real t1431
        real t1435
        real t1438
        real t144
        real t1455
        real t1472
        real t1476
        real t1480
        real t1482
        real t1483
        real t1485
        real t1487
        real t1489
        real t149
        real t1491
        real t1493
        real t1495
        real t1496
        integer t15
        real t1502
        real t1503
        real t1505
        real t1507
        real t1509
        real t1512
        real t1519
        real t152
        real t1525
        real t1529
        real t1530
        real t1539
        real t154
        real t1541
        real t1542
        real t1551
        real t1552
        real t1558
        real t1559
        real t156
        real t1567
        real t1568
        real t157
        real t1574
        real t1575
        real t158
        real t16
        real t160
        real t161
        real t162
        real t164
        real t166
        real t168
        real t17
        real t170
        real t171
        real t172
        real t174
        real t176
        real t177
        real t178
        real t180
        real t181
        real t182
        real t184
        real t186
        real t188
        real t19
        real t190
        real t193
        real t195
        real t196
        real t197
        real t198
        real t199
        real t2
        real t20
        real t200
        real t204
        real t206
        real t209
        real t21
        real t212
        real t214
        real t216
        real t217
        real t218
        real t222
        real t226
        real t229
        real t23
        real t231
        real t233
        real t234
        real t235
        real t239
        real t246
        real t248
        real t249
        real t25
        real t250
        real t254
        real t26
        real t263
        real t266
        real t267
        real t269
        integer t27
        real t270
        real t271
        real t273
        real t275
        real t277
        real t279
        real t28
        real t282
        real t284
        real t286
        real t288
        real t289
        real t29
        real t290
        real t291
        real t293
        real t294
        real t296
        real t297
        real t299
        integer t300
        real t301
        real t302
        real t303
        real t305
        real t306
        real t308
        real t31
        real t311
        real t312
        real t314
        real t316
        real t317
        real t318
        real t324
        real t325
        real t326
        real t327
        real t329
        real t330
        real t332
        real t333
        real t335
        real t336
        real t337
        real t338
        real t34
        real t340
        real t341
        real t343
        real t347
        real t349
        real t350
        real t351
        real t353
        real t355
        real t356
        real t357
        real t36
        real t364
        real t366
        real t367
        real t368
        real t369
        real t371
        real t372
        real t374
        real t378
        real t380
        real t381
        real t382
        real t388
        real t389
        real t390
        real t391
        real t393
        real t394
        real t396
        real t397
        real t399
        real t4
        integer t40
        real t400
        real t401
        real t402
        real t404
        real t405
        real t407
        real t41
        real t411
        real t413
        real t414
        real t415
        real t417
        real t419
        real t42
        real t420
        real t421
        real t428
        real t43
        real t430
        real t431
        real t432
        real t434
        real t435
        real t436
        real t438
        real t440
        real t442
        real t444
        real t447
        real t449
        real t45
        real t450
        real t452
        real t455
        real t457
        real t459
        real t46
        real t460
        real t462
        real t463
        real t465
        real t467
        real t468
        real t470
        real t474
        real t476
        real t477
        real t478
        real t48
        real t480
        real t481
        real t482
        real t484
        real t486
        real t488
        real t490
        real t493
        real t495
        real t497
        real t499
        real t5
        real t500
        real t501
        real t502
        real t504
        real t507
        real t508
        real t511
        real t514
        real t531
        real t534
        real t536
        real t54
        real t558
        real t561
        real t566
        real t567
        real t575
        real t577
        real t579
        real t58
        real t581
        real t583
        real t585
        real t586
        real t588
        real t589
        real t591
        real t593
        real t594
        real t598
        real t599
        real t6
        real t60
        real t600
        real t601
        real t603
        real t604
        real t606
        real t609
        real t61
        real t611
        real t614
        real t615
        real t616
        real t618
        real t619
        real t62
        real t621
        real t623
        real t624
        real t626
        real t628
        real t630
        real t631
        real t633
        real t635
        real t637
        real t643
        real t645
        real t652
        real t659
        real t660
        real t667
        real t669
        real t673
        real t675
        real t676
        integer t68
        real t682
        real t683
        real t69
        real t690
        real t691
        real t692
        real t694
        real t698
        integer t699
        real t7
        real t70
        real t701
        real t705
        real t706
        real t710
        real t715
        real t719
        real t72
        real t721
        real t723
        real t725
        real t73
        real t730
        integer t74
        real t748
        real t749
        real t75
        real t751
        real t754
        real t756
        real t76
        real t760
        real t761
        real t773
        real t779
        real t78
        real t788
        real t791
        real t792
        real t8
        real t80
        real t804
        real t81
        real t812
        real t813
        real t815
        real t818
        integer t82
        real t820
        real t824
        real t825
        real t83
        real t84
        real t852
        real t857
        real t86
        real t865
        real t869
        real t873
        real t877
        real t884
        real t887
        real t89
        real t891
        real t9
        real t903
        real t909
        real t91
        real t911
        real t913
        real t915
        real t917
        real t919
        real t921
        real t922
        real t924
        real t927
        real t935
        real t941
        real t949
        integer t95
        real t951
        real t955
        real t956
        real t957
        real t96
        real t964
        real t967
        real t969
        real t97
        real t970
        real t972
        real t974
        real t977
        real t981
        real t984
        real t986
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = beta ** 2
        t5 = sqrt(0.3E1)
        t6 = t5 / 0.6E1
        t7 = 0.1E1 / 0.2E1 - t6
        t8 = t7 ** 2
        t9 = t4 * t8
        t10 = dt ** 2
        t11 = t10 * cc
        t12 = cc ** 2
        t13 = i + 2
        t14 = ut(t13,j,n)
        t15 = i + 1
        t16 = ut(t15,j,n)
        t17 = t14 - t16
        t19 = 0.1E1 / dx
        t20 = t12 * t17 * t19
        t21 = t16 - t2
        t23 = t12 * t21 * t19
        t25 = (t20 - t23) * t19
        t26 = dx ** 2
        t27 = i + 3
        t28 = ut(t27,j,n)
        t29 = t28 - t14
        t31 = t17 * t19
        t34 = t21 * t19
        t36 = (t31 - t34) * t19
        t40 = i - 1
        t41 = ut(t40,j,n)
        t42 = t2 - t41
        t43 = t42 * t19
        t45 = (t34 - t43) * t19
        t46 = t36 - t45
        t48 = t12 * t46 * t19
        t54 = (t12 * t19 * t29 - t20) * t19
        t58 = t12 * t42 * t19
        t60 = (t23 - t58) * t19
        t61 = t25 - t60
        t62 = t61 * t19
        t68 = j + 1
        t69 = ut(t15,t68,n)
        t70 = t69 - t16
        t72 = 0.1E1 / dy
        t73 = t12 * t70 * t72
        t74 = j - 1
        t75 = ut(t15,t74,n)
        t76 = t16 - t75
        t78 = t12 * t76 * t72
        t80 = (t73 - t78) * t72
        t81 = dy ** 2
        t82 = j + 2
        t83 = ut(t15,t82,n)
        t84 = t83 - t69
        t86 = t70 * t72
        t89 = t76 * t72
        t91 = (t86 - t89) * t72
        t95 = j - 2
        t96 = ut(t15,t95,n)
        t97 = t75 - t96
        t124 = t11 * (t25 - t26 * ((t12 * ((t19 * t29 - t31) * t19 - t36
     #) * t19 - t48) * t19 + ((t54 - t25) * t19 - t62) * t19) / 0.24E2 +
     # t80 - t81 * ((t12 * ((t72 * t84 - t86) * t72 - t91) * t72 - t12 *
     # (t91 - (-t72 * t97 + t89) * t72) * t72) * t72 + (((t12 * t72 * t8
     #4 - t73) * t72 - t80) * t72 - (t80 - (-t12 * t72 * t97 + t78) * t7
     #2) * t72) * t72) / 0.24E2)
        t126 = t9 * t124 / 0.4E1
        t127 = beta * t7
        t128 = dt * dx
        t130 = u(t13,j,n)
        t131 = u(t27,j,n) - t130
        t134 = u(t15,j,n)
        t135 = t130 - t134
        t137 = t12 * t135 * t19
        t139 = (t12 * t131 * t19 - t137) * t19
        t140 = u(t13,t68,n)
        t144 = u(t13,t74,n)
        t149 = (t12 * (t140 - t130) * t72 - t12 * (t130 - t144) * t72) *
     # t72
        t152 = t134 - t1
        t154 = t12 * t152 * t19
        t156 = (t137 - t154) * t19
        t157 = u(t15,t68,n)
        t158 = t157 - t134
        t160 = t12 * t158 * t72
        t161 = u(t15,t74,n)
        t162 = t134 - t161
        t164 = t12 * t162 * t72
        t166 = (t160 - t164) * t72
        t168 = cc * (t156 + t166)
        t170 = (cc * (t139 + t149) - t168) * t19
        t171 = u(t40,j,n)
        t172 = t1 - t171
        t174 = t12 * t172 * t19
        t176 = (t154 - t174) * t19
        t177 = u(i,t68,n)
        t178 = t177 - t1
        t180 = t12 * t178 * t72
        t181 = u(i,t74,n)
        t182 = t1 - t181
        t184 = t12 * t182 * t72
        t186 = (t180 - t184) * t72
        t188 = cc * (t176 + t186)
        t190 = (t168 - t188) * t19
        t193 = t128 * (t170 / 0.2E1 + t190 / 0.2E1)
        t195 = t127 * t193 / 0.4E1
        t196 = t4 * beta
        t197 = t8 * t7
        t198 = t196 * t197
        t199 = t10 * dt
        t200 = t199 * cc
        t204 = t156 + t166 - t176 - t186
        t206 = t12 * t204 * t19
        t209 = t140 - t157
        t212 = t157 - t177
        t214 = t12 * t212 * t19
        t216 = (t12 * t19 * t209 - t214) * t19
        t217 = u(t15,t82,n)
        t218 = t217 - t157
        t222 = (t12 * t218 * t72 - t160) * t72
        t226 = t144 - t161
        t229 = t161 - t181
        t231 = t12 * t229 * t19
        t233 = (t12 * t19 * t226 - t231) * t19
        t234 = u(t15,t95,n)
        t235 = t161 - t234
        t239 = (-t12 * t235 * t72 + t164) * t72
        t246 = t200 * ((t12 * (t139 + t149 - t156 - t166) * t19 - t206) 
     #* t19 + (t12 * (t216 + t222 - t156 - t166) * t72 - t12 * (t156 + t
     #166 - t233 - t239) * t72) * t72)
        t248 = t198 * t246 / 0.12E2
        t249 = t10 * dx
        t250 = ut(t13,t68,n)
        t254 = ut(t13,t74,n)
        t263 = cc * (t25 + t80)
        t266 = ut(i,t68,n)
        t267 = t266 - t2
        t269 = t12 * t267 * t72
        t270 = ut(i,t74,n)
        t271 = t2 - t270
        t273 = t12 * t271 * t72
        t275 = (t269 - t273) * t72
        t277 = cc * (t60 + t275)
        t279 = (t263 - t277) * t19
        t282 = t249 * ((cc * (t54 + (t12 * (t250 - t14) * t72 - t12 * (t
     #14 - t254) * t72) * t72) - t263) * t19 / 0.2E1 + t279 / 0.2E1)
        t284 = t9 * t282 / 0.8E1
        t286 = t128 * (t170 - t190)
        t288 = t127 * t286 / 0.24E2
        t289 = dt * cc
        t290 = t135 * t19
        t291 = t152 * t19
        t293 = (t290 - t291) * t19
        t294 = t172 * t19
        t296 = (t291 - t294) * t19
        t297 = t293 - t296
        t299 = t12 * t297 * t19
        t300 = i - 2
        t301 = u(t300,j,n)
        t302 = t171 - t301
        t303 = t302 * t19
        t305 = (t294 - t303) * t19
        t306 = t296 - t305
        t308 = t12 * t306 * t19
        t311 = t156 - t176
        t312 = t311 * t19
        t314 = t12 * t302 * t19
        t316 = (t174 - t314) * t19
        t317 = t176 - t316
        t318 = t317 * t19
        t324 = u(i,t82,n)
        t325 = t324 - t177
        t326 = t325 * t72
        t327 = t178 * t72
        t329 = (t326 - t327) * t72
        t330 = t182 * t72
        t332 = (t327 - t330) * t72
        t333 = t329 - t332
        t335 = t12 * t333 * t72
        t336 = u(i,t95,n)
        t337 = t181 - t336
        t338 = t337 * t72
        t340 = (t330 - t338) * t72
        t341 = t332 - t340
        t343 = t12 * t341 * t72
        t347 = t12 * t325 * t72
        t349 = (t347 - t180) * t72
        t350 = t349 - t186
        t351 = t350 * t72
        t353 = t12 * t337 * t72
        t355 = (t184 - t353) * t72
        t356 = t186 - t355
        t357 = t356 * t72
        t364 = t289 * (t176 - t26 * ((t299 - t308) * t19 + (t312 - t318)
     # * t19) / 0.24E2 + t186 - t81 * ((t335 - t343) * t72 + (t351 - t35
     #7) * t72) / 0.24E2)
        t366 = t127 * t364 / 0.2E1
        t367 = ut(t300,j,n)
        t368 = t41 - t367
        t369 = t368 * t19
        t371 = (t43 - t369) * t19
        t372 = t45 - t371
        t374 = t12 * t372 * t19
        t378 = t12 * t368 * t19
        t380 = (t58 - t378) * t19
        t381 = t60 - t380
        t382 = t381 * t19
        t388 = ut(i,t82,n)
        t389 = t388 - t266
        t390 = t389 * t72
        t391 = t267 * t72
        t393 = (t390 - t391) * t72
        t394 = t271 * t72
        t396 = (t391 - t394) * t72
        t397 = t393 - t396
        t399 = t12 * t397 * t72
        t400 = ut(i,t95,n)
        t401 = t270 - t400
        t402 = t401 * t72
        t404 = (t394 - t402) * t72
        t405 = t396 - t404
        t407 = t12 * t405 * t72
        t411 = t12 * t389 * t72
        t413 = (t411 - t269) * t72
        t414 = t413 - t275
        t415 = t414 * t72
        t417 = t12 * t401 * t72
        t419 = (t273 - t417) * t72
        t420 = t275 - t419
        t421 = t420 * t72
        t428 = t11 * (t60 - t26 * ((t48 - t374) * t19 + (t62 - t382) * t
     #19) / 0.24E2 + t275 - t81 * ((t399 - t407) * t72 + (t415 - t421) *
     # t72) / 0.24E2)
        t430 = t9 * t428 / 0.4E1
        t431 = u(t40,t68,n)
        t432 = t431 - t171
        t434 = t12 * t432 * t72
        t435 = u(t40,t74,n)
        t436 = t171 - t435
        t438 = t12 * t436 * t72
        t440 = (t434 - t438) * t72
        t442 = cc * (t316 + t440)
        t444 = (t188 - t442) * t19
        t447 = t128 * (t190 / 0.2E1 + t444 / 0.2E1)
        t449 = t127 * t447 / 0.4E1
        t450 = t176 + t186 - t316 - t440
        t452 = t12 * t450 * t19
        t455 = t177 - t431
        t457 = t12 * t455 * t19
        t459 = (t214 - t457) * t19
        t460 = t459 + t349 - t176 - t186
        t462 = t12 * t460 * t72
        t463 = t181 - t435
        t465 = t12 * t463 * t19
        t467 = (t231 - t465) * t19
        t468 = t176 + t186 - t467 - t355
        t470 = t12 * t468 * t72
        t474 = t200 * ((t206 - t452) * t19 + (t462 - t470) * t72)
        t476 = t198 * t474 / 0.12E2
        t477 = ut(t40,t68,n)
        t478 = t477 - t41
        t480 = t12 * t478 * t72
        t481 = ut(t40,t74,n)
        t482 = t41 - t481
        t484 = t12 * t482 * t72
        t486 = (t480 - t484) * t72
        t488 = cc * (t380 + t486)
        t490 = (t277 - t488) * t19
        t493 = t249 * (t279 / 0.2E1 + t490 / 0.2E1)
        t495 = t9 * t493 / 0.8E1
        t497 = t128 * (t190 - t444)
        t499 = t127 * t497 / 0.24E2
        t500 = 0.1E1 / 0.2E1 + t6
        t501 = t500 ** 2
        t502 = t12 * t501
        t504 = t10 * t204 * t19
        t507 = t501 * t500
        t508 = t12 * t507
        t511 = t199 * (t25 + t80 - t60 - t275) * t19
        t514 = beta * t500
        t531 = t158 * t72
        t534 = t162 * t72
        t536 = (t531 - t534) * t72
        t558 = t289 * (t156 - t26 * ((t12 * ((t131 * t19 - t290) * t19 -
     # t293) * t19 - t299) * t19 + ((t139 - t156) * t19 - t312) * t19) /
     # 0.24E2 + t166 - t81 * ((t12 * ((t218 * t72 - t531) * t72 - t536) 
     #* t72 - t12 * (t536 - (-t235 * t72 + t534) * t72) * t72) * t72 + (
     #(t222 - t166) * t72 - (t166 - t239) * t72) * t72) / 0.24E2)
        t561 = t4 * t501
        t566 = -t126 + t195 - t248 + t284 - t288 + t366 + t430 + t449 + 
     #t476 + t495 + t499 + t502 * t504 / 0.2E1 + t508 * t511 / 0.6E1 + t
     #514 * t558 / 0.2E1 + t561 * t124 / 0.4E1 - t514 * t193 / 0.4E1
        t567 = t196 * t507
        t575 = t514 * t364 / 0.2E1
        t577 = t561 * t428 / 0.4E1
        t579 = t514 * t447 / 0.4E1
        t581 = t567 * t474 / 0.12E2
        t583 = t561 * t493 / 0.8E1
        t585 = t514 * t497 / 0.24E2
        t586 = t12 * t8
        t588 = t586 * t504 / 0.2E1
        t589 = t12 * t197
        t591 = t589 * t511 / 0.6E1
        t593 = t127 * t558 / 0.2E1
        t594 = t12 * t7
        t598 = dt * (t34 - dx * t46 / 0.24E2)
        t599 = t594 * t598
        t600 = t7 * dt
        t601 = dx * t61
        t603 = t600 * t601 / 0.24E2
        t604 = t12 * t500
        t606 = t500 * dt
        t609 = t567 * t246 / 0.12E2 - t561 * t282 / 0.8E1 + t514 * t286 
     #/ 0.24E2 - t575 - t577 - t579 - t581 - t583 - t585 - t588 - t591 -
     # t593 - t599 + t603 + t604 * t598 - t606 * t601 / 0.24E2
        t611 = (t566 + t609) * t5
        t614 = t126 - t195 + t248 - t284 + t288 - t366 - t430 - t449 - t
     #476 - t495 - t499 + t588
        t615 = cc * t2
        t616 = cc * t16
        t618 = (-t615 + t616) * t19
        t619 = cc * t41
        t621 = (t615 - t619) * t19
        t623 = (t618 - t621) * t19
        t624 = cc * t14
        t626 = (-t616 + t624) * t19
        t628 = (t626 - t618) * t19
        t630 = (t628 - t623) * t19
        t631 = cc * t367
        t633 = (-t631 + t619) * t19
        t635 = (t621 - t633) * t19
        t637 = (t623 - t635) * t19
        t643 = t26 * (t623 - dx * (t630 - t637) / 0.12E2) / 0.24E2
        t645 = t618 / 0.2E1
        t652 = (((cc * t28 - t624) * t19 - t626) * t19 - t628) * t19
        t659 = dx * (t626 / 0.2E1 + t645 - t26 * (t652 / 0.2E1 + t630 / 
     #0.2E1) / 0.6E1) / 0.4E1
        t660 = t621 / 0.2E1
        t667 = dx * (t645 + t660 - t26 * (t630 / 0.2E1 + t637 / 0.2E1) /
     # 0.6E1) / 0.4E1
        t669 = dx * t311 / 0.24E2
        t673 = t12 * (t291 - dx * t297 / 0.24E2)
        t675 = t616 / 0.2E1
        t676 = t615 / 0.2E1
        t682 = t26 * (t628 - dx * (t652 - t630) / 0.12E2) / 0.24E2
        t683 = -t611 * t7 + t591 + t593 + t599 - t603 - t643 - t659 - t6
     #67 - t669 + t673 + t675 - t676 + t682
        t690 = dt * (t43 - dx * t372 / 0.24E2)
        t691 = t594 * t690
        t692 = dx * t381
        t694 = t600 * t692 / 0.24E2
        t698 = -t366 - t430 + t449 - t476 + t495 - t499 - t691 + t694 + 
     #t604 * t690 - t606 * t692 / 0.24E2 + t575 + t577 - t579 + t581 - t
     #583 + t585
        t699 = i - 3
        t701 = t301 - u(t699,j,n)
        t705 = (-t12 * t19 * t701 + t314) * t19
        t706 = u(t300,t68,n)
        t710 = u(t300,t74,n)
        t715 = (t12 * (t706 - t301) * t72 - t12 * (t301 - t710) * t72) *
     # t72
        t719 = (t442 - cc * (t705 + t715)) * t19
        t721 = t128 * (t444 - t719)
        t723 = t127 * t721 / 0.24E2
        t725 = t10 * t450 * t19
        t730 = t199 * (t60 + t275 - t380 - t486) * t19
        t748 = u(t40,t82,n)
        t749 = t748 - t431
        t751 = t432 * t72
        t754 = t436 * t72
        t756 = (t751 - t754) * t72
        t760 = u(t40,t95,n)
        t761 = t435 - t760
        t773 = (t12 * t72 * t749 - t434) * t72
        t779 = (-t12 * t72 * t761 + t438) * t72
        t788 = t289 * (t316 - t26 * ((t308 - t12 * (t305 - (-t19 * t701 
     #+ t303) * t19) * t19) * t19 + (t318 - (t316 - t705) * t19) * t19) 
     #/ 0.24E2 + t440 - t81 * ((t12 * ((t72 * t749 - t751) * t72 - t756)
     # * t72 - t12 * (t756 - (-t72 * t761 + t754) * t72) * t72) * t72 + 
     #((t773 - t440) * t72 - (t440 - t779) * t72) * t72) / 0.24E2)
        t791 = ut(t699,j,n)
        t792 = t367 - t791
        t804 = (-t12 * t19 * t792 + t378) * t19
        t812 = ut(t40,t82,n)
        t813 = t812 - t477
        t815 = t478 * t72
        t818 = t482 * t72
        t820 = (t815 - t818) * t72
        t824 = ut(t40,t95,n)
        t825 = t481 - t824
        t852 = t11 * (t380 - t26 * ((t374 - t12 * (t371 - (-t19 * t792 +
     # t369) * t19) * t19) * t19 + (t382 - (t380 - t804) * t19) * t19) /
     # 0.24E2 + t486 - t81 * ((t12 * ((t72 * t813 - t815) * t72 - t820) 
     #* t72 - t12 * (t820 - (-t72 * t825 + t818) * t72) * t72) * t72 + (
     #((t12 * t72 * t813 - t480) * t72 - t486) * t72 - (t486 - (-t12 * t
     #72 * t825 + t484) * t72) * t72) * t72) / 0.24E2)
        t857 = t128 * (t444 / 0.2E1 + t719 / 0.2E1)
        t865 = t431 - t706
        t869 = (-t12 * t19 * t865 + t457) * t19
        t873 = t435 - t710
        t877 = (-t12 * t19 * t873 + t465) * t19
        t884 = t200 * ((t452 - t12 * (t316 + t440 - t705 - t715) * t19) 
     #* t19 + (t12 * (t869 + t773 - t316 - t440) * t72 - t12 * (t316 + t
     #440 - t877 - t779) * t72) * t72)
        t887 = ut(t300,t68,n)
        t891 = ut(t300,t74,n)
        t903 = t249 * (t490 / 0.2E1 + (t488 - cc * (t804 + (t12 * (t887 
     #- t367) * t72 - t12 * (t367 - t891) * t72) * t72)) * t19 / 0.2E1)
        t909 = t586 * t725 / 0.2E1
        t911 = t589 * t730 / 0.6E1
        t913 = t127 * t788 / 0.2E1
        t915 = t9 * t852 / 0.4E1
        t917 = t127 * t857 / 0.4E1
        t919 = t198 * t884 / 0.12E2
        t921 = t9 * t903 / 0.8E1
        t922 = t723 + t502 * t725 / 0.2E1 + t508 * t730 / 0.6E1 - t514 *
     # t788 / 0.2E1 - t561 * t852 / 0.4E1 - t514 * t857 / 0.4E1 - t567 *
     # t884 / 0.12E2 - t561 * t903 / 0.8E1 - t514 * t721 / 0.24E2 - t909
     # - t911 + t913 + t915 + t917 + t919 + t921
        t924 = (t698 + t922) * t5
        t927 = t366 + t430 - t449 + t476 - t495 + t499 + t691 - t694 + t
     #643 - t723 + t909 + t911
        t935 = (t635 - (t633 - (-cc * t791 + t631) * t19) * t19) * t19
        t941 = t26 * (t635 - dx * (t637 - t935) / 0.12E2) / 0.24E2
        t949 = dx * (t660 + t633 / 0.2E1 - t26 * (t637 / 0.2E1 + t935 / 
     #0.2E1) / 0.6E1) / 0.4E1
        t951 = dx * t317 / 0.24E2
        t955 = t12 * (t294 - dx * t306 / 0.24E2)
        t956 = t619 / 0.2E1
        t957 = -t7 * t924 - t667 + t676 - t913 - t915 - t917 - t919 - t9
     #21 - t941 - t949 - t951 + t955 - t956
        t964 = t10 * t460 * t72
        t967 = t69 - t266
        t969 = t12 * t967 * t19
        t970 = t266 - t477
        t972 = t12 * t970 * t19
        t974 = (t969 - t972) * t19
        t977 = t199 * (t974 + t413 - t60 - t275) * t72
        t981 = t212 * t19
        t984 = t455 * t19
        t986 = (t981 - t984) * t19
        t1007 = j + 3
        t1009 = u(i,t1007,n) - t324
        t1021 = (t1009 * t12 * t72 - t347) * t72
        t1030 = t289 * (t459 - t26 * ((t12 * ((t19 * t209 - t981) * t19 
     #- t986) * t19 - t12 * (t986 - (-t19 * t865 + t984) * t19) * t19) *
     # t19 + ((t216 - t459) * t19 - (t459 - t869) * t19) * t19) / 0.24E2
     # + t349 - t81 * ((t12 * ((t1009 * t72 - t326) * t72 - t329) * t72 
     #- t335) * t72 + ((t1021 - t349) * t72 - t351) * t72) / 0.24E2)
        t1033 = t250 - t69
        t1035 = t967 * t19
        t1038 = t970 * t19
        t1040 = (t1035 - t1038) * t19
        t1044 = t477 - t887
        t1070 = ut(i,t1007,n)
        t1071 = t1070 - t388
        t1083 = (t1071 * t12 * t72 - t411) * t72
        t1092 = t11 * (t974 - t26 * ((t12 * ((t1033 * t19 - t1035) * t19
     # - t1040) * t19 - t12 * (t1040 - (-t1044 * t19 + t1038) * t19) * t
     #19) * t19 + (((t1033 * t12 * t19 - t969) * t19 - t974) * t19 - (t9
     #74 - (-t1044 * t12 * t19 + t972) * t19) * t19) * t19) / 0.24E2 + t
     #413 - t81 * ((t12 * ((t1071 * t72 - t390) * t72 - t393) * t72 - t3
     #99) * t72 + ((t1083 - t413) * t72 - t415) * t72) / 0.24E2)
        t1095 = dt * dy
        t1103 = (t12 * (t217 - t324) * t19 - t12 * (t324 - t748) * t19) 
     #* t19
        t1107 = cc * (t459 + t349)
        t1109 = (cc * (t1103 + t1021) - t1107) * t72
        t1111 = (t1107 - t188) * t72
        t1114 = t1095 * (t1109 / 0.2E1 + t1111 / 0.2E1)
        t1131 = t200 * ((t12 * (t216 + t222 - t459 - t349) * t19 - t12 *
     # (t459 + t349 - t869 - t773) * t19) * t19 + (t12 * (t1103 + t1021 
     #- t459 - t349) * t72 - t462) * t72)
        t1134 = t10 * dy
        t1146 = cc * (t974 + t413)
        t1150 = (t1146 - t277) * t72
        t1153 = t1134 * ((cc * ((t12 * (t83 - t388) * t19 - t12 * (t388 
     #- t812) * t19) * t19 + t1083) - t1146) * t72 / 0.2E1 + t1150 / 0.2
     #E1)
        t1157 = t1095 * (t1109 - t1111)
        t1161 = cc * (t467 + t355)
        t1163 = (t188 - t1161) * t72
        t1166 = t1095 * (t1111 / 0.2E1 + t1163 / 0.2E1)
        t1168 = t514 * t1166 / 0.4E1
        t1169 = t75 - t270
        t1171 = t12 * t1169 * t19
        t1172 = t270 - t481
        t1174 = t12 * t1172 * t19
        t1176 = (t1171 - t1174) * t19
        t1178 = cc * (t1176 + t419)
        t1180 = (t277 - t1178) * t72
        t1183 = t1134 * (t1150 / 0.2E1 + t1180 / 0.2E1)
        t1185 = t561 * t1183 / 0.8E1
        t1186 = t366 + t430 + t476 - t575 - t577 - t581 + t502 * t964 / 
     #0.2E1 + t508 * t977 / 0.6E1 + t514 * t1030 / 0.2E1 + t561 * t1092 
     #/ 0.4E1 - t514 * t1114 / 0.4E1 + t567 * t1131 / 0.12E2 - t561 * t1
     #153 / 0.8E1 + t514 * t1157 / 0.24E2 - t1168 - t1185
        t1188 = t1095 * (t1111 - t1163)
        t1190 = t514 * t1188 / 0.24E2
        t1192 = t586 * t964 / 0.2E1
        t1194 = t589 * t977 / 0.6E1
        t1196 = t127 * t1030 / 0.2E1
        t1198 = t9 * t1092 / 0.4E1
        t1200 = t127 * t1114 / 0.4E1
        t1202 = t198 * t1131 / 0.12E2
        t1204 = t9 * t1153 / 0.8E1
        t1206 = t127 * t1157 / 0.24E2
        t1208 = t127 * t1166 / 0.4E1
        t1210 = t9 * t1183 / 0.8E1
        t1212 = t127 * t1188 / 0.24E2
        t1216 = dt * (t391 - dy * t397 / 0.24E2)
        t1217 = t594 * t1216
        t1218 = dy * t414
        t1220 = t600 * t1218 / 0.24E2
        t1224 = -t1190 - t1192 - t1194 - t1196 - t1198 + t1200 - t1202 +
     # t1204 - t1206 + t1208 + t1210 + t1212 - t1217 + t1220 + t604 * t1
     #216 - t606 * t1218 / 0.24E2
        t1226 = (t1186 + t1224) * t5
        t1229 = cc * t266
        t1230 = t1229 / 0.2E1
        t1231 = cc * t388
        t1233 = (-t1229 + t1231) * t72
        t1236 = (-t615 + t1229) * t72
        t1237 = t1236 / 0.2E1
        t1244 = (t1233 - t1236) * t72
        t1246 = (((cc * t1070 - t1231) * t72 - t1233) * t72 - t1244) * t
     #72
        t1247 = cc * t270
        t1249 = (t615 - t1247) * t72
        t1251 = (t1236 - t1249) * t72
        t1253 = (t1244 - t1251) * t72
        t1260 = dy * (t1233 / 0.2E1 + t1237 - t81 * (t1246 / 0.2E1 + t12
     #53 / 0.2E1) / 0.6E1) / 0.4E1
        t1261 = cc * t400
        t1263 = (-t1261 + t1247) * t72
        t1265 = (t1249 - t1263) * t72
        t1267 = (t1251 - t1265) * t72
        t1273 = t81 * (t1251 - dy * (t1253 - t1267) / 0.12E2) / 0.24E2
        t1275 = dy * t350 / 0.24E2
        t1279 = t12 * (t327 - dy * t333 / 0.24E2)
        t1285 = t81 * (t1244 - dy * (t1246 - t1253) / 0.12E2) / 0.24E2
        t1287 = t1249 / 0.2E1
        t1294 = dy * (t1237 + t1287 - t81 * (t1253 / 0.2E1 + t1267 / 0.2
     #E1) / 0.6E1) / 0.4E1
        t1295 = -t1226 * t7 + t1192 + t1230 - t1260 - t1273 - t1275 + t1
     #279 + t1285 - t1294 - t366 - t430 - t476
        t1296 = t1194 + t1196 + t1198 - t1200 + t1202 - t1204 + t1206 - 
     #t1208 - t1210 - t1212 + t1217 - t1220 - t676
        t1301 = t10 * t468 * t72
        t1306 = t199 * (t60 + t275 - t1176 - t419) * t72
        t1310 = t229 * t19
        t1313 = t463 * t19
        t1315 = (t1310 - t1313) * t19
        t1336 = j - 3
        t1338 = t336 - u(i,t1336,n)
        t1350 = (-t12 * t1338 * t72 + t353) * t72
        t1359 = t289 * (t467 - t26 * ((t12 * ((t19 * t226 - t1310) * t19
     # - t1315) * t19 - t12 * (t1315 - (-t19 * t873 + t1313) * t19) * t1
     #9) * t19 + ((t233 - t467) * t19 - (t467 - t877) * t19) * t19) / 0.
     #24E2 + t355 - t81 * ((t343 - t12 * (t340 - (-t1338 * t72 + t338) *
     # t72) * t72) * t72 + (t357 - (t355 - t1350) * t72) * t72) / 0.24E2
     #)
        t1362 = t254 - t75
        t1364 = t1169 * t19
        t1367 = t1172 * t19
        t1369 = (t1364 - t1367) * t19
        t1373 = t481 - t891
        t1399 = ut(i,t1336,n)
        t1400 = t400 - t1399
        t1412 = (-t12 * t1400 * t72 + t417) * t72
        t1421 = t11 * (t1176 - t26 * ((t12 * ((t1362 * t19 - t1364) * t1
     #9 - t1369) * t19 - t12 * (t1369 - (-t1373 * t19 + t1367) * t19) * 
     #t19) * t19 + (((t12 * t1362 * t19 - t1171) * t19 - t1176) * t19 - 
     #(t1176 - (-t12 * t1373 * t19 + t1174) * t19) * t19) * t19) / 0.24E
     #2 + t419 - t81 * ((t407 - t12 * (t404 - (-t1400 * t72 + t402) * t7
     #2) * t72) * t72 + (t421 - (t419 - t1412) * t72) * t72) / 0.24E2)
        t1431 = (t12 * (t234 - t336) * t19 - t12 * (t336 - t760) * t19) 
     #* t19
        t1435 = (t1161 - cc * (t1431 + t1350)) * t72
        t1438 = t1095 * (t1163 / 0.2E1 + t1435 / 0.2E1)
        t1455 = t200 * ((t12 * (t233 + t239 - t467 - t355) * t19 - t12 *
     # (t467 + t355 - t877 - t779) * t19) * t19 + (t470 - t12 * (t467 + 
     #t355 - t1431 - t1350) * t72) * t72)
        t1472 = t1134 * (t1180 / 0.2E1 + (t1178 - cc * ((t12 * (t96 - t4
     #00) * t19 - t12 * (t400 - t824) * t19) * t19 + t1412)) * t72 / 0.2
     #E1)
        t1476 = t1095 * (t1163 - t1435)
        t1480 = t586 * t1301 / 0.2E1
        t1482 = t589 * t1306 / 0.6E1
        t1483 = -t366 - t430 - t476 + t575 + t577 + t581 + t502 * t1301 
     #/ 0.2E1 + t508 * t1306 / 0.6E1 - t514 * t1359 / 0.2E1 - t561 * t14
     #21 / 0.4E1 - t514 * t1438 / 0.4E1 - t567 * t1455 / 0.12E2 - t561 *
     # t1472 / 0.8E1 - t514 * t1476 / 0.24E2 - t1480 - t1482
        t1485 = t127 * t1359 / 0.2E1
        t1487 = t9 * t1421 / 0.4E1
        t1489 = t127 * t1438 / 0.4E1
        t1491 = t198 * t1455 / 0.12E2
        t1493 = t9 * t1472 / 0.8E1
        t1495 = t127 * t1476 / 0.24E2
        t1496 = dy * t420
        t1502 = dt * (t394 - dy * t405 / 0.24E2)
        t1503 = t594 * t1502
        t1505 = t600 * t1496 / 0.24E2
        t1507 = t1485 + t1487 + t1489 + t1491 + t1493 + t1495 - t1168 - 
     #t1185 + t1190 + t1208 + t1210 - t1212 - t606 * t1496 / 0.24E2 - t1
     #503 + t1505 + t604 * t1502
        t1509 = (t1483 + t1507) * t5
        t1512 = t1247 / 0.2E1
        t1519 = (t1265 - (t1263 - (-cc * t1399 + t1261) * t72) * t72) * 
     #t72
        t1525 = t81 * (t1265 - dy * (t1267 - t1519) / 0.12E2) / 0.24E2
        t1529 = t12 * (t330 - dy * t341 / 0.24E2)
        t1530 = t366 + t430 + t476 - t1512 + t1273 - t1294 - t1525 + t15
     #29 + t1480 + t1482 - t1485 - t1487
        t1539 = dy * (t1287 + t1263 / 0.2E1 - t81 * (t1267 / 0.2E1 + t15
     #19 / 0.2E1) / 0.6E1) / 0.4E1
        t1541 = dy * t356 / 0.24E2
        t1542 = -t1509 * t7 - t1208 - t1210 + t1212 - t1489 - t1491 - t1
     #493 - t1495 + t1503 - t1505 - t1539 - t1541 + t676
        t1551 = t673 + t599 + t588 - t669 + t591 - t603 + t675 + t593 - 
     #t659 + t126 - t195 + t682
        t1552 = t248 - t284 + t288 - t676 - t366 - t667 - t430 - t449 - 
     #t643 - t476 - t495 - t499
        t1558 = t955 + t691 + t909 - t951 + t911 - t694 + t676 + t366 - 
     #t667 + t430 - t449 + t643
        t1559 = t476 - t495 + t499 - t956 - t913 - t949 - t915 - t917 - 
     #t941 - t919 - t921 - t723
        t1567 = t1279 + t1217 + t1192 - t1275 + t1194 - t1220 + t1230 + 
     #t1196 - t1260 + t1198 - t1200 + t1285
        t1568 = t1202 - t1204 + t1206 - t676 - t366 - t1294 - t430 - t12
     #08 - t1273 - t476 - t1210 - t1212
        t1574 = t1529 + t1503 + t1480 - t1541 + t1482 - t1505 + t676 + t
     #366 - t1294 + t430 - t1208 + t1273
        t1575 = t476 - t1210 + t1212 - t1512 - t1485 - t1539 - t1487 - t
     #1489 - t1525 - t1491 - t1493 - t1495

        unew(i,j) = t1 + dt * t2 + (t611 * t10 / 0.6E1 + (t614 + t6
     #83) * t10 / 0.2E1 - t924 * t10 / 0.6E1 - (t927 + t957) * t10 / 0.2
     #E1) * t19 + (t1226 * t10 / 0.6E1 + (t1295 + t1296) * t10 / 0.2E1 -
     # t1509 * t10 / 0.6E1 - (t1530 + t1542) * t10 / 0.2E1) * t72

        utnew(i,j) = t
     #2 + (t611 * dt / 0.2E1 + (t1551 + t1552) * dt - t611 * t600 - t924
     # * dt / 0.2E1 - (t1558 + t1559) * dt + t924 * t600) * t19 + (t1226
     # * dt / 0.2E1 + (t1567 + t1568) * dt - t1226 * t600 - t1509 * dt /
     # 0.2E1 - (t1574 + t1575) * dt + t1509 * t600) * t72

        return
      end
