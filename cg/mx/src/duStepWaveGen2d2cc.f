      subroutine duStepWaveGen2d2cc( 
     *   nd1a,nd1b,nd2a,nd2b,
     *   n1a,n1b,n2a,n2b,
     *   u,ut,unew,utnew,rx,
     *   dx,dy,dt,cc,
     *   i,j,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b
      integer n1a,n1b,n2a,n2b
      integer i,j,n,ix,jy

      real u    (nd1a:nd1b,nd2a:nd2b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,*)
      real unew (nd1a:nd1b,nd2a:nd2b)
      real utnew(nd1a:nd1b,nd2a:nd2b)
      real rx   (nd1a:nd1b,nd2a:nd2b,0:1,0:1)
      real dx,dy,dt,cc
c
c.. generated code to follow
c
        real t1
        real t10
        real t102
        real t109
        real t110
        real t112
        real t113
        real t115
        real t119
        real t121
        real t122
        real t123
        real t124
        real t126
        real t127
        real t13
        real t130
        real t138
        real t14
        real t141
        real t146
        real t15
        real t150
        real t151
        real t153
        real t154
        real t157
        real t16
        real t162
        real t165
        real t17
        real t173
        real t174
        real t177
        real t178
        real t18
        real t180
        real t185
        real t186
        real t19
        real t192
        real t2
        real t200
        real t206
        real t207
        real t209
        real t21
        real t211
        real t216
        integer t217
        real t218
        real t219
        real t22
        real t221
        real t222
        real t225
        real t226
        real t227
        real t228
        real t229
        real t232
        real t233
        real t235
        real t236
        real t24
        real t240
        real t242
        real t243
        real t245
        real t246
        real t248
        real t25
        real t252
        real t255
        real t256
        real t257
        real t258
        real t259
        real t26
        real t260
        real t263
        real t267
        real t269
        real t27
        real t273
        real t277
        real t28
        real t280
        real t281
        real t282
        real t284
        real t285
        real t288
        real t29
        real t292
        real t294
        real t298
        real t301
        real t302
        real t303
        real t304
        real t305
        real t306
        real t307
        real t308
        real t309
        real t312
        real t313
        real t314
        real t315
        real t316
        real t317
        real t32
        real t320
        real t321
        real t324
        real t327
        real t329
        real t33
        real t330
        real t332
        real t334
        real t338
        real t339
        real t340
        real t343
        real t346
        real t35
        real t350
        real t353
        real t356
        real t358
        real t359
        real t36
        real t361
        real t367
        real t37
        integer t375
        real t376
        real t377
        real t379
        real t38
        real t380
        real t383
        real t384
        real t385
        real t386
        real t391
        real t393
        real t394
        real t4
        real t401
        real t404
        real t414
        real t415
        real t417
        real t418
        real t421
        real t430
        real t435
        real t439
        real t440
        real t442
        real t443
        real t444
        real t446
        real t45
        real t460
        real t461
        real t464
        real t465
        real t467
        real t47
        real t472
        real t473
        real t487
        real t49
        real t497
        integer t5
        integer t50
        real t502
        real t506
        real t509
        real t51
        real t517
        real t52
        real t526
        real t534
        real t535
        real t54
        real t541
        real t548
        real t55
        real t550
        real t552
        real t553
        real t556
        real t557
        real t559
        real t564
        real t565
        real t57
        integer t574
        real t575
        real t58
        real t582
        real t584
        real t588
        real t59
        real t592
        real t6
        real t60
        real t602
        real t603
        real t605
        real t606
        real t609
        real t61
        real t625
        real t626
        real t627
        real t640
        real t646
        real t648
        real t650
        real t655
        real t658
        real t66
        real t660
        real t662
        real t664
        real t668
        real t669
        real t674
        real t68
        real t687
        real t694
        real t695
        real t698
        real t699
        real t7
        real t70
        real t701
        real t706
        real t707
        integer t716
        real t717
        real t724
        real t726
        real t730
        real t734
        real t744
        real t745
        real t747
        real t748
        real t751
        integer t76
        real t767
        real t768
        real t769
        real t77
        real t782
        real t79
        real t792
        integer t81
        real t82
        real t9
        real t92
        real t93
        real t95
        real t96
        real t98
        t1 = u(i,j,n)
        t2 = ut(i,j,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,0,0)
        t7 = rx(t5,j,1,1)
        t9 = rx(t5,j,0,1)
        t10 = rx(t5,j,1,0)
        t13 = 0.1E1 / (-t10 * t9 + t6 * t7)
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
        t50 = i + 2
        t51 = rx(t50,j,0,0)
        t52 = rx(t50,j,1,1)
        t54 = rx(t50,j,0,1)
        t55 = rx(t50,j,1,0)
        t58 = 0.1E1 / (t51 * t52 - t54 * t55)
        t59 = t51 ** 2
        t60 = t54 ** 2
        t61 = t59 + t60
        t66 = u(t50,j,n)
        t68 = (t66 - t33) * t35
        t76 = j + 1
        t77 = u(t50,t76,n)
        t79 = 0.1E1 / dy
        t81 = j - 1
        t82 = u(t50,t81,n)
        t92 = t10 * t6 + t7 * t9
        t93 = u(t5,t76,n)
        t95 = (t93 - t33) * t79
        t96 = u(t5,t81,n)
        t98 = (t33 - t96) * t79
        t57 = t4 * t13 * t92
        t102 = t57 * (t95 / 0.2E1 + t98 / 0.2E1)
        t109 = t18 * t22 + t19 * t21
        t110 = u(i,t76,n)
        t112 = (t110 - t1) * t79
        t113 = u(i,t81,n)
        t115 = (t1 - t113) * t79
        t70 = t4 * t25 * t109
        t119 = t70 * (t112 / 0.2E1 + t115 / 0.2E1)
        t122 = (t102 - t119) * t35 / 0.2E1
        t123 = rx(t5,t76,0,0)
        t124 = rx(t5,t76,1,1)
        t126 = rx(t5,t76,0,1)
        t127 = rx(t5,t76,1,0)
        t130 = 0.1E1 / (t123 * t124 - t126 * t127)
        t138 = (t93 - t110) * t35
        t146 = t57 * (t68 / 0.2E1 + t36 / 0.2E1)
        t150 = rx(t5,t81,0,0)
        t151 = rx(t5,t81,1,1)
        t153 = rx(t5,t81,0,1)
        t154 = rx(t5,t81,1,0)
        t157 = 0.1E1 / (t150 * t151 - t153 * t154)
        t165 = (t96 - t113) * t35
        t173 = t127 ** 2
        t174 = t124 ** 2
        t177 = t10 ** 2
        t178 = t7 ** 2
        t180 = t13 * (t177 + t178)
        t185 = t154 ** 2
        t186 = t151 ** 2
        t200 = sqrt(t61)
        t206 = cc * t25
        t207 = sqrt(t28)
        t209 = t206 * t207 * t2
        t211 = (-t209 + t47) * t35
        t216 = t209 / 0.2E1
        t217 = i - 1
        t218 = rx(t217,j,0,0)
        t219 = rx(t217,j,1,1)
        t221 = rx(t217,j,0,1)
        t222 = rx(t217,j,1,0)
        t225 = 0.1E1 / (t218 * t219 - t221 * t222)
        t226 = t218 ** 2
        t227 = t221 ** 2
        t228 = t226 + t227
        t229 = t225 * t228
        t232 = t4 * (t29 / 0.2E1 + t229 / 0.2E1)
        t233 = u(t217,j,n)
        t235 = (t1 - t233) * t35
        t236 = t232 * t235
        t242 = t218 * t222 + t219 * t221
        t243 = u(t217,t76,n)
        t245 = (t243 - t233) * t79
        t246 = u(t217,t81,n)
        t248 = (t233 - t246) * t79
        t121 = t4 * t225 * t242
        t252 = t121 * (t245 / 0.2E1 + t248 / 0.2E1)
        t255 = (t119 - t252) * t35 / 0.2E1
        t256 = rx(i,t76,0,0)
        t257 = rx(i,t76,1,1)
        t259 = rx(i,t76,0,1)
        t260 = rx(i,t76,1,0)
        t263 = 0.1E1 / (t256 * t257 - t259 * t260)
        t267 = t256 * t260 + t257 * t259
        t269 = (t110 - t243) * t35
        t141 = t4 * t263 * t267
        t273 = t141 * (t138 / 0.2E1 + t269 / 0.2E1)
        t277 = t70 * (t36 / 0.2E1 + t235 / 0.2E1)
        t280 = (t273 - t277) * t79 / 0.2E1
        t281 = rx(i,t81,0,0)
        t282 = rx(i,t81,1,1)
        t284 = rx(i,t81,0,1)
        t285 = rx(i,t81,1,0)
        t288 = 0.1E1 / (t281 * t282 - t284 * t285)
        t292 = t281 * t285 + t282 * t284
        t294 = (t113 - t246) * t35
        t162 = t4 * t288 * t292
        t298 = t162 * (t165 / 0.2E1 + t294 / 0.2E1)
        t301 = (t277 - t298) * t79 / 0.2E1
        t302 = t260 ** 2
        t303 = t257 ** 2
        t304 = t302 + t303
        t305 = t263 * t304
        t306 = t22 ** 2
        t307 = t19 ** 2
        t308 = t306 + t307
        t309 = t25 * t308
        t312 = t4 * (t305 / 0.2E1 + t309 / 0.2E1)
        t313 = t312 * t112
        t314 = t285 ** 2
        t315 = t282 ** 2
        t316 = t314 + t315
        t317 = t288 * t316
        t320 = t4 * (t309 / 0.2E1 + t317 / 0.2E1)
        t321 = t320 * t115
        t324 = (t37 - t236) * t35 + t122 + t255 + t280 + t301 + (t313 - 
     #t321) * t79
        t327 = t49 * t207 * t324 / 0.4E1
        t329 = sqrt(t228)
        t330 = ut(t217,j,n)
        t332 = cc * t225 * t329 * t330
        t334 = (t209 - t332) * t35
        t338 = dx * (t211 / 0.2E1 + t334 / 0.2E1) / 0.4E1
        t192 = (t38 - t2) * t35
        t240 = t4 * t130 * (t123 * t127 + t124 * t126)
        t258 = t4 * t157 * (t150 * t154 + t151 * t153)
        t339 = t37 + t32 * dt * t192 / 0.2E1 + t47 / 0.2E1 + t49 * t45 *
     # ((t4 * (t58 * t61 / 0.2E1 + t17 / 0.2E1) * t68 - t37) * t35 + (t4
     # * t58 * (t51 * t55 + t52 * t54) * ((t77 - t66) * t79 / 0.2E1 + (t
     #66 - t82) * t79 / 0.2E1) - t102) * t35 / 0.2E1 + t122 + (t240 * ((
     #t77 - t93) * t35 / 0.2E1 + t138 / 0.2E1) - t146) * t79 / 0.2E1 + (
     #t146 - t258 * ((t82 - t96) * t35 / 0.2E1 + t165 / 0.2E1)) * t79 / 
     #0.2E1 + (t4 * (t130 * (t173 + t174) / 0.2E1 + t180 / 0.2E1) * t95 
     #- t4 * (t180 / 0.2E1 + t157 * (t185 + t186) / 0.2E1) * t98) * t79)
     # / 0.4E1 - dx * ((cc * t200 * t58 * ut(t50,j,n) - t47) * t35 / 0.2
     #E1 + t211 / 0.2E1) / 0.4E1 - t216 - t327 - t338
        t340 = dt ** 2
        t343 = t25 * t109
        t346 = t4 * (t13 * t92 / 0.2E1 + t343 / 0.2E1)
        t350 = ut(t5,t76,n)
        t353 = ut(t5,t81,n)
        t356 = ut(i,t76,n)
        t358 = (t356 - t2) * t79
        t359 = ut(i,t81,n)
        t361 = (t2 - t359) * t79
        t367 = t346 * (t95 / 0.4E1 + t98 / 0.4E1 + t112 / 0.4E1 + t115 /
     # 0.4E1) + t346 * dt * ((t350 - t38) * t79 / 0.4E1 + (t38 - t353) *
     # t79 / 0.4E1 + t358 / 0.4E1 + t361 / 0.4E1) / 0.2E1
        t375 = i - 2
        t376 = rx(t375,j,0,0)
        t377 = rx(t375,j,1,1)
        t379 = rx(t375,j,0,1)
        t380 = rx(t375,j,1,0)
        t383 = 0.1E1 / (t376 * t377 - t379 * t380)
        t384 = t376 ** 2
        t385 = t379 ** 2
        t386 = t384 + t385
        t391 = u(t375,j,n)
        t393 = (t233 - t391) * t35
        t401 = u(t375,t76,n)
        t404 = u(t375,t81,n)
        t414 = rx(t217,t76,0,0)
        t415 = rx(t217,t76,1,1)
        t417 = rx(t217,t76,0,1)
        t418 = rx(t217,t76,1,0)
        t421 = 0.1E1 / (t414 * t415 - t417 * t418)
        t435 = t121 * (t235 / 0.2E1 + t393 / 0.2E1)
        t439 = rx(t217,t81,0,0)
        t440 = rx(t217,t81,1,1)
        t442 = rx(t217,t81,0,1)
        t443 = rx(t217,t81,1,0)
        t446 = 0.1E1 / (t439 * t440 - t442 * t443)
        t460 = t418 ** 2
        t461 = t415 ** 2
        t464 = t222 ** 2
        t465 = t219 ** 2
        t467 = t225 * (t464 + t465)
        t472 = t443 ** 2
        t473 = t440 ** 2
        t487 = sqrt(t386)
        t394 = (t2 - t330) * t35
        t430 = t4 * t421 * (t414 * t418 + t415 * t417)
        t444 = t4 * t446 * (t439 * t443 + t440 * t442)
        t497 = t236 + t232 * dt * t394 / 0.2E1 + t216 + t327 - t338 - t3
     #32 / 0.2E1 - t49 * t329 * ((t236 - t4 * (t383 * t386 / 0.2E1 + t22
     #9 / 0.2E1) * t393) * t35 + t255 + (t252 - t4 * t383 * (t376 * t380
     # + t377 * t379) * ((t401 - t391) * t79 / 0.2E1 + (t391 - t404) * t
     #79 / 0.2E1)) * t35 / 0.2E1 + (t430 * (t269 / 0.2E1 + (t243 - t401)
     # * t35 / 0.2E1) - t435) * t79 / 0.2E1 + (t435 - t444 * (t294 / 0.2
     #E1 + (t246 - t404) * t35 / 0.2E1)) * t79 / 0.2E1 + (t4 * (t421 * (
     #t460 + t461) / 0.2E1 + t467 / 0.2E1) * t245 - t4 * (t467 / 0.2E1 +
     # t446 * (t472 + t473) / 0.2E1) * t248) * t79) / 0.4E1 - dx * (t334
     # / 0.2E1 + (-cc * t383 * t487 * ut(t375,j,n) + t332) * t35 / 0.2E1
     #) / 0.4E1
        t502 = t4 * (t225 * t242 / 0.2E1 + t343 / 0.2E1)
        t506 = ut(t217,t76,n)
        t509 = ut(t217,t81,n)
        t517 = t502 * (t112 / 0.4E1 + t115 / 0.4E1 + t245 / 0.4E1 + t248
     # / 0.4E1) + t502 * dt * (t358 / 0.4E1 + t361 / 0.4E1 + (t506 - t33
     #0) * t79 / 0.4E1 + (t330 - t509) * t79 / 0.4E1) / 0.2E1
        t526 = t4 * (t263 * t267 / 0.2E1 + t343 / 0.2E1)
        t534 = t192
        t535 = t394
        t541 = t526 * (t138 / 0.4E1 + t269 / 0.4E1 + t36 / 0.4E1 + t235 
     #/ 0.4E1) + t526 * dt * ((t350 - t356) * t35 / 0.4E1 + (t356 - t506
     #) * t35 / 0.4E1 + t534 / 0.4E1 + t535 / 0.4E1) / 0.2E1
        t548 = sqrt(t304)
        t550 = cc * t263 * t548 * t356
        t552 = t123 ** 2
        t553 = t126 ** 2
        t556 = t256 ** 2
        t557 = t259 ** 2
        t559 = t263 * (t556 + t557)
        t564 = t414 ** 2
        t565 = t417 ** 2
        t574 = j + 2
        t575 = u(t5,t574,n)
        t582 = u(i,t574,n)
        t584 = (t582 - t110) * t79
        t588 = t141 * (t584 / 0.2E1 + t112 / 0.2E1)
        t592 = u(t217,t574,n)
        t602 = rx(i,t574,0,0)
        t603 = rx(i,t574,1,1)
        t605 = rx(i,t574,0,1)
        t606 = rx(i,t574,1,0)
        t609 = 0.1E1 / (t602 * t603 - t605 * t606)
        t625 = t606 ** 2
        t626 = t603 ** 2
        t627 = t625 + t626
        t640 = sqrt(t627)
        t646 = sqrt(t308)
        t648 = t206 * t646 * t2
        t650 = (-t648 + t550) * t79
        t655 = t648 / 0.2E1
        t658 = t49 * t646 * t324 / 0.4E1
        t660 = sqrt(t316)
        t662 = cc * t288 * t660 * t359
        t664 = (t648 - t662) * t79
        t668 = dy * (t650 / 0.2E1 + t664 / 0.2E1) / 0.4E1
        t669 = t313 + t312 * dt * t358 / 0.2E1 + t550 / 0.2E1 + t49 * t5
     #48 * ((t4 * (t130 * (t552 + t553) / 0.2E1 + t559 / 0.2E1) * t138 -
     # t4 * (t559 / 0.2E1 + t421 * (t564 + t565) / 0.2E1) * t269) * t35 
     #+ (t240 * ((t575 - t93) * t79 / 0.2E1 + t95 / 0.2E1) - t588) * t35
     # / 0.2E1 + (t588 - t430 * ((t592 - t243) * t79 / 0.2E1 + t245 / 0.
     #2E1)) * t35 / 0.2E1 + (t4 * t609 * (t602 * t606 + t603 * t605) * (
     #(t575 - t582) * t35 / 0.2E1 + (t582 - t592) * t35 / 0.2E1) - t273)
     # * t79 / 0.2E1 + t280 + (t4 * (t609 * t627 / 0.2E1 + t305 / 0.2E1)
     # * t584 - t313) * t79) / 0.4E1 - dy * ((cc * t609 * t640 * ut(i,t5
     #74,n) - t550) * t79 / 0.2E1 + t650 / 0.2E1) / 0.4E1 - t655 - t658 
     #- t668
        t674 = t4 * (t288 * t292 / 0.2E1 + t343 / 0.2E1)
        t687 = t674 * (t36 / 0.4E1 + t235 / 0.4E1 + t165 / 0.4E1 + t294 
     #/ 0.4E1) + t674 * dt * (t534 / 0.4E1 + t535 / 0.4E1 + (t353 - t359
     #) * t35 / 0.4E1 + (t359 - t509) * t35 / 0.4E1) / 0.2E1
        t694 = t150 ** 2
        t695 = t153 ** 2
        t698 = t281 ** 2
        t699 = t284 ** 2
        t701 = t288 * (t698 + t699)
        t706 = t439 ** 2
        t707 = t442 ** 2
        t716 = j - 2
        t717 = u(t5,t716,n)
        t724 = u(i,t716,n)
        t726 = (t113 - t724) * t79
        t730 = t162 * (t115 / 0.2E1 + t726 / 0.2E1)
        t734 = u(t217,t716,n)
        t744 = rx(i,t716,0,0)
        t745 = rx(i,t716,1,1)
        t747 = rx(i,t716,0,1)
        t748 = rx(i,t716,1,0)
        t751 = 0.1E1 / (t744 * t745 - t747 * t748)
        t767 = t748 ** 2
        t768 = t745 ** 2
        t769 = t767 + t768
        t782 = sqrt(t769)
        t792 = t321 + t320 * dt * t361 / 0.2E1 + t655 + t658 - t668 - t6
     #62 / 0.2E1 - t49 * t660 * ((t4 * (t157 * (t694 + t695) / 0.2E1 + t
     #701 / 0.2E1) * t165 - t4 * (t701 / 0.2E1 + t446 * (t706 + t707) / 
     #0.2E1) * t294) * t35 + (t258 * (t98 / 0.2E1 + (t96 - t717) * t79 /
     # 0.2E1) - t730) * t35 / 0.2E1 + (t730 - t444 * (t248 / 0.2E1 + (t2
     #46 - t734) * t79 / 0.2E1)) * t35 / 0.2E1 + t301 + (t298 - t4 * t75
     #1 * (t744 * t748 + t745 * t747) * ((t717 - t724) * t35 / 0.2E1 + (
     #t724 - t734) * t35 / 0.2E1)) * t79 / 0.2E1 + (t321 - t4 * (t751 * 
     #t769 / 0.2E1 + t317 / 0.2E1) * t726) * t79) / 0.4E1 - dy * (t664 /
     # 0.2E1 + (-cc * t751 * t782 * ut(i,t716,n) + t662) * t79 / 0.2E1) 
     #/ 0.4E1

        unew(i,j) = t1 + dt * t2 + (t339 * t340 / 0.2E1 + t367 * t3
     #40 / 0.2E1 - t497 * t340 / 0.2E1 - t517 * t340 / 0.2E1) * t24 * t3
     #5 + (t541 * t340 / 0.2E1 + t669 * t340 / 0.2E1 - t687 * t340 / 0.2
     #E1 - t792 * t340 / 0.2E1) * t24 * t79

        utnew(i,j) = t2 + (dt * t339 + dt * 
     #t367 - dt * t497 - dt * t517) * t24 * t35 + (dt * t541 + dt * t669
     # - dt * t687 - dt * t792) * t24 * t79
        
        return
      end
