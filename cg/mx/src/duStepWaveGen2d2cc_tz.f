      subroutine duStepWaveGen2d2cc_tz( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   ndf4a,ndf4b,nComp,
     *   u,ut,unew,utnew,
     *   rx,src,
     *   dx,dy,dt,cc,
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
      real rx   (nd1a:nd1b,nd2a:nd2b,0:1,0:1)
      real src  (nd1a:nd1b,nd2a:nd2b,ndf4a:ndf4b,1:*)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t103
        real t110
        real t111
        real t113
        real t114
        real t116
        real t12
        real t120
        real t123
        real t124
        real t125
        real t127
        real t128
        real t13
        real t131
        real t139
        real t14
        real t140
        real t147
        real t15
        real t151
        real t152
        real t153
        real t154
        real t155
        real t158
        real t16
        real t166
        real t17
        real t171
        real t174
        real t175
        real t178
        real t179
        real t18
        real t181
        real t186
        real t187
        real t19
        real t194
        real t2
        real t204
        real t207
        real t209
        real t21
        real t211
        real t212
        real t214
        real t216
        real t217
        real t218
        integer t219
        real t22
        real t220
        real t223
        real t225
        real t229
        real t230
        real t232
        real t24
        real t241
        integer t244
        real t245
        real t246
        real t248
        real t249
        real t25
        real t251
        real t252
        real t254
        real t255
        real t256
        real t257
        real t258
        real t26
        real t260
        real t262
        real t264
        real t266
        real t27
        real t272
        real t274
        real t276
        real t279
        real t28
        real t280
        real t282
        real t283
        real t288
        real t289
        real t29
        real t290
        real t292
        real t293
        real t295
        real t299
        real t302
        real t303
        real t304
        real t306
        real t307
        real t309
        real t310
        real t314
        real t316
        real t32
        real t320
        real t324
        real t327
        real t328
        real t329
        real t33
        real t331
        real t332
        real t334
        real t335
        real t339
        real t341
        real t345
        real t348
        real t349
        real t35
        real t350
        real t351
        real t352
        real t353
        real t354
        real t355
        real t356
        real t359
        real t36
        real t360
        real t361
        real t362
        real t363
        real t364
        real t367
        real t368
        real t37
        real t374
        real t377
        real t378
        integer t379
        real t38
        real t380
        real t381
        real t383
        real t384
        real t387
        real t389
        real t390
        real t391
        real t392
        real t395
        real t397
        real t399
        real t4
        real t401
        real t408
        real t409
        real t410
        real t413
        real t416
        real t420
        real t423
        real t426
        real t428
        real t429
        real t431
        real t437
        real t45
        real t450
        real t452
        real t453
        real t460
        real t463
        real t47
        real t473
        real t474
        real t476
        real t477
        real t480
        real t489
        real t49
        real t494
        real t498
        real t499
        integer t5
        real t501
        real t502
        real t503
        real t505
        integer t51
        real t519
        real t52
        real t520
        real t523
        real t524
        real t526
        real t53
        real t531
        real t532
        integer t549
        real t55
        real t550
        real t553
        real t559
        real t56
        real t560
        real t562
        real t57
        real t579
        real t584
        real t588
        real t59
        real t591
        real t599
        real t6
        real t60
        real t608
        real t61
        real t616
        real t617
        real t62
        real t623
        real t630
        real t632
        real t635
        real t636
        real t639
        real t640
        real t642
        real t647
        real t648
        integer t657
        real t658
        real t665
        real t667
        real t67
        real t671
        real t675
        real t685
        real t686
        real t688
        real t689
        real t69
        real t692
        real t7
        real t70
        real t708
        real t709
        real t710
        real t726
        real t729
        real t731
        real t733
        real t735
        real t737
        real t738
        real t739
        integer t740
        real t742
        real t745
        real t750
        real t751
        real t753
        real t762
        real t766
        real t768
        integer t77
        real t770
        real t772
        real t774
        real t78
        real t782
        real t786
        real t787
        integer t788
        real t789
        real t790
        real t792
        real t793
        real t796
        real t798
        real t799
        real t80
        real t800
        real t801
        real t804
        real t806
        real t808
        real t810
        real t817
        real t818
        integer t82
        real t823
        real t83
        real t836
        real t844
        real t845
        real t848
        real t849
        real t851
        real t856
        real t857
        real t866
        real t873
        real t875
        real t879
        real t883
        real t9
        integer t923
        real t925
        real t928
        real t93
        real t933
        real t934
        real t936
        real t94
        real t953
        real t96
        real t960
        real t97
        real t99
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,0,1)
        t10 = rx(t5,j,1,0)
        t12 = -t10 * t9 + t6 * t7
        t13 = 0.1E1 / t12
        t14 = t6 ** 2
        t15 = t9 ** 2
        t16 = t14 + t15
        t17 = t13 * t16
        t18 = rx(i,j,0,0)
        t19 = rx(i,j,1,1)
        t21 = rx(i,j,0,1)
        t22 = rx(i,j,1,0)
        t24 = t18 * t19 - t21 * t22
        t25 = 0.1E1 / t24
        t26 = t18 ** 2
        t27 = t21 ** 2
        t28 = t26 + t27
        t29 = t25 * t28
        t32 = t4 * (t17 / 0.2E1 + t29 / 0.2E1)
        t33 = u(t5,j,n)
        t35 = 0.1E1 / dx
        t36 = (t33 - t1) * t35
        t37 = t32 * t36
        t38 = ut(t5,j,n)
        t45 = sqrt(t16)
        t47 = cc * t13 * t45 * t38
        t49 = dt * cc
        t51 = i + 2
        t52 = rx(t51,j,0,0)
        t53 = rx(t51,j,1,1)
        t55 = rx(t51,j,0,1)
        t56 = rx(t51,j,1,0)
        t59 = 0.1E1 / (t52 * t53 - t55 * t56)
        t60 = t52 ** 2
        t61 = t55 ** 2
        t62 = t60 + t61
        t67 = u(t51,j,n)
        t69 = (t67 - t33) * t35
        t77 = j + 1
        t78 = u(t51,t77,n)
        t80 = 0.1E1 / dy
        t82 = j - 1
        t83 = u(t51,t82,n)
        t93 = t10 * t6 + t7 * t9
        t94 = u(t5,t77,n)
        t96 = (t94 - t33) * t80
        t97 = u(t5,t82,n)
        t99 = (t33 - t97) * t80
        t57 = t4 * t13 * t93
        t103 = t57 * (t96 / 0.2E1 + t99 / 0.2E1)
        t110 = t18 * t22 + t19 * t21
        t111 = u(i,t77,n)
        t113 = (t111 - t1) * t80
        t114 = u(i,t82,n)
        t116 = (t1 - t114) * t80
        t70 = t4 * t25 * t110
        t120 = t70 * (t113 / 0.2E1 + t116 / 0.2E1)
        t123 = (t103 - t120) * t35 / 0.2E1
        t124 = rx(t5,t77,0,0)
        t125 = rx(t5,t77,1,1)
        t127 = rx(t5,t77,0,1)
        t128 = rx(t5,t77,1,0)
        t131 = 0.1E1 / (t124 * t125 - t127 * t128)
        t139 = (t94 - t111) * t35
        t147 = t57 * (t69 / 0.2E1 + t36 / 0.2E1)
        t151 = rx(t5,t82,0,0)
        t152 = rx(t5,t82,1,1)
        t154 = rx(t5,t82,0,1)
        t155 = rx(t5,t82,1,0)
        t158 = 0.1E1 / (t151 * t152 - t154 * t155)
        t166 = (t97 - t114) * t35
        t174 = t128 ** 2
        t175 = t125 ** 2
        t178 = t10 ** 2
        t179 = t7 ** 2
        t181 = t13 * (t178 + t179)
        t186 = t155 ** 2
        t187 = t152 ** 2
        t204 = sqrt(t62)
        t207 = cc * t59 * t204 * ut(t51,j,n)
        t209 = (-t47 + t207) * t35
        t211 = cc * t25
        t212 = sqrt(t28)
        t214 = t211 * t212 * t2
        t216 = (-t214 + t47) * t35
        t217 = t216 / 0.2E1
        t218 = dx ** 2
        t219 = i + 3
        t220 = rx(t219,j,0,0)
        t223 = rx(t219,j,0,1)
        t229 = t220 ** 2
        t230 = t223 ** 2
        t232 = sqrt(t229 + t230)
        t241 = (t209 - t216) * t35
        t244 = i - 1
        t245 = rx(t244,j,0,0)
        t246 = rx(t244,j,1,1)
        t248 = rx(t244,j,0,1)
        t249 = rx(t244,j,1,0)
        t251 = t245 * t246 - t248 * t249
        t252 = 0.1E1 / t251
        t254 = t245 ** 2
        t255 = t248 ** 2
        t256 = t254 + t255
        t257 = sqrt(t256)
        t258 = ut(t244,j,n)
        t260 = cc * t252 * t257 * t258
        t262 = (t214 - t260) * t35
        t264 = (t216 - t262) * t35
        t266 = (t241 - t264) * t35
        t274 = t214 / 0.2E1
        t276 = t252 * t256
        t279 = t4 * (t29 / 0.2E1 + t276 / 0.2E1)
        t280 = u(t244,j,n)
        t282 = (t1 - t280) * t35
        t283 = t279 * t282
        t289 = t245 * t249 + t246 * t248
        t290 = u(t244,t77,n)
        t292 = (t290 - t280) * t80
        t293 = u(t244,t82,n)
        t295 = (t280 - t293) * t80
        t140 = t4 * t252 * t289
        t299 = t140 * (t292 / 0.2E1 + t295 / 0.2E1)
        t302 = (t120 - t299) * t35 / 0.2E1
        t303 = rx(i,t77,0,0)
        t304 = rx(i,t77,1,1)
        t306 = rx(i,t77,0,1)
        t307 = rx(i,t77,1,0)
        t309 = t303 * t304 - t306 * t307
        t310 = 0.1E1 / t309
        t314 = t303 * t307 + t304 * t306
        t316 = (t111 - t290) * t35
        t153 = t4 * t310 * t314
        t320 = t153 * (t139 / 0.2E1 + t316 / 0.2E1)
        t324 = t70 * (t36 / 0.2E1 + t282 / 0.2E1)
        t327 = (t320 - t324) * t80 / 0.2E1
        t328 = rx(i,t82,0,0)
        t329 = rx(i,t82,1,1)
        t331 = rx(i,t82,0,1)
        t332 = rx(i,t82,1,0)
        t334 = t328 * t329 - t331 * t332
        t335 = 0.1E1 / t334
        t339 = t328 * t332 + t329 * t331
        t341 = (t114 - t293) * t35
        t171 = t4 * t335 * t339
        t345 = t171 * (t166 / 0.2E1 + t341 / 0.2E1)
        t348 = (t324 - t345) * t80 / 0.2E1
        t349 = t307 ** 2
        t350 = t304 ** 2
        t351 = t349 + t350
        t352 = t310 * t351
        t353 = t22 ** 2
        t354 = t19 ** 2
        t355 = t353 + t354
        t356 = t25 * t355
        t359 = t4 * (t352 / 0.2E1 + t356 / 0.2E1)
        t360 = t359 * t113
        t361 = t332 ** 2
        t362 = t329 ** 2
        t363 = t361 + t362
        t364 = t335 * t363
        t367 = t4 * (t356 / 0.2E1 + t364 / 0.2E1)
        t368 = t367 * t116
        t374 = ((t37 - t283) * t35 + t123 + t302 + t327 + t348 + (t360 -
     # t368) * t80) * t24 + src(i,j,nComp,n)
        t194 = t49 * t25
        t377 = t194 * t212 * t374 / 0.4E1
        t378 = t262 / 0.2E1
        t379 = i - 2
        t380 = rx(t379,j,0,0)
        t381 = rx(t379,j,1,1)
        t383 = rx(t379,j,0,1)
        t384 = rx(t379,j,1,0)
        t387 = 0.1E1 / (t380 * t381 - t383 * t384)
        t389 = t380 ** 2
        t390 = t383 ** 2
        t391 = t389 + t390
        t392 = sqrt(t391)
        t395 = cc * t387 * t392 * ut(t379,j,n)
        t397 = (-t395 + t260) * t35
        t399 = (t262 - t397) * t35
        t401 = (t264 - t399) * t35
        t408 = dx * (t217 + t378 - t218 * (t266 / 0.2E1 + t401 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t225 = (t38 - t2) * t35
        t272 = t4 * t131 * (t124 * t128 + t125 * t127)
        t288 = t4 * t158 * (t151 * t155 + t152 * t154)
        t409 = t37 + t32 * dt * t225 / 0.2E1 + t47 / 0.2E1 + t49 * t13 *
     # t45 * (((t4 * (t59 * t62 / 0.2E1 + t17 / 0.2E1) * t69 - t37) * t3
     #5 + (t4 * t59 * (t52 * t56 + t53 * t55) * ((t78 - t67) * t80 / 0.2
     #E1 + (t67 - t83) * t80 / 0.2E1) - t103) * t35 / 0.2E1 + t123 + (t2
     #72 * ((t78 - t94) * t35 / 0.2E1 + t139 / 0.2E1) - t147) * t80 / 0.
     #2E1 + (t147 - t288 * ((t83 - t97) * t35 / 0.2E1 + t166 / 0.2E1)) *
     # t80 / 0.2E1 + (t4 * (t131 * (t174 + t175) / 0.2E1 + t181 / 0.2E1)
     # * t96 - t4 * (t181 / 0.2E1 + t158 * (t186 + t187) / 0.2E1) * t99)
     # * t80) * t12 + src(t5,j,nComp,n)) / 0.4E1 - dx * (t209 / 0.2E1 + 
     #t217 - t218 * ((((-t207 + cc / (t220 * rx(t219,j,1,1) - t223 * rx(
     #t219,j,1,0)) * t232 * ut(t219,j,n)) * t35 - t209) * t35 - t241) * 
     #t35 / 0.2E1 + t266 / 0.2E1) / 0.6E1) / 0.4E1 - t274 - t377 - t408
        t410 = dt ** 2
        t413 = t25 * t110
        t416 = t4 * (t13 * t93 / 0.2E1 + t413 / 0.2E1)
        t420 = ut(t5,t77,n)
        t423 = ut(t5,t82,n)
        t426 = ut(i,t77,n)
        t428 = (t426 - t2) * t80
        t429 = ut(i,t82,n)
        t431 = (t2 - t429) * t80
        t437 = t416 * (t96 / 0.4E1 + t99 / 0.4E1 + t113 / 0.4E1 + t116 /
     # 0.4E1) + t416 * dt * ((t420 - t38) * t80 / 0.4E1 + (t38 - t423) *
     # t80 / 0.4E1 + t428 / 0.4E1 + t431 / 0.4E1) / 0.2E1
        t450 = u(t379,j,n)
        t452 = (t280 - t450) * t35
        t460 = u(t379,t77,n)
        t463 = u(t379,t82,n)
        t473 = rx(t244,t77,0,0)
        t474 = rx(t244,t77,1,1)
        t476 = rx(t244,t77,0,1)
        t477 = rx(t244,t77,1,0)
        t480 = 0.1E1 / (t473 * t474 - t476 * t477)
        t494 = t140 * (t282 / 0.2E1 + t452 / 0.2E1)
        t498 = rx(t244,t82,0,0)
        t499 = rx(t244,t82,1,1)
        t501 = rx(t244,t82,0,1)
        t502 = rx(t244,t82,1,0)
        t505 = 0.1E1 / (t498 * t499 - t501 * t502)
        t519 = t477 ** 2
        t520 = t474 ** 2
        t523 = t249 ** 2
        t524 = t246 ** 2
        t526 = t252 * (t523 + t524)
        t531 = t502 ** 2
        t532 = t499 ** 2
        t549 = i - 3
        t550 = rx(t549,j,0,0)
        t553 = rx(t549,j,0,1)
        t559 = t550 ** 2
        t560 = t553 ** 2
        t562 = sqrt(t559 + t560)
        t453 = (t2 - t258) * t35
        t489 = t4 * t480 * (t473 * t477 + t474 * t476)
        t503 = t4 * t505 * (t498 * t502 + t499 * t501)
        t579 = t283 + t279 * dt * t453 / 0.2E1 + t274 + t377 - t408 - t2
     #60 / 0.2E1 - t49 * t252 * t257 * (((t283 - t4 * (t387 * t391 / 0.2
     #E1 + t276 / 0.2E1) * t452) * t35 + t302 + (t299 - t4 * t387 * (t38
     #0 * t384 + t381 * t383) * ((t460 - t450) * t80 / 0.2E1 + (t450 - t
     #463) * t80 / 0.2E1)) * t35 / 0.2E1 + (t489 * (t316 / 0.2E1 + (t290
     # - t460) * t35 / 0.2E1) - t494) * t80 / 0.2E1 + (t494 - t503 * (t3
     #41 / 0.2E1 + (t293 - t463) * t35 / 0.2E1)) * t80 / 0.2E1 + (t4 * (
     #t480 * (t519 + t520) / 0.2E1 + t526 / 0.2E1) * t292 - t4 * (t526 /
     # 0.2E1 + t505 * (t531 + t532) / 0.2E1) * t295) * t80) * t251 + src
     #(t244,j,nComp,n)) / 0.4E1 - dx * (t378 + t397 / 0.2E1 - t218 * (t4
     #01 / 0.2E1 + (t399 - (t397 - (-cc / (t550 * rx(t549,j,1,1) - t553 
     #* rx(t549,j,1,0)) * t562 * ut(t549,j,n) + t395) * t35) * t35) * t3
     #5 / 0.2E1) / 0.6E1) / 0.4E1
        t584 = t4 * (t252 * t289 / 0.2E1 + t413 / 0.2E1)
        t588 = ut(t244,t77,n)
        t591 = ut(t244,t82,n)
        t599 = t584 * (t113 / 0.4E1 + t116 / 0.4E1 + t292 / 0.4E1 + t295
     # / 0.4E1) + t584 * dt * (t428 / 0.4E1 + t431 / 0.4E1 + (t588 - t25
     #8) * t80 / 0.4E1 + (t258 - t591) * t80 / 0.4E1) / 0.2E1
        t608 = t4 * (t310 * t314 / 0.2E1 + t413 / 0.2E1)
        t616 = t225
        t617 = t453
        t623 = t608 * (t139 / 0.4E1 + t316 / 0.4E1 + t36 / 0.4E1 + t282 
     #/ 0.4E1) + t608 * dt * ((t420 - t426) * t35 / 0.4E1 + (t426 - t588
     #) * t35 / 0.4E1 + t616 / 0.4E1 + t617 / 0.4E1) / 0.2E1
        t630 = sqrt(t351)
        t632 = cc * t310 * t630 * t426
        t635 = t124 ** 2
        t636 = t127 ** 2
        t639 = t303 ** 2
        t640 = t306 ** 2
        t642 = t310 * (t639 + t640)
        t647 = t473 ** 2
        t648 = t476 ** 2
        t657 = j + 2
        t658 = u(t5,t657,n)
        t665 = u(i,t657,n)
        t667 = (t665 - t111) * t80
        t671 = t153 * (t667 / 0.2E1 + t113 / 0.2E1)
        t675 = u(t244,t657,n)
        t685 = rx(i,t657,0,0)
        t686 = rx(i,t657,1,1)
        t688 = rx(i,t657,0,1)
        t689 = rx(i,t657,1,0)
        t692 = 0.1E1 / (t685 * t686 - t688 * t689)
        t708 = t689 ** 2
        t709 = t686 ** 2
        t710 = t708 + t709
        t726 = sqrt(t710)
        t729 = cc * t692 * t726 * ut(i,t657,n)
        t731 = (-t632 + t729) * t80
        t733 = sqrt(t355)
        t735 = t211 * t733 * t2
        t737 = (-t735 + t632) * t80
        t738 = t737 / 0.2E1
        t739 = dy ** 2
        t740 = j + 3
        t742 = rx(i,t740,1,1)
        t745 = rx(i,t740,1,0)
        t750 = t745 ** 2
        t751 = t742 ** 2
        t753 = sqrt(t750 + t751)
        t762 = (t731 - t737) * t80
        t766 = sqrt(t363)
        t768 = cc * t335 * t766 * t429
        t770 = (t735 - t768) * t80
        t772 = (t737 - t770) * t80
        t774 = (t762 - t772) * t80
        t782 = t735 / 0.2E1
        t786 = t194 * t733 * t374 / 0.4E1
        t787 = t770 / 0.2E1
        t788 = j - 2
        t789 = rx(i,t788,0,0)
        t790 = rx(i,t788,1,1)
        t792 = rx(i,t788,0,1)
        t793 = rx(i,t788,1,0)
        t796 = 0.1E1 / (t789 * t790 - t792 * t793)
        t798 = t793 ** 2
        t799 = t790 ** 2
        t800 = t798 + t799
        t801 = sqrt(t800)
        t804 = cc * t796 * t801 * ut(i,t788,n)
        t806 = (-t804 + t768) * t80
        t808 = (t770 - t806) * t80
        t810 = (t772 - t808) * t80
        t817 = dy * (t738 + t787 - t739 * (t774 / 0.2E1 + t810 / 0.2E1) 
     #/ 0.6E1) / 0.4E1
        t818 = t360 + t359 * dt * t428 / 0.2E1 + t632 / 0.2E1 + t49 * t3
     #10 * t630 * (((t4 * (t131 * (t635 + t636) / 0.2E1 + t642 / 0.2E1) 
     #* t139 - t4 * (t642 / 0.2E1 + t480 * (t647 + t648) / 0.2E1) * t316
     #) * t35 + (t272 * ((t658 - t94) * t80 / 0.2E1 + t96 / 0.2E1) - t67
     #1) * t35 / 0.2E1 + (t671 - t489 * ((t675 - t290) * t80 / 0.2E1 + t
     #292 / 0.2E1)) * t35 / 0.2E1 + (t4 * t692 * (t685 * t689 + t686 * t
     #688) * ((t658 - t665) * t35 / 0.2E1 + (t665 - t675) * t35 / 0.2E1)
     # - t320) * t80 / 0.2E1 + t327 + (t4 * (t692 * t710 / 0.2E1 + t352 
     #/ 0.2E1) * t667 - t360) * t80) * t309 + src(i,t77,nComp,n)) / 0.4E
     #1 - dy * (t731 / 0.2E1 + t738 - t739 * ((((-t729 + cc / (t742 * rx
     #(i,t740,0,0) - t745 * rx(i,t740,0,1)) * t753 * ut(i,t740,n)) * t80
     # - t731) * t80 - t762) * t80 / 0.2E1 + t774 / 0.2E1) / 0.6E1) / 0.
     #4E1 - t782 - t786 - t817
        t823 = t4 * (t335 * t339 / 0.2E1 + t413 / 0.2E1)
        t836 = t823 * (t36 / 0.4E1 + t282 / 0.4E1 + t166 / 0.4E1 + t341 
     #/ 0.4E1) + t823 * dt * (t616 / 0.4E1 + t617 / 0.4E1 + (t423 - t429
     #) * t35 / 0.4E1 + (t429 - t591) * t35 / 0.4E1) / 0.2E1
        t844 = t151 ** 2
        t845 = t154 ** 2
        t848 = t328 ** 2
        t849 = t331 ** 2
        t851 = t335 * (t848 + t849)
        t856 = t498 ** 2
        t857 = t501 ** 2
        t866 = u(t5,t788,n)
        t873 = u(i,t788,n)
        t875 = (t114 - t873) * t80
        t879 = t171 * (t116 / 0.2E1 + t875 / 0.2E1)
        t883 = u(t244,t788,n)
        t923 = j - 3
        t925 = rx(i,t923,1,1)
        t928 = rx(i,t923,1,0)
        t933 = t928 ** 2
        t934 = t925 ** 2
        t936 = sqrt(t933 + t934)
        t953 = t368 + t367 * dt * t431 / 0.2E1 + t782 + t786 - t817 - t7
     #68 / 0.2E1 - t49 * t335 * t766 * (((t4 * (t158 * (t844 + t845) / 0
     #.2E1 + t851 / 0.2E1) * t166 - t4 * (t851 / 0.2E1 + t505 * (t856 + 
     #t857) / 0.2E1) * t341) * t35 + (t288 * (t99 / 0.2E1 + (t97 - t866)
     # * t80 / 0.2E1) - t879) * t35 / 0.2E1 + (t879 - t503 * (t295 / 0.2
     #E1 + (t293 - t883) * t80 / 0.2E1)) * t35 / 0.2E1 + t348 + (t345 - 
     #t4 * t796 * (t789 * t793 + t790 * t792) * ((t866 - t873) * t35 / 0
     #.2E1 + (t873 - t883) * t35 / 0.2E1)) * t80 / 0.2E1 + (t368 - t4 * 
     #(t796 * t800 / 0.2E1 + t364 / 0.2E1) * t875) * t80) * t334 + src(i
     #,t82,nComp,n)) / 0.4E1 - dy * (t787 + t806 / 0.2E1 - t739 * (t810 
     #/ 0.2E1 + (t808 - (t806 - (-cc / (t925 * rx(i,t923,0,0) - t928 * r
     #x(i,t923,0,1)) * t936 * ut(i,t923,n) + t804) * t80) * t80) * t80 /
     # 0.2E1) / 0.6E1) / 0.4E1
        t960 = src(i,j,nComp,n + 1)

        unew(i,j) = t1 + dt * t2 + (t409 * t410 / 0.2E1 + t437 * t4
     #10 / 0.2E1 - t579 * t410 / 0.2E1 - t599 * t410 / 0.2E1) * t24 * t3
     #5 + (t623 * t410 / 0.2E1 + t818 * t410 / 0.2E1 - t836 * t410 / 0.2
     #E1 - t953 * t410 / 0.2E1) * t24 * t80 + t960 * t410 / 0.2E1

        utnew(i,j) = t
     #2 + (dt * t409 + dt * t437 - dt * t579 - dt * t599) * t24 * t35 + 
     #(dt * t623 + dt * t818 - dt * t836 - dt * t953) * t24 * t80 + t960
     # * dt

        return
      end
