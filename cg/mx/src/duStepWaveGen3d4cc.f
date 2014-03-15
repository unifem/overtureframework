      subroutine duStepWaveGen3d4cc( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,rx,
     *   dx,dy,dz,dt,cc,
     *   i,j,k,n )

      implicit none
c
c.. declarations of incoming variables      
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      integer n1a,n1b,n2a,n2b,n3a,n3b
      integer i,j,k,n

      real u    (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real ut   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,*)
      real unew (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real utnew(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real rx   (nd1a:nd1b,nd2a:nd2b,nd3a:nd3b,0:2,0:2)
      real dx,dy,dz,dt,cc
c
c.. generated code to follow
c
        real t1
        real t100
        real t10000
        real t10001
        real t10002
        real t10003
        real t10004
        real t10005
        real t10006
        real t10008
        real t1001
        real t10010
        real t10012
        real t10013
        real t10015
        real t10017
        real t10019
        real t1002
        real t10020
        real t10022
        real t10024
        real t10026
        real t10027
        real t10029
        real t10031
        real t10032
        real t10033
        real t10034
        real t10036
        real t10038
        real t1004
        real t10040
        real t10042
        real t10044
        real t10045
        real t10047
        real t10049
        real t1005
        real t10051
        real t10052
        real t10053
        real t10054
        real t10055
        real t10056
        real t10057
        real t10058
        real t10060
        real t10066
        real t1007
        real t10070
        real t10073
        real t10076
        real t10078
        real t10080
        real t10087
        real t10089
        real t1009
        real t10090
        real t10093
        real t10094
        real t10100
        real t1011
        real t10111
        real t10112
        real t10113
        real t10116
        real t10117
        real t10118
        real t10119
        real t10120
        real t10124
        real t10126
        real t1013
        real t10130
        real t10133
        real t10134
        real t10142
        real t10146
        real t1015
        real t10153
        real t10158
        real t1016
        real t10163
        real t10166
        real t10169
        real t1017
        real t10189
        real t1019
        real t102
        real t1020
        real t10212
        real t1022
        real t10234
        real t1024
        real t10247
        real t1026
        integer t10261
        real t10262
        real t10264
        real t10272
        real t10273
        real t10275
        real t10277
        real t10279
        real t1028
        real t10281
        real t10283
        real t10285
        real t10288
        real t1029
        real t10294
        real t10295
        real t10296
        real t10297
        real t10299
        real t10302
        real t10305
        real t1031
        real t10314
        real t10320
        real t10324
        real t10325
        real t1033
        real t10336
        real t10340
        real t10342
        real t1035
        real t10356
        real t10359
        real t10361
        real t1037
        real t1039
        real t104
        real t10409
        real t1041
        real t10413
        real t10415
        real t1042
        real t10425
        real t10428
        real t10430
        real t1044
        real t10444
        real t10448
        real t10457
        real t1046
        real t10461
        real t10471
        real t1048
        real t10490
        real t10496
        real t10498
        real t1050
        real t10504
        real t10507
        real t10511
        real t10518
        real t10519
        real t1052
        real t10520
        real t10526
        real t1053
        real t10533
        real t10536
        real t10540
        real t10546
        real t1055
        real t10558
        real t10565
        real t1057
        real t10576
        real t10578
        real t10581
        real t10583
        real t10587
        real t1059
        real t10594
        real t10595
        real t10598
        real t106
        real t10606
        real t1061
        real t10621
        real t10626
        real t1063
        real t10637
        real t10640
        real t10644
        real t10648
        real t1065
        real t10652
        real t10655
        real t10658
        real t1066
        real t10662
        real t10666
        real t10675
        real t10677
        real t1068
        real t10683
        real t10687
        real t10690
        real t10692
        real t10698
        real t1070
        real t10707
        real t10709
        real t10715
        real t10717
        real t10719
        real t1072
        real t10722
        real t10724
        real t10730
        real t10739
        real t1074
        real t10740
        real t10742
        real t10746
        real t10748
        real t10753
        real t10758
        real t10759
        real t1076
        real t10761
        real t10763
        real t10766
        real t10767
        real t10769
        real t1077
        real t10774
        real t1078
        real t10787
        real t10789
        real t1079
        real t108
        real t10803
        real t10806
        real t10808
        real t1081
        real t1082
        real t10823
        real t10828
        real t1083
        real t1084
        real t10851
        real t1086
        real t10860
        real t10865
        real t1087
        real t10888
        real t1089
        real t10898
        real t1090
        real t10908
        real t10912
        real t1092
        real t10929
        real t10932
        real t10937
        real t1094
        real t10946
        real t10951
        real t1096
        real t10960
        real t10961
        real t10965
        real t10967
        real t10970
        real t10973
        real t10979
        real t1098
        real t10989
        real t1099
        real t10993
        real t10997
        real t11
        real t110
        real t1100
        real t11005
        real t11009
        real t11018
        real t1102
        real t11020
        real t11026
        real t11028
        real t1103
        real t11030
        real t11032
        real t11035
        real t11037
        real t11039
        real t11042
        real t11043
        real t11044
        real t11045
        real t11047
        real t11048
        real t11049
        real t1105
        real t11050
        real t11052
        real t11055
        real t11056
        real t11057
        real t11058
        real t11059
        real t11061
        real t11064
        real t11065
        real t1107
        real t11073
        real t11079
        real t11082
        real t11088
        real t1109
        real t11091
        real t11093
        real t11095
        real t11097
        real t11100
        real t11102
        real t11104
        real t11107
        real t1111
        real t11113
        real t11115
        real t11118
        real t1112
        real t11124
        real t11127
        real t11128
        real t11129
        real t11130
        real t11132
        real t11133
        real t11134
        real t11135
        real t11137
        real t1114
        real t11140
        real t11141
        real t11142
        real t11143
        real t11144
        real t11146
        real t11148
        real t11149
        real t11150
        real t11153
        real t11154
        real t11156
        real t11158
        real t11159
        real t1116
        real t11160
        real t11167
        real t11171
        real t11172
        real t11174
        real t11176
        real t11178
        real t1118
        real t11180
        real t11182
        real t11184
        real t11187
        real t11193
        real t11194
        real t11195
        real t11196
        real t1120
        real t11201
        real t11205
        real t11210
        real t1122
        real t11224
        real t11227
        real t11237
        real t11238
        real t1124
        real t11240
        real t11242
        real t11244
        real t11246
        real t11248
        real t1125
        real t11250
        real t11253
        real t11259
        real t11260
        real t1127
        real t11274
        real t11275
        real t11276
        real t11289
        real t1129
        real t11292
        real t11296
        real t113
        real t1130
        real t11302
        real t11303
        real t11305
        real t11307
        real t11309
        real t1131
        real t11311
        real t11313
        real t11315
        real t11318
        real t11324
        real t11325
        real t1133
        real t11333
        real t11337
        real t1134
        real t11341
        real t11342
        real t11344
        real t11346
        real t11348
        real t1135
        real t11350
        real t11352
        real t11353
        real t11354
        real t11357
        real t1136
        real t11360
        real t11363
        real t11364
        real t11372
        real t1138
        real t11385
        real t11389
        real t1139
        real t11394
        real t11400
        real t11406
        real t11407
        real t11408
        real t1141
        real t11411
        real t11412
        real t11413
        real t11415
        real t1142
        real t11420
        real t11421
        real t11422
        real t11431
        real t11432
        real t11435
        real t11436
        real t11438
        real t1144
        real t11440
        real t11442
        real t11444
        real t11446
        real t11448
        real t11451
        real t11457
        real t11458
        real t11459
        real t1146
        real t11460
        real t11469
        real t11474
        real t1148
        real t11488
        real t11491
        real t1150
        real t11501
        real t11502
        real t11504
        real t11506
        real t11508
        real t11510
        real t11512
        real t11514
        real t11517
        real t1152
        real t11523
        real t11524
        real t11529
        real t1153
        real t11538
        real t11539
        real t1154
        real t11540
        real t11545
        real t11550
        real t11553
        real t11556
        real t11557
        real t1156
        real t11560
        real t11566
        real t11567
        real t11569
        real t1157
        real t11571
        real t11573
        real t11575
        real t11577
        real t11579
        real t11582
        real t11588
        real t11589
        real t1159
        real t11597
        real t11601
        real t11605
        real t11606
        real t11608
        real t1161
        real t11610
        real t11612
        real t11614
        real t11616
        real t11618
        real t11621
        real t11627
        real t11628
        real t1163
        real t11636
        real t11649
        real t1165
        real t11653
        real t1166
        real t11664
        real t11669
        real t11670
        real t11671
        real t11672
        real t11675
        real t11676
        real t11677
        real t11679
        real t1168
        real t11684
        real t11685
        real t11686
        real t11692
        real t11695
        real t11696
        real t11699
        real t1170
        real t11704
        real t11706
        real t11707
        real t11709
        real t11711
        real t11713
        real t11715
        real t11717
        real t11719
        real t1172
        real t11722
        real t11728
        real t11729
        real t11730
        real t11731
        real t1174
        real t11740
        real t1176
        real t11760
        real t1177
        real t11777
        real t1179
        real t11790
        real t11791
        real t11792
        real t11795
        real t11796
        real t11797
        real t11799
        real t118
        real t11804
        real t11805
        real t11806
        real t1181
        real t11815
        real t11817
        real t11819
        real t11823
        real t11827
        real t1183
        real t11831
        real t11834
        real t11837
        real t11838
        real t1184
        real t11840
        real t11842
        real t11843
        real t11844
        real t11846
        real t11848
        real t1185
        real t11850
        real t11852
        real t11853
        real t11859
        real t1186
        real t11860
        real t1188
        real t11883
        real t11889
        real t11890
        real t11891
        real t119
        real t1190
        real t11900
        real t11901
        real t11904
        real t11905
        real t11907
        real t11909
        real t11911
        real t11913
        real t11915
        real t11917
        real t1192
        real t11920
        real t11926
        real t11927
        real t11928
        real t11929
        real t11938
        real t1194
        real t11955
        real t11958
        real t1196
        real t11969
        real t11975
        real t11976
        real t1198
        real t11981
        real t11988
        real t11989
        real t1199
        real t11990
        real t11993
        real t11994
        real t11995
        real t11997
        real t120
        real t12002
        real t12003
        real t12004
        real t1201
        real t12013
        real t12017
        real t12021
        real t12025
        real t12029
        real t1203
        real t12035
        real t12036
        real t12038
        real t12040
        real t12042
        real t12044
        real t12046
        real t12048
        real t1205
        real t12051
        real t12057
        real t12058
        real t1207
        real t12081
        real t12087
        real t12088
        real t12089
        real t1209
        real t12098
        real t12099
        real t121
        real t1210
        real t1211
        real t12116
        real t1212
        real t12133
        real t12134
        real t12135
        real t1214
        real t1215
        real t12154
        real t12155
        real t12157
        real t12159
        real t1216
        real t12161
        real t12163
        real t12165
        real t12167
        real t1217
        real t12170
        real t12176
        real t12177
        real t12185
        real t1219
        real t12191
        real t12192
        real t12193
        real t122
        real t12206
        real t12210
        real t12216
        real t12217
        real t12219
        real t1222
        real t12221
        real t12223
        real t12225
        real t12227
        real t12229
        real t1223
        real t12232
        real t12238
        real t12239
        real t12247
        real t1226
        real t12260
        real t12266
        real t12267
        real t12268
        real t1227
        real t12277
        real t12278
        integer t1228
        real t12281
        real t12282
        real t12283
        real t1229
        real t12302
        real t12303
        real t12305
        real t12307
        real t12309
        real t1231
        real t12311
        real t12313
        real t12315
        real t12318
        real t12324
        real t12325
        real t12333
        real t12339
        real t12340
        real t12341
        real t12354
        real t12358
        real t12364
        real t12365
        real t12367
        real t12369
        real t1237
        real t12371
        real t12373
        real t12375
        real t12377
        real t12380
        real t12386
        real t12387
        real t12395
        real t124
        real t1240
        real t12408
        real t12414
        real t12415
        real t12416
        real t1242
        real t12425
        real t12426
        real t12430
        real t12434
        real t12438
        real t12439
        real t1244
        real t12440
        real t12459
        real t12460
        real t12462
        real t12464
        real t12466
        real t12468
        real t12470
        real t12472
        real t12475
        real t12481
        real t12482
        real t12490
        real t12496
        real t12497
        real t12498
        real t1250
        real t12511
        real t12515
        real t12521
        real t12522
        real t12524
        real t12526
        real t12528
        real t12530
        real t12532
        real t12534
        real t12537
        real t1254
        real t12543
        real t12544
        real t12552
        real t12565
        real t1257
        real t12571
        real t12572
        real t12573
        real t12582
        real t12583
        real t12586
        real t12587
        real t12588
        real t1259
        real t126
        real t12607
        real t12608
        real t12610
        real t12612
        real t12614
        real t12616
        real t12618
        real t12620
        real t12623
        real t12629
        real t12630
        real t12638
        real t12644
        real t12645
        real t12646
        real t1265
        real t12659
        real t12663
        real t12669
        real t12670
        real t12672
        real t12674
        real t12676
        real t12678
        real t12680
        real t12682
        real t12685
        real t1269
        real t12691
        real t12692
        real t12700
        real t12713
        real t12719
        real t12720
        real t12721
        real t12730
        real t12731
        real t12735
        real t12748
        real t1276
        real t12766
        real t1277
        real t12770
        real t12779
        real t12789
        real t1279
        real t12790
        real t12800
        real t12802
        real t1281
        real t12817
        real t12820
        real t12824
        real t12828
        real t1283
        real t12832
        real t12835
        real t12839
        real t1285
        real t12850
        real t12866
        real t1287
        real t12870
        real t12879
        real t12889
        real t1289
        real t12895
        real t12899
        real t12902
        real t12915
        real t12916
        real t1292
        real t12921
        real t12924
        real t12927
        real t12929
        real t12941
        real t12944
        real t12946
        real t12952
        real t12954
        real t12970
        real t1298
        real t12981
        real t1299
        real t12998
        real t13
        real t13000
        real t13003
        real t13005
        real t13011
        real t13017
        real t13022
        real t13027
        real t13029
        real t13033
        real t13038
        real t13039
        real t1304
        real t13040
        real t13047
        real t13051
        real t13056
        real t1306
        real t13066
        real t13067
        real t1307
        real t13072
        real t13078
        real t13083
        real t13088
        real t1309
        real t13090
        real t13094
        real t13099
        real t131
        real t13101
        real t13108
        real t13112
        real t13117
        real t13127
        real t13128
        real t13132
        real t13138
        real t13142
        real t13145
        real t13148
        real t1315
        real t13150
        real t13152
        real t13165
        real t13183
        real t13187
        real t1319
        real t13195
        real t132
        real t13203
        real t13214
        real t1323
        real t13231
        real t13236
        real t13238
        real t13244
        real t13248
        real t1325
        real t13255
        real t13260
        real t13274
        real t13278
        real t13283
        real t13291
        real t13292
        real t13297
        real t133
        real t1330
        real t13301
        real t13308
        integer t1331
        real t13313
        real t1332
        real t13327
        real t1333
        real t13331
        real t13336
        real t13344
        real t13345
        real t13346
        real t13349
        real t1335
        real t13351
        real t13355
        real t13359
        real t13362
        real t13365
        real t13367
        real t13369
        real t1337
        real t13382
        real t1339
        real t134
        real t13400
        real t13404
        real t13409
        real t1341
        real t13412
        real t13417
        real t13425
        real t1343
        real t13431
        real t13433
        real t13437
        real t13440
        real t13442
        real t13443
        real t13444
        real t1345
        real t13450
        real t13461
        real t13462
        real t13463
        real t13466
        real t13467
        real t13471
        real t13473
        real t13477
        real t1348
        real t13480
        real t13481
        real t13488
        real t1349
        real t13493
        real t13495
        real t135
        real t13504
        real t13510
        real t13514
        real t13517
        real t13520
        real t13522
        real t13524
        real t1353
        real t13532
        real t13534
        real t13535
        real t13538
        real t1354
        real t13541
        real t13543
        real t13544
        real t13547
        real t13548
        real t1355
        real t13554
        real t13561
        real t13565
        real t13566
        real t13567
        real t13568
        real t13570
        real t13571
        real t13572
        real t13573
        real t13574
        real t13578
        real t13580
        real t13584
        real t13587
        real t13588
        real t13596
        real t136
        real t1360
        real t13600
        real t13607
        real t1361
        real t13612
        real t13617
        real t13620
        real t13623
        real t13627
        real t1363
        integer t13639
        real t1364
        real t13640
        real t13641
        real t13643
        real t13645
        real t13647
        real t13649
        real t13651
        real t13653
        real t13656
        real t1366
        real t13662
        real t13663
        real t13664
        real t13665
        real t13667
        real t1367
        real t13675
        real t13679
        real t1368
        real t13684
        real t13685
        real t13687
        real t13688
        real t13690
        real t13696
        real t137
        real t1370
        real t13719
        real t1372
        real t13729
        real t13741
        real t13745
        real t13747
        real t13756
        real t1376
        real t13763
        real t13766
        real t13768
        real t13776
        real t13780
        real t13783
        real t13787
        integer t1379
        real t13793
        real t13794
        real t13797
        real t1380
        real t13801
        real t1381
        real t13812
        real t13822
        real t1383
        real t13834
        real t13838
        real t13842
        real t1385
        real t13858
        real t1387
        real t13873
        real t1389
        real t139
        real t13909
        real t1391
        real t13912
        real t13923
        real t13925
        real t13928
        real t1393
        real t13930
        real t13934
        real t13946
        real t1396
        real t13978
        real t13980
        real t13986
        real t140
        real t14007
        real t1401
        real t1402
        real t14026
        real t14028
        real t1403
        real t14031
        real t14033
        real t14037
        real t14044
        real t14052
        real t14054
        real t14059
        real t14065
        real t14066
        real t14070
        real t14072
        real t1408
        real t1409
        real t14094
        real t14097
        real t14105
        real t1411
        real t14114
        real t14117
        real t1412
        real t14125
        real t1414
        real t14142
        real t14150
        real t1416
        real t14163
        real t1418
        real t14187
        real t14191
        real t14195
        real t142
        real t1420
        real t14203
        real t14207
        real t14217
        real t14226
        real t14231
        real t14235
        real t14249
        real t14251
        real t14257
        real t14261
        real t14264
        real t14266
        real t14272
        real t1429
        integer t1430
        real t14301
        real t1431
        real t14313
        real t14323
        real t14327
        real t1433
        real t14342
        real t14347
        real t14364
        real t1437
        real t14370
        real t14380
        real t1439
        real t14392
        real t14394
        real t14397
        real t14398
        real t14399
        real t144
        real t14401
        real t14402
        real t14403
        real t14404
        real t14406
        real t14409
        integer t1441
        real t14410
        real t14411
        real t14412
        real t14413
        real t14415
        real t14418
        real t14419
        real t1442
        real t14423
        real t14425
        real t14427
        real t14429
        real t14432
        real t14434
        real t14436
        real t14439
        real t1444
        real t14445
        real t14451
        real t14454
        real t14460
        real t14463
        real t14471
        real t14473
        real t14476
        real t1448
        real t14482
        real t14485
        real t14487
        real t14489
        real t14491
        real t14494
        real t14496
        real t14498
        real t145
        real t14501
        real t14502
        real t14503
        real t14504
        real t14506
        real t14507
        real t14508
        real t14509
        real t14511
        real t14514
        real t14515
        real t14516
        real t14517
        real t14518
        real t1452
        real t14520
        real t14523
        real t14524
        real t14527
        real t14528
        real t1453
        real t14530
        real t14532
        real t14533
        real t14541
        real t14546
        real t14547
        real t14548
        real t1455
        real t14557
        real t1457
        real t14576
        real t14577
        real t14579
        real t14581
        real t14583
        real t14585
        real t14587
        real t14589
        real t1459
        real t14592
        real t14598
        real t14599
        real t1461
        real t14613
        real t14614
        real t14615
        real t14628
        real t1463
        real t14631
        real t14648
        real t1465
        real t14664
        real t14668
        real t14675
        real t1468
        real t14681
        real t14682
        real t14683
        real t14686
        real t14687
        real t14688
        real t14690
        real t14695
        real t14696
        real t14697
        real t147
        real t14706
        real t14707
        real t14715
        real t14719
        real t14720
        real t14721
        real t1473
        real t14730
        real t1474
        real t14749
        real t1475
        real t14750
        real t14752
        real t14754
        real t14756
        real t14758
        real t1476
        real t14760
        real t14762
        real t14765
        real t1477
        real t14771
        real t14772
        real t14786
        real t14787
        real t14788
        real t1479
        real t14801
        real t14804
        real t1482
        real t14821
        real t1483
        real t14837
        real t14841
        real t14848
        real t1485
        real t14854
        real t14855
        real t14856
        real t14859
        real t14860
        real t14861
        real t14863
        real t14868
        real t14869
        real t14870
        real t14879
        real t1488
        real t14880
        real t1489
        real t14893
        real t149
        real t1491
        real t14916
        real t14917
        real t14918
        real t14921
        real t14922
        real t14923
        real t14925
        real t1493
        real t14930
        real t14931
        real t14932
        real t14944
        real t1495
        real t14956
        real t14965
        real t14966
        real t14968
        real t1497
        real t14970
        real t14972
        real t14974
        real t14976
        real t14978
        real t14981
        real t14987
        real t14988
        real t1499
        real t15
        real t150
        real t15004
        real t15005
        real t15006
        real t1501
        real t15019
        real t15029
        real t15030
        real t15032
        real t15034
        real t15036
        real t15038
        real t1504
        real t15040
        real t15042
        real t15045
        real t15051
        real t15052
        real t15062
        real t15081
        real t15082
        real t15083
        real t1509
        real t15092
        real t15093
        real t15096
        real t15097
        real t15098
        real t1510
        real t15101
        real t15102
        real t15103
        real t15105
        real t1511
        real t15110
        real t15111
        real t15112
        real t1512
        real t15124
        real t1513
        real t15136
        real t15141
        real t15145
        real t15146
        real t15148
        real t1515
        real t15150
        real t15152
        real t15154
        real t15156
        real t15158
        real t15161
        real t15167
        real t15168
        real t1518
        real t15184
        real t15185
        real t15186
        real t1519
        real t15199
        real t15202
        real t15206
        real t15209
        real t1521
        real t15210
        real t15212
        real t15214
        real t15216
        real t15218
        real t15220
        real t15222
        real t15225
        real t15231
        real t15232
        real t15242
        real t15261
        real t15262
        real t15263
        real t15272
        real t15273
        real t15288
        real t1529
        real t15292
        real t153
        real t15301
        real t15314
        real t15331
        real t15332
        real t1534
        real t15344
        real t15346
        real t1536
        real t15360
        real t1537
        real t15376
        real t15380
        real t15389
        real t1539
        real t15400
        real t1541
        real t15415
        real t15416
        real t15421
        real t15425
        real t1543
        real t15430
        real t15431
        real t15433
        real t15435
        real t15439
        real t15442
        real t15444
        real t15446
        real t15448
        real t15449
        real t1545
        real t15453
        real t15454
        real t15455
        real t15465
        real t15466
        real t15471
        real t15474
        real t15477
        real t15479
        real t1549
        real t15492
        real t15494
        real t15496
        real t15497
        real t15500
        real t15502
        real t15508
        real t15510
        real t15512
        real t15515
        real t15517
        real t15518
        real t1552
        real t15522
        real t15530
        real t15535
        real t15542
        real t15553
        real t15554
        real t15555
        real t15558
        real t15559
        real t15563
        real t15565
        real t15569
        real t1557
        real t15572
        real t15573
        real t15580
        real t15585
        real t15587
        real t1559
        real t15598
        real t1560
        real t15610
        real t1562
        real t15622
        real t15626
        real t15632
        real t15636
        real t15637
        real t1564
        real t15645
        real t15647
        real t15650
        real t15659
        real t1566
        real t15671
        real t15675
        real t1568
        real t15681
        real t15685
        real t15686
        real t15690
        real t15696
        real t15700
        real t15703
        real t15706
        real t15708
        real t15710
        real t15717
        real t15724
        real t15735
        real t15736
        real t15737
        real t15740
        real t15741
        real t15745
        real t15747
        real t15751
        real t15754
        real t15755
        real t15763
        real t15767
        real t1578
        real t15783
        real t15794
        real t158
        real t15811
        real t15816
        real t15818
        real t15827
        real t15833
        real t15837
        real t15840
        real t15843
        real t15845
        real t15847
        real t1585
        real t15860
        real t15869
        real t15873
        real t15878
        real t1588
        real t15880
        real t15882
        real t15887
        real t15889
        real t15894
        real t15895
        real t15899
        real t159
        real t15902
        real t15905
        real t15923
        real t15933
        real t15945
        real t15961
        real t15983
        integer t15994
        real t15995
        real t15997
        real t1600
        real t16005
        real t16007
        real t1601
        real t16014
        real t16017
        real t16019
        real t16030
        real t16033
        real t16035
        real t16037
        real t16045
        real t16048
        real t16050
        real t16065
        real t1607
        real t16075
        real t16087
        real t1609
        real t16094
        real t161
        real t16108
        real t1612
        real t1613
        real t1614
        real t1615
        real t16150
        real t16160
        real t16165
        real t16166
        real t1618
        real t1619
        real t16192
        real t16194
        real t16195
        real t16197
        real t16199
        real t1620
        real t16201
        real t16203
        real t16205
        real t16207
        real t16210
        real t16216
        real t16217
        real t16218
        real t16219
        real t1622
        real t16221
        real t16229
        real t16233
        real t16239
        real t16241
        real t16247
        real t1625
        real t1626
        real t16265
        real t16268
        real t1628
        real t16281
        real t16283
        real t16289
        real t16298
        real t16300
        real t16303
        real t16305
        real t16309
        real t1631
        real t16316
        real t16317
        real t16319
        real t1632
        real t16333
        real t16336
        real t16338
        real t16342
        real t16358
        real t16363
        real t1638
        real t16389
        real t1639
        real t164
        real t16401
        real t16403
        real t16407
        real t16409
        real t1643
        real t16435
        real t16459
        real t16463
        real t16467
        real t16475
        real t16479
        real t1648
        real t16484
        real t16488
        real t16489
        real t1649
        real t16491
        real t165
        real t16501
        real t16504
        real t16506
        real t1651
        real t16523
        real t16535
        real t16541
        real t16545
        real t16549
        real t16555
        real t16577
        real t16588
        real t1659
        real t166
        real t16608
        real t1661
        real t1662
        real t16642
        real t16654
        real t16656
        real t16659
        real t16660
        real t16661
        real t16663
        real t16664
        real t16665
        real t16666
        real t16668
        real t1667
        real t16671
        real t16672
        real t16673
        real t16674
        real t16675
        real t16677
        real t16680
        real t16681
        real t16685
        real t16687
        real t16689
        real t16691
        real t16694
        real t16696
        real t16698
        real t16701
        real t16707
        real t1671
        real t16713
        real t16716
        real t16722
        real t16725
        real t16733
        real t16735
        real t16738
        real t16744
        real t16747
        real t16749
        real t1675
        real t16751
        real t16753
        real t16756
        real t16758
        real t16760
        real t16763
        real t16764
        real t16765
        real t16766
        real t16768
        real t16769
        real t1677
        real t16770
        real t16771
        real t16773
        real t16776
        real t16777
        real t16778
        real t16779
        real t16780
        real t16782
        real t16785
        real t16786
        real t16789
        real t16790
        real t16792
        real t16794
        real t16795
        real t168
        real t16803
        real t16808
        real t16809
        real t16810
        real t16819
        real t1682
        real t16838
        real t16839
        real t1684
        real t16841
        real t16843
        real t16845
        real t16847
        real t16849
        real t16851
        real t16854
        real t16860
        real t16861
        real t16875
        real t16876
        real t16877
        real t1688
        real t16890
        real t16893
        real t169
        real t1690
        real t16910
        real t16926
        real t16930
        real t16937
        real t16943
        real t16944
        real t16945
        real t16948
        real t16949
        real t16950
        real t16952
        real t16957
        real t16958
        real t16959
        real t16968
        real t16969
        real t16977
        real t16981
        real t16982
        real t16983
        real t16992
        real t17
        real t1700
        real t17011
        real t17012
        real t17014
        real t17016
        real t17018
        real t17020
        real t17022
        real t17024
        real t17027
        real t1703
        real t17033
        real t17034
        real t17048
        real t17049
        real t17050
        real t17063
        real t17066
        real t1707
        real t17083
        real t17099
        real t171
        real t1710
        real t17103
        real t17110
        real t17116
        real t17117
        real t17118
        real t1712
        real t17121
        real t17122
        real t17123
        real t17125
        real t17130
        real t17131
        real t17132
        real t17141
        real t17142
        real t1715
        real t17155
        real t1716
        real t17178
        real t17179
        real t1718
        real t17180
        real t17183
        real t17184
        real t17185
        real t17187
        real t17192
        real t17193
        real t17194
        real t172
        real t17206
        real t1721
        real t17218
        real t17227
        real t17228
        real t17230
        real t17232
        real t17234
        real t17236
        real t17238
        real t17240
        real t17243
        real t17249
        real t1725
        real t17250
        real t17266
        real t17267
        real t17268
        real t1727
        real t17281
        real t17291
        real t17292
        real t17294
        real t17296
        real t17298
        real t17300
        real t17302
        real t17304
        real t17307
        real t17313
        real t17314
        real t17324
        real t17343
        real t17344
        real t17345
        real t17354
        real t17355
        real t17358
        real t17359
        real t17360
        real t17363
        real t17364
        real t17365
        real t17367
        real t1737
        real t17372
        real t17373
        real t17374
        real t17386
        real t1739
        real t17398
        real t174
        real t17402
        real t17407
        real t17408
        real t1741
        real t17410
        real t17412
        real t17414
        real t17416
        real t17418
        real t17420
        real t17423
        real t17429
        real t1743
        real t17430
        real t17433
        real t17446
        real t17447
        real t17448
        real t1745
        real t17455
        real t1746
        real t17461
        real t17471
        real t17472
        real t17474
        real t17476
        real t17478
        real t17480
        real t17482
        real t17484
        real t17487
        real t1749
        real t17493
        real t17494
        real t175
        real t17504
        real t17523
        real t17524
        real t17525
        real t17534
        real t17535
        real t17550
        real t17554
        real t17563
        real t1757
        real t17576
        real t1759
        real t17593
        real t17594
        real t17606
        real t17608
        real t1761
        real t17622
        real t1763
        real t17638
        real t17642
        real t1765
        real t17651
        real t17662
        real t17677
        real t17683
        real t17687
        real t17690
        real t17703
        real t17704
        real t17709
        real t17712
        real t17715
        real t17717
        real t17729
        real t17732
        real t17734
        real t17740
        real t17742
        real t17758
        real t17769
        real t1778
        real t17782
        real t17786
        real t17791
        real t17793
        real t178
        real t1780
        real t17804
        real t1781
        real t17816
        real t17828
        real t1783
        real t17832
        real t17838
        real t17842
        real t17843
        real t1785
        real t17853
        real t17865
        real t1787
        real t17877
        real t17881
        real t17887
        real t1789
        real t17891
        real t17892
        real t17896
        real t179
        real t17902
        real t17906
        real t17909
        real t17912
        real t17914
        real t17916
        real t17929
        real t1793
        real t17947
        real t1795
        real t17951
        real t17956
        real t17959
        real t17964
        real t17972
        real t17978
        real t17980
        real t17984
        real t17987
        real t17994
        integer t180
        real t1800
        real t18005
        real t18006
        real t18007
        real t18010
        real t18011
        real t18015
        real t18017
        real t1802
        real t18021
        real t18024
        real t18025
        real t1803
        real t18032
        real t18037
        real t18039
        real t18048
        real t1805
        real t18054
        real t18058
        real t18061
        real t18064
        real t18066
        real t18068
        real t1807
        real t18073
        real t18076
        real t18078
        real t18079
        real t18082
        real t18085
        real t18087
        real t1809
        real t18092
        real t181
        real t18101
        real t18103
        real t18104
        real t18105
        real t18108
        real t18109
        real t1811
        real t18112
        real t18113
        real t18115
        real t18119
        real t18122
        real t18123
        real t18131
        real t18135
        real t18140
        real t18145
        real t18153
        real t18159
        real t18161
        real t18165
        real t18168
        real t18175
        real t18186
        real t18187
        real t18188
        real t18191
        real t18192
        real t18196
        real t18198
        real t18202
        real t18205
        real t18206
        real t1821
        real t18213
        real t18218
        real t18219
        real t1822
        real t18220
        real t18225
        real t18229
        real t1823
        real t18235
        real t18239
        real t1824
        real t18242
        real t18245
        real t18247
        real t18249
        real t18257
        real t18259
        real t1826
        real t18263
        real t18266
        real t18273
        real t18284
        real t18285
        real t18286
        real t18289
        real t18290
        real t18294
        real t18296
        real t183
        real t18300
        real t18303
        real t18304
        real t18312
        real t18316
        real t18323
        real t18328
        real t18333
        real t18336
        real t18339
        real t18343
        real t1836
        real t18369
        real t18374
        integer t18383
        real t18384
        real t18386
        real t18394
        real t18396
        real t184
        real t18403
        real t18406
        real t18408
        real t1841
        real t1842
        real t18422
        real t18423
        real t18425
        real t18427
        real t18429
        real t1843
        real t18431
        real t18433
        real t18435
        real t18438
        real t18444
        real t18445
        real t1845
        real t18451
        real t18453
        real t18459
        real t18489
        integer t185
        real t18502
        real t18503
        real t18504
        real t18506
        real t18514
        real t18522
        real t18524
        real t18525
        real t18527
        real t1853
        real t18533
        real t18545
        real t18565
        real t18568
        real t18577
        real t1858
        real t18587
        real t18599
        real t186
        real t18608
        real t1862
        real t18621
        real t18631
        real t1864
        real t18643
        real t18648
        real t18658
        real t18662
        real t1867
        real t18676
        real t18680
        real t18707
        real t1871
        real t18718
        real t18720
        real t18723
        real t18725
        real t18729
        real t18736
        real t18740
        real t1877
        real t18772
        real t1878
        real t18787
        real t188
        real t1880
        real t18814
        real t18825
        real t18827
        real t18841
        real t18844
        real t18846
        real t1885
        real t1886
        real t18863
        real t18866
        real t18874
        real t1888
        real t18883
        real t18889
        real t18915
        real t18925
        real t18929
        real t18950
        real t1897
        real t18986
        real t19
        real t190
        real t19000
        real t19012
        real t19014
        real t19017
        real t19018
        real t19019
        real t19021
        real t19022
        real t19023
        real t19024
        real t19026
        real t19029
        real t1903
        real t19030
        real t19031
        real t19032
        real t19033
        real t19035
        real t19038
        real t19039
        real t19047
        real t19053
        real t19056
        real t19062
        real t19065
        real t19067
        real t19069
        real t19071
        real t19073
        real t19076
        real t19078
        real t19080
        real t19083
        real t19089
        real t19091
        real t19094
        real t1910
        real t19100
        real t19103
        real t19104
        real t19105
        real t19106
        real t19108
        real t19109
        real t19110
        real t19111
        real t19113
        real t19116
        real t19117
        real t19118
        real t19119
        real t19120
        real t19122
        real t19125
        real t19126
        real t19130
        real t19132
        real t19134
        real t19137
        real t19139
        real t1914
        real t19141
        real t19144
        real t19147
        real t19148
        real t19150
        real t19152
        real t19153
        real t19161
        real t19169
        real t19178
        real t19179
        real t1918
        real t19180
        real t19198
        real t192
        real t1920
        real t19210
        real t19215
        real t19228
        real t19229
        real t1923
        real t19230
        real t19233
        real t19234
        real t19235
        real t19237
        real t19242
        real t19243
        real t19244
        real t19253
        real t19257
        real t19261
        real t19265
        real t19269
        real t19275
        real t19276
        real t19278
        real t19280
        real t19282
        real t19284
        real t19286
        real t19288
        real t19291
        real t19297
        real t19298
        real t193
        real t1931
        real t1932
        real t19327
        real t19328
        real t19329
        real t19338
        real t19339
        real t19347
        real t19350
        real t19351
        real t19352
        real t19353
        real t1936
        real t1937
        real t19371
        real t19388
        real t19396
        real t19401
        real t19402
        real t19403
        real t19406
        real t19407
        real t19408
        real t19410
        real t19415
        real t19416
        real t19417
        real t19426
        real t19430
        real t19434
        real t19438
        real t19442
        real t19448
        real t19449
        real t19451
        real t19453
        real t19455
        real t19457
        real t19459
        real t19461
        real t19464
        real t1947
        real t19470
        real t19471
        real t1950
        real t19500
        real t19501
        real t19502
        real t19511
        real t19512
        real t19525
        real t19538
        real t19539
        real t1954
        real t19540
        real t19543
        real t19544
        real t19545
        real t19547
        real t19552
        real t19553
        real t19554
        real t19566
        real t19578
        real t1958
        real t19596
        real t19597
        real t19598
        real t196
        real t19607
        real t19617
        real t19618
        real t19620
        real t19622
        real t19624
        real t19626
        real t19628
        real t1963
        real t19630
        real t19633
        real t19639
        real t19640
        real t19669
        real t19670
        real t19671
        real t19680
        real t19681
        real t19689
        real t19693
        real t19694
        real t19695
        real t19698
        real t19699
        real t197
        real t19700
        real t19702
        real t19707
        real t19708
        real t19709
        real t19721
        real t19725
        real t19733
        real t19751
        real t19752
        real t19753
        real t19762
        real t19772
        real t19773
        real t19775
        real t19777
        real t19779
        real t1978
        real t19781
        real t19783
        real t19785
        real t19788
        real t19794
        real t19795
        real t198
        real t19824
        real t19825
        real t19826
        real t1983
        real t19835
        real t19836
        real t19871
        real t19872
        real t19882
        real t19896
        real t19898
        real t19907
        real t1991
        real t19911
        real t19912
        real t19918
        real t1993
        real t19930
        real t19932
        real t19938
        real t19943
        real t19949
        real t19953
        real t19958
        real t19959
        real t19961
        real t19967
        real t1997
        real t19970
        real t19972
        real t19974
        real t19976
        real t19977
        real t19981
        real t19982
        real t19983
        real t19993
        real t19994
        real t19999
        real t2
        real t200
        real t2000
        real t20002
        real t20005
        real t20007
        real t2002
        real t20020
        real t20022
        real t20024
        real t20025
        real t20028
        real t20030
        real t20036
        real t20038
        real t20047
        real t20052
        real t20054
        real t20065
        real t20082
        real t20087
        real t20089
        real t20098
        real t201
        real t20104
        real t20108
        real t20111
        real t20114
        real t20116
        real t20118
        real t20131
        real t20149
        real t20153
        real t20169
        real t2017
        real t20180
        real t20183
        real t20188
        real t20197
        real t20202
        real t20204
        real t20213
        real t20219
        real t2022
        real t20223
        real t20226
        real t20229
        real t20231
        real t20233
        real t20246
        real t2025
        real t20264
        real t20268
        real t20275
        real t20280
        real t20285
        real t20288
        real t20291
        integer t20292
        real t20293
        real t20295
        real t203
        real t20303
        real t20305
        real t20312
        real t20315
        real t20317
        real t2032
        real t20338
        real t20339
        real t20341
        real t20343
        real t20345
        real t20347
        real t20349
        real t20351
        real t20354
        real t2036
        real t20360
        real t20361
        real t20362
        real t20363
        real t20365
        real t20368
        real t20371
        real t2039
        real t20395
        real t2040
        real t20406
        real t20408
        real t2041
        real t20418
        real t20430
        real t20439
        real t2044
        real t20467
        real t20473
        real t20475
        real t2048
        real t20481
        real t20498
        real t205
        real t2050
        real t20505
        real t2052
        real t20530
        real t20540
        real t2055
        real t20552
        real t20560
        real t20562
        real t20563
        real t20565
        real t2057
        real t20571
        real t20585
        real t2060
        real t2061
        real t20614
        real t20624
        real t20628
        real t2063
        real t20645
        real t20659
        real t2067
        real t20670
        real t20672
        real t20675
        real t20677
        real t2068
        real t20681
        real t20688
        real t20692
        real t207
        real t20723
        real t20726
        real t20734
        real t20743
        real t20746
        real t2075
        real t20754
        real t2076
        real t2077
        real t20771
        real t20787
        real t2079
        real t20791
        real t20807
        real t20820
        real t2083
        real t20834
        real t20838
        real t2085
        real t20855
        real t20871
        real t209
        real t20909
        real t20919
        real t20923
        real t2093
        real t2095
        real t20953
        real t20964
        real t20966
        real t20969
        real t20970
        real t20971
        real t20973
        real t20974
        real t20975
        real t20976
        real t20978
        real t2098
        real t20981
        real t20982
        real t20983
        real t20984
        real t20985
        real t20987
        real t2099
        real t20990
        real t20991
        real t20999
        real t210
        real t21005
        real t21008
        real t2101
        real t21014
        real t21017
        real t21019
        real t21021
        real t21023
        real t21025
        real t21028
        real t21030
        real t21032
        real t21035
        real t2104
        real t21041
        real t21043
        real t21046
        real t21052
        real t21055
        real t21056
        real t21057
        real t21058
        real t21060
        real t21061
        real t21062
        real t21063
        real t21065
        real t21068
        real t21069
        real t21070
        real t21071
        real t21072
        real t21074
        real t21077
        real t21078
        real t2108
        real t21082
        real t21084
        real t21086
        real t21089
        real t21091
        real t21093
        real t21096
        real t21099
        real t211
        real t21100
        real t21102
        real t21104
        real t21105
        real t2111
        real t21113
        real t21121
        real t2113
        real t21130
        real t21131
        real t21132
        real t21150
        real t2116
        real t21167
        real t2117
        real t21180
        real t21181
        real t21182
        real t21185
        real t21186
        real t21187
        real t21189
        real t2119
        real t21194
        real t21195
        real t21196
        real t21205
        real t21209
        real t21213
        real t21217
        real t2122
        real t21221
        real t21227
        real t21228
        real t21230
        real t21232
        real t21234
        real t21236
        real t21238
        real t21240
        real t21243
        real t21249
        real t21250
        real t2126
        real t21279
        real t2128
        real t21280
        real t21281
        real t21290
        real t21291
        real t21299
        real t21303
        real t21304
        real t21305
        real t21323
        real t2133
        real t21340
        real t21353
        real t21354
        real t21355
        real t21358
        real t21359
        real t2136
        real t21360
        real t21362
        real t21367
        real t21368
        real t21369
        real t21378
        real t21382
        real t21386
        real t21390
        real t21394
        real t214
        real t21400
        real t21401
        real t21403
        real t21405
        real t21407
        real t21409
        real t21411
        real t21413
        real t21416
        real t21422
        real t21423
        real t2144
        real t21452
        real t21453
        real t21454
        real t21463
        real t21464
        real t21477
        real t2148
        real t21490
        real t21491
        real t21492
        real t21495
        real t21496
        real t21497
        real t21499
        real t215
        real t21504
        real t21505
        real t21506
        real t21518
        real t2152
        real t21530
        real t2154
        real t21548
        real t21549
        real t21550
        real t21559
        real t21569
        real t21570
        real t21572
        real t21574
        real t21576
        real t21578
        real t21580
        real t21582
        real t21585
        real t2159
        real t21591
        real t21592
        real t216
        real t2161
        real t21621
        real t21622
        real t21623
        real t21632
        real t21633
        real t21641
        real t21645
        real t21646
        real t21647
        real t21650
        real t21651
        real t21652
        real t21654
        real t21659
        real t21660
        real t21661
        real t2167
        real t21673
        real t21685
        real t21703
        real t21704
        real t21705
        real t21714
        real t21724
        real t21725
        real t21727
        real t21729
        real t21731
        real t21733
        real t21735
        real t21737
        real t21740
        real t21746
        real t21747
        real t2177
        real t21776
        real t21777
        real t21778
        real t21787
        real t21788
        real t218
        real t2181
        real t21823
        real t21824
        real t21834
        real t2184
        real t21848
        real t21850
        real t2186
        real t21864
        real t21882
        real t21895
        real t219
        real t21901
        real t21905
        real t21908
        real t2192
        real t21921
        real t21922
        real t21927
        real t21930
        real t21933
        real t21935
        real t21947
        real t21950
        real t21952
        real t21958
        real t21960
        real t21965
        real t21999
        real t22
        real t22032
        real t2204
        real t22065
        real t2207
        real t2209
        real t221
        real t2213
        real t2215
        real t2223
        real t2225
        real t2228
        real t2229
        real t223
        real t2231
        real t2234
        real t2238
        real t2241
        real t2243
        real t2246
        real t2247
        real t2249
        real t225
        real t2252
        real t2256
        real t2258
        real t2263
        real t2266
        real t227
        real t2270
        real t2274
        real t2278
        real t228
        real t2281
        real t2284
        real t2288
        real t2292
        real t2302
        real t2307
        real t231
        real t2310
        real t232
        real t2325
        real t2329
        integer t233
        real t2333
        real t2335
        real t234
        real t2340
        real t2345
        real t2348
        real t2352
        real t2356
        real t236
        real t2360
        real t2363
        real t2366
        real t237
        real t2370
        real t2374
        integer t238
        real t2383
        real t2384
        real t2388
        real t2389
        real t239
        real t2399
        real t2403
        real t2408
        real t241
        real t2425
        real t2427
        real t243
        real t2432
        real t2438
        real t2443
        real t245
        real t2452
        real t2454
        real t2460
        real t2464
        real t2468
        real t2470
        real t2471
        real t2475
        real t2476
        real t248
        real t2488
        real t249
        real t2490
        real t2495
        real t250
        real t2501
        real t2506
        real t2515
        real t252
        real t2521
        real t2524
        real t2528
        real t253
        real t2532
        real t2538
        real t2547
        real t255
        real t2550
        real t2555
        real t2565
        real t2567
        real t2569
        real t257
        real t2572
        real t2574
        real t2583
        real t2589
        real t259
        real t2591
        real t2597
        real t2601
        real t2605
        real t2607
        real t2608
        real t261
        real t2613
        real t262
        real t2626
        real t2628
        real t2630
        real t2633
        real t2634
        real t2636
        real t2638
        real t2640
        real t2642
        real t2644
        real t2646
        real t2647
        real t2649
        real t265
        real t2654
        real t2655
        real t2656
        real t266
        real t2661
        real t2662
        real t2664
        real t2666
        real t2668
        real t267
        real t2671
        real t2672
        real t2673
        real t2675
        real t2677
        real t2679
        real t2681
        real t2683
        real t2685
        real t2688
        real t269
        real t2693
        real t2694
        real t2695
        real t27
        real t270
        real t2701
        real t2703
        real t2706
        real t2707
        real t2708
        real t2709
        real t2711
        real t2712
        real t2713
        real t2714
        real t2716
        real t2719
        real t272
        real t2720
        real t2721
        real t2722
        real t2723
        real t2725
        real t2728
        real t2729
        real t2736
        real t2738
        real t2739
        real t274
        real t2741
        real t2743
        real t2745
        real t2751
        real t2753
        real t2754
        real t2759
        real t276
        real t2760
        real t2761
        real t2762
        real t2764
        real t2766
        real t2768
        real t2771
        real t2772
        real t2773
        real t2775
        real t2777
        real t2779
        real t278
        real t2781
        real t2783
        real t2785
        real t2788
        real t279
        real t2793
        real t2794
        real t2795
        real t28
        real t280
        real t2801
        real t2803
        real t2805
        real t2808
        real t2809
        real t281
        real t2810
        real t2812
        real t2814
        real t2816
        real t2818
        real t2820
        real t2822
        real t2825
        real t283
        real t2830
        real t2831
        real t2832
        real t2838
        real t2840
        real t2843
        real t2849
        real t285
        real t2851
        real t2853
        real t2855
        real t2857
        real t2860
        real t2866
        real t2868
        real t287
        real t2870
        real t2872
        real t2875
        real t2876
        real t2877
        real t2878
        real t2880
        real t2881
        real t2882
        real t2883
        real t2885
        real t2888
        real t2889
        real t289
        real t2890
        real t2891
        real t2892
        real t2894
        real t2897
        real t2898
        real t29
        real t2901
        real t2902
        real t2904
        real t2905
        real t2907
        real t2908
        real t291
        real t2916
        real t2917
        real t2919
        real t2922
        real t2923
        real t2925
        real t2927
        real t2929
        real t293
        real t2931
        real t2933
        real t2935
        real t2938
        real t2944
        real t2945
        real t2946
        real t2947
        real t2950
        real t2951
        real t2952
        real t2954
        real t2959
        real t296
        real t2960
        real t2961
        real t2963
        real t2966
        real t2967
        real t2970
        real t2973
        real t2975
        real t2983
        real t2985
        real t2990
        real t2992
        real t2994
        real t2995
        real t30
        real t3000
        real t3003
        real t3008
        real t301
        real t3014
        real t3015
        real t302
        real t3020
        real t3024
        real t3026
        real t3027
        real t3028
        real t3029
        real t303
        real t3031
        real t3032
        real t3033
        real t3035
        real t3037
        real t3039
        real t3041
        real t3044
        real t3050
        real t3051
        real t306
        real t3065
        real t3066
        real t3067
        real t3080
        real t3083
        real t3087
        real t3089
        real t309
        real t3093
        real t3094
        real t3095
        real t3096
        real t3098
        real t31
        real t3100
        real t3102
        real t3104
        real t3106
        real t3109
        real t311
        real t3115
        real t3116
        real t3124
        real t3126
        real t313
        real t3130
        real t3134
        real t3135
        real t3137
        real t3139
        real t3141
        real t3143
        real t3145
        real t3147
        real t315
        real t3150
        real t3156
        real t3157
        real t3165
        real t3167
        real t317
        real t3180
        real t3184
        real t319
        real t3195
        real t3201
        real t3202
        real t3203
        real t3206
        real t3207
        real t3208
        real t321
        real t3210
        real t3215
        real t3216
        real t3217
        real t322
        real t3226
        real t3227
        real t323
        real t3230
        real t3231
        real t3233
        real t3235
        real t3237
        real t3239
        real t324
        real t3241
        real t3243
        real t3246
        real t3252
        real t3253
        real t3254
        real t3255
        real t3258
        real t3259
        real t326
        real t3260
        real t3262
        real t3267
        real t3268
        real t3269
        real t3271
        real t3272
        real t3274
        real t3275
        real t3278
        real t328
        real t3283
        real t3291
        real t3293
        real t3298
        real t33
        real t330
        real t3300
        real t3302
        real t3303
        real t3308
        real t3309
        real t3311
        real t3315
        real t332
        real t3320
        real t3323
        real t3327
        real t3332
        real t3334
        real t3335
        real t3336
        real t3337
        real t3339
        real t334
        real t3341
        real t3343
        real t3345
        real t3347
        real t3349
        real t3352
        real t3358
        real t3359
        real t336
        real t3371
        real t3373
        real t3374
        real t3375
        real t3388
        real t339
        real t3391
        real t3395
        real t3396
        real t34
        real t3401
        real t3402
        real t3404
        real t3406
        real t3408
        real t3410
        real t3412
        real t3414
        real t3417
        real t3423
        real t3424
        real t3427
        real t3432
        real t3434
        real t3437
        real t3438
        real t344
        real t3442
        real t3443
        real t3445
        real t3447
        real t3449
        real t345
        real t3450
        real t3451
        real t3453
        real t3455
        real t3458
        real t346
        real t3464
        real t3465
        real t3473
        real t3475
        real t348
        real t3481
        real t3488
        real t3492
        real t35
        real t3503
        real t3505
        real t3509
        real t3510
        real t3511
        real t3514
        real t3515
        real t3516
        real t3518
        real t352
        real t3523
        real t3524
        real t3525
        real t3534
        real t3535
        real t3537
        real t354
        real t3542
        real t3543
        real t3544
        real t3546
        real t3548
        real t3549
        real t3550
        real t3552
        real t3554
        real t3556
        real t3558
        real t3559
        real t356
        real t3562
        real t3565
        real t3567
        real t3568
        real t3569
        real t3570
        real t3571
        real t3572
        real t3574
        real t3576
        real t3578
        real t358
        real t3580
        real t3582
        real t3584
        real t3587
        real t3592
        real t3593
        real t3594
        real t36
        real t360
        real t3600
        real t3602
        real t3604
        real t3606
        real t3609
        real t361
        real t3610
        real t3611
        real t3613
        real t3615
        real t3617
        real t3619
        real t362
        real t3621
        real t3623
        real t3626
        real t363
        real t3630
        real t3631
        real t3632
        real t3633
        real t3639
        real t364
        real t3641
        real t3643
        real t3645
        real t3646
        real t3652
        real t3654
        real t3656
        real t3659
        real t366
        real t3665
        real t3667
        real t367
        real t3670
        real t3671
        real t3672
        real t3673
        real t3675
        real t3676
        real t3677
        real t3678
        real t368
        real t3680
        real t3682
        real t3683
        real t3684
        real t3685
        real t3686
        real t3687
        real t3689
        real t369
        real t3692
        real t3693
        real t3696
        real t3697
        real t3699
        real t3700
        real t3701
        real t3702
        real t3704
        real t3707
        real t3708
        real t371
        real t3710
        real t3712
        real t3713
        real t3714
        real t3716
        real t3717
        real t3723
        real t3725
        real t3726
        real t3727
        real t3728
        real t3729
        real t3730
        real t3732
        real t3734
        real t3736
        real t3738
        real t374
        real t3740
        real t3742
        real t3745
        real t375
        real t3750
        real t3751
        real t3752
        real t3753
        real t3758
        real t376
        real t3760
        real t3762
        real t3764
        real t3767
        real t3768
        real t3769
        real t377
        real t3771
        real t3773
        real t3775
        real t3777
        real t3779
        real t378
        real t3781
        real t3784
        real t3787
        real t3789
        real t3790
        real t3791
        real t3797
        real t3799
        real t38
        real t380
        real t3800
        real t3801
        real t3804
        real t3810
        real t3811
        real t3812
        real t3814
        real t3817
        real t3823
        real t3825
        real t3828
        real t3829
        real t383
        real t3830
        real t3831
        real t3833
        real t3834
        real t3835
        real t3836
        real t3838
        real t384
        real t3841
        real t3842
        real t3843
        real t3844
        real t3845
        real t3847
        real t3850
        real t3851
        real t3854
        real t3855
        real t3857
        real t3859
        real t386
        real t3861
        real t3865
        real t3866
        real t3868
        real t3870
        real t3872
        real t3874
        real t3876
        real t3877
        real t3878
        real t388
        real t3881
        real t3886
        real t3887
        real t3888
        real t3889
        real t3890
        real t3892
        real t3893
        real t3895
        real t3896
        real t3898
        real t3899
        real t3904
        real t3906
        real t3908
        real t391
        real t3910
        real t3912
        real t3913
        real t3918
        real t3920
        real t3921
        real t3923
        real t3925
        real t3927
        real t3929
        real t393
        real t3930
        real t3931
        real t3932
        real t3934
        real t3935
        real t3936
        real t3938
        real t394
        real t3940
        real t3942
        real t3944
        real t3947
        real t3952
        real t3953
        real t3954
        real t3956
        real t3958
        real t396
        real t3960
        real t3962
        real t3964
        real t3966
        real t3967
        real t3968
        real t3969
        real t397
        real t3970
        real t3972
        real t3975
        real t3976
        real t3978
        real t398
        real t3982
        real t3983
        real t3985
        real t3986
        real t3988
        real t3990
        real t3992
        real t3994
        real t3995
        real t3996
        real t3997
        real t3999
        real t4
        real t40
        real t400
        real t4000
        real t4001
        real t4003
        real t4005
        real t4007
        real t4009
        real t4012
        real t4017
        real t4018
        real t4019
        real t4025
        real t4027
        real t4029
        real t4031
        real t4033
        real t4034
        real t4035
        real t4036
        real t4037
        real t4038
        real t4040
        real t4042
        real t4044
        real t4046
        real t4048
        real t4050
        real t4051
        real t4056
        real t4057
        real t4058
        real t406
        real t4061
        real t4064
        real t4066
        real t4068
        real t4070
        real t4071
        real t4077
        real t4079
        real t408
        real t4081
        real t4083
        real t4085
        real t4086
        real t409
        real t4092
        real t4094
        real t4096
        real t4098
        real t4099
        real t4100
        real t4101
        real t4102
        real t4104
        real t4105
        real t4106
        real t4107
        real t4109
        real t411
        real t4112
        real t4113
        real t4114
        real t4115
        real t4116
        real t4118
        real t4121
        real t4122
        real t4124
        real t4125
        real t4126
        real t4127
        real t4128
        real t4129
        real t4130
        real t4132
        real t4134
        real t4136
        real t4138
        real t414
        real t4140
        real t4141
        real t4142
        real t4145
        real t4147
        real t4150
        real t4151
        real t4152
        real t4153
        real t4154
        real t4156
        real t4159
        real t416
        real t4160
        real t4162
        real t4163
        real t4168
        real t417
        real t4170
        real t4172
        real t4174
        real t4176
        real t4177
        real t4182
        real t4184
        real t4185
        real t4187
        real t4189
        real t419
        real t4191
        real t4193
        real t4194
        real t4195
        real t4196
        real t4198
        real t42
        real t4200
        real t4202
        real t4204
        real t4206
        real t4208
        real t421
        real t4211
        real t4216
        real t4217
        real t4218
        real t4222
        real t4224
        real t4226
        real t4228
        real t423
        real t4230
        real t4231
        real t4232
        real t4233
        real t4234
        real t4236
        real t4239
        real t4240
        real t4242
        real t4246
        real t4247
        real t4249
        real t425
        real t4250
        real t4252
        real t4254
        real t4256
        real t4258
        real t4259
        real t426
        real t4260
        real t4261
        real t4263
        real t4265
        real t4267
        real t4269
        real t427
        real t4271
        real t4273
        real t4276
        real t428
        real t4281
        real t4282
        real t4283
        real t4289
        real t4291
        real t4293
        real t4295
        real t4297
        real t4298
        real t4299
        real t430
        real t4300
        real t4302
        real t4304
        real t4305
        real t4306
        real t4308
        real t4310
        real t4312
        real t4314
        real t4315
        real t432
        real t4320
        real t4321
        real t4322
        real t4328
        real t4330
        real t4332
        real t4334
        real t4335
        real t434
        real t4341
        real t4343
        real t4345
        real t4347
        real t4349
        real t4350
        real t4356
        real t4358
        real t436
        real t4360
        real t4362
        real t4363
        real t4364
        real t4365
        real t4366
        real t4367
        real t4368
        real t4369
        real t4370
        real t4371
        real t4373
        real t4376
        real t4377
        real t4378
        real t4379
        real t438
        real t4380
        real t4382
        real t4385
        real t4386
        real t4388
        real t4389
        real t4390
        real t4391
        real t4392
        real t4394
        real t4396
        real t4399
        real t44
        real t440
        real t4400
        real t4401
        real t4403
        real t4405
        real t4407
        real t4409
        real t4411
        real t4413
        real t4416
        real t4422
        real t4423
        real t4424
        real t4425
        real t4428
        real t4429
        real t443
        real t4430
        real t4432
        real t4437
        real t4438
        real t4439
        real t4441
        real t4444
        real t4445
        real t4448
        real t4458
        real t4462
        real t4466
        real t4475
        real t4477
        real t4478
        real t448
        real t4483
        real t449
        real t4491
        real t4493
        real t4498
        real t450
        real t4500
        real t4502
        real t4503
        real t4511
        real t452
        real t4524
        real t4525
        real t4526
        real t4529
        real t4530
        real t4531
        real t4533
        real t4538
        real t4539
        real t4540
        real t4549
        real t4553
        real t4557
        real t456
        real t4561
        real t4565
        real t4568
        real t4571
        real t4572
        real t4574
        real t4576
        real t4577
        real t4578
        real t458
        real t4580
        real t4582
        real t4584
        real t4587
        real t4593
        real t4594
        real t46
        real t460
        real t4612
        real t4617
        real t462
        real t4623
        real t4624
        real t4625
        real t4634
        real t4635
        real t4638
        real t4639
        real t464
        real t4641
        real t4643
        real t4645
        real t4647
        real t4649
        real t4651
        real t4654
        real t466
        real t4660
        real t4661
        real t4662
        real t4663
        real t4664
        real t4666
        real t4667
        real t4668
        real t467
        real t4670
        real t4675
        real t4676
        real t4677
        real t4678
        real t4679
        real t468
        real t4682
        real t4683
        real t4686
        real t4688
        real t469
        real t4704
        real t471
        real t4713
        real t4715
        real t4716
        real t4720
        real t4721
        real t4728
        real t4729
        real t473
        real t4731
        real t4736
        real t4737
        real t4738
        real t4740
        real t4741
        real t4749
        real t475
        real t4762
        real t4763
        real t4764
        real t4767
        real t4768
        real t4769
        real t477
        real t4771
        real t4776
        real t4777
        real t4778
        real t4787
        real t479
        real t4791
        real t4795
        real t4798
        real t4799
        real t48
        real t4803
        real t4807
        real t4809
        real t481
        real t4810
        real t4812
        real t4814
        real t4816
        real t4817
        real t4818
        real t4820
        real t4822
        real t4825
        real t4826
        real t4831
        real t4832
        real t484
        real t4855
        real t4861
        real t4862
        real t4863
        real t4866
        real t4872
        real t4873
        real t4876
        real t4880
        real t4881
        real t4882
        real t4884
        real t4887
        real t4888
        real t489
        real t4890
        real t4896
        real t4898
        real t4899
        real t490
        real t4901
        real t4903
        real t4905
        real t4906
        real t491
        real t4912
        real t4914
        real t4917
        real t492
        real t4923
        real t4925
        real t4926
        real t4927
        real t4928
        real t4929
        real t4931
        real t4932
        real t4933
        real t4934
        real t4936
        real t4939
        real t4940
        real t4941
        real t4942
        real t4943
        real t4945
        real t4947
        real t4948
        real t4949
        real t4953
        real t4955
        real t4956
        real t4957
        real t4960
        real t4962
        real t4964
        real t4965
        real t4967
        real t4968
        real t4969
        real t497
        real t4970
        real t4971
        real t4973
        real t4974
        real t4975
        real t4976
        real t4978
        real t4981
        real t4982
        real t4984
        real t499
        real t4990
        real t4992
        real t4993
        real t4995
        real t4997
        real t4999
        integer t5
        real t5000
        real t5006
        real t5008
        real t501
        real t5011
        real t5017
        real t5020
        real t5021
        real t5022
        real t5023
        real t5025
        real t5026
        real t5027
        real t5028
        real t503
        real t5030
        real t5033
        real t5034
        real t5035
        real t5036
        real t5037
        real t5039
        real t5040
        real t5042
        real t5043
        real t5047
        real t5049
        real t505
        real t5051
        real t5053
        real t5054
        real t5056
        real t5058
        real t506
        real t5061
        real t5062
        real t5063
        real t5064
        real t5065
        real t5067
        real t5069
        real t507
        real t5071
        real t5075
        real t5076
        real t5078
        real t5080
        real t5082
        real t5084
        real t5086
        real t5088
        real t5091
        real t5096
        real t5097
        real t5098
        real t5099
        real t51
        real t5100
        real t5102
        real t5105
        real t5106
        real t5108
        real t5109
        real t5115
        real t5117
        real t5119
        real t512
        real t5121
        real t5123
        real t5124
        real t5129
        real t5131
        real t5133
        real t5135
        real t5137
        real t5138
        real t514
        real t5144
        real t5146
        real t5148
        real t5149
        real t5155
        real t5157
        real t5158
        real t5159
        real t516
        real t5160
        real t5161
        real t5163
        real t5164
        real t5165
        real t5166
        real t5168
        real t5171
        real t5172
        real t5173
        real t5174
        real t5175
        real t5177
        real t518
        real t5180
        real t5181
        real t5183
        real t5184
        real t5186
        real t5188
        real t5190
        real t5192
        real t5194
        real t5195
        real t5196
        real t5198
        real t520
        real t5200
        real t5202
        real t5204
        real t5205
        real t5206
        real t5207
        real t5209
        real t521
        real t5211
        real t5213
        real t5215
        real t5217
        real t5219
        real t522
        real t5222
        real t5227
        real t5228
        real t5229
        real t523
        real t5233
        real t5235
        real t5237
        real t5238
        real t5239
        real t5241
        real t5242
        real t5246
        real t5248
        real t5250
        real t5252
        real t5254
        real t5256
        real t5257
        real t5258
        real t5259
        real t5260
        real t5262
        real t5265
        real t5266
        real t5267
        real t5268
        real t5269
        real t5270
        real t5271
        real t5272
        real t5273
        real t5274
        real t5276
        real t5278
        real t5280
        real t5282
        real t5283
        real t5284
        real t5286
        real t5289
        real t529
        real t5291
        real t5294
        real t5295
        real t5296
        real t5297
        real t5298
        real t5300
        real t5303
        real t5304
        real t5306
        real t5307
        real t531
        real t5313
        real t5315
        real t5317
        real t5319
        real t5321
        real t5322
        real t5327
        real t5329
        real t533
        real t5331
        real t5333
        real t5335
        real t5336
        real t5342
        real t5344
        real t5346
        real t5347
        real t535
        real t5353
        real t5355
        real t5356
        real t5357
        real t5358
        real t5359
        real t5361
        real t5362
        real t5363
        real t5364
        real t5366
        real t5369
        real t537
        real t5370
        real t5371
        real t5372
        real t5373
        real t5375
        real t5378
        real t5379
        real t538
        real t5381
        real t5382
        real t5384
        real t5386
        real t5388
        real t539
        real t5390
        real t5392
        real t5393
        real t5394
        real t5396
        real t5398
        real t540
        real t5400
        real t5402
        real t5403
        real t5404
        real t5405
        real t5407
        real t5409
        real t541
        real t5411
        real t5413
        real t5415
        real t5417
        real t5420
        real t5425
        real t5426
        real t5427
        real t543
        real t5431
        real t5433
        real t5435
        real t5437
        real t5439
        real t544
        real t5440
        real t5444
        real t5446
        real t5448
        real t545
        real t5450
        real t5452
        real t5454
        real t5455
        real t5456
        real t5457
        real t5458
        real t546
        real t5460
        real t5463
        real t5464
        real t5466
        real t5467
        real t5468
        real t5469
        real t5470
        real t5472
        real t5474
        real t5477
        real t548
        real t5481
        real t5483
        real t5487
        real t5494
        real t5498
        real t5503
        real t5506
        real t5507
        real t5508
        real t551
        real t5511
        real t5512
        real t5513
        real t5514
        real t5515
        real t552
        real t5520
        real t5521
        real t5522
        real t5524
        real t5527
        real t5528
        real t553
        real t5534
        real t5539
        real t554
        real t5542
        real t5546
        real t555
        real t5551
        real t5554
        real t5555
        real t5556
        real t5558
        real t5560
        real t5562
        real t5564
        real t5566
        real t5568
        real t557
        real t5571
        real t5577
        real t5578
        real t5586
        real t5588
        real t5594
        real t5595
        real t5596
        real t56
        real t560
        real t5609
        real t561
        real t5613
        real t5619
        real t5620
        real t5622
        real t5624
        real t5626
        real t5628
        real t563
        real t5630
        real t5632
        real t5635
        real t564
        real t5641
        real t5642
        real t565
        real t5650
        real t5652
        real t5655
        real t5665
        real t5670
        real t5671
        real t5672
        real t5673
        real t5678
        real t568
        real t5682
        real t5683
        real t5685
        real t5686
        real t5687
        real t5688
        real t569
        real t5691
        real t5692
        real t5693
        real t5695
        real t57
        real t5700
        real t5701
        real t5702
        real t5704
        real t5707
        real t5708
        real t571
        real t5714
        real t5719
        real t572
        real t5722
        real t5726
        real t573
        real t5731
        real t5734
        real t5735
        real t5736
        real t5738
        real t574
        real t5740
        real t5742
        real t5744
        real t5746
        real t5748
        real t575
        real t5751
        real t5757
        real t5758
        real t5766
        real t5768
        real t5774
        real t5775
        real t5776
        real t5789
        real t579
        real t5793
        real t5799
        real t58
        real t580
        real t5800
        real t5802
        real t5804
        real t5806
        real t5808
        real t5810
        real t5812
        real t5815
        real t582
        real t5821
        real t5822
        real t5824
        real t583
        real t5830
        real t5832
        real t5840
        real t5845
        real t5847
        real t585
        real t5851
        real t5852
        real t5853
        real t5855
        real t5862
        real t5863
        real t5867
        real t587
        real t5871
        real t5875
        real t5876
        real t5877
        real t5880
        real t5881
        real t5882
        real t5884
        real t5889
        real t589
        real t5890
        real t5891
        real t5893
        real t5896
        real t5897
        real t59
        real t590
        real t5903
        real t5908
        real t591
        real t5911
        real t5915
        real t592
        real t5920
        real t5923
        real t5924
        real t5925
        real t5927
        real t5929
        real t5931
        real t5933
        real t5935
        real t5937
        real t5940
        real t5946
        real t5947
        real t5955
        real t5957
        real t596
        real t5963
        real t5964
        real t5965
        real t597
        real t5978
        real t5982
        real t5988
        real t5989
        real t599
        real t5991
        real t5993
        real t5995
        real t5997
        real t5999
        real t6
        real t60
        real t600
        real t6001
        real t6004
        real t6010
        real t6011
        real t6019
        real t602
        real t6021
        real t6034
        real t604
        real t6040
        real t6041
        real t6042
        real t6051
        real t6052
        real t6055
        real t6056
        real t6057
        real t606
        real t6060
        real t6061
        real t6062
        real t6064
        real t6069
        real t6070
        real t6071
        real t6073
        real t6076
        real t6077
        real t608
        real t6083
        real t6088
        real t609
        real t6091
        real t6095
        real t610
        real t6100
        real t6103
        real t6104
        real t6105
        real t6107
        real t6109
        real t611
        real t6111
        real t6113
        real t6115
        real t6117
        real t6120
        real t6126
        real t6127
        real t613
        real t6135
        real t6137
        real t6143
        real t6144
        real t6145
        real t6148
        real t615
        real t6154
        real t6158
        real t6162
        real t6168
        real t6169
        real t617
        real t6171
        real t6173
        real t6175
        real t6177
        real t6179
        real t6181
        real t6184
        real t619
        real t6190
        real t6191
        real t6197
        real t6199
        real t62
        real t6201
        real t621
        real t6214
        real t6220
        real t6221
        real t6222
        real t623
        real t6231
        real t6232
        real t6236
        real t6244
        real t6245
        real t6251
        real t6258
        real t626
        real t6271
        real t6275
        real t6284
        real t629
        real t6294
        real t6295
        real t63
        real t6304
        real t6305
        real t6307
        real t6309
        real t631
        real t6314
        real t632
        real t6320
        real t6322
        real t6325
        real t6328
        real t6329
        real t633
        real t6333
        real t6337
        real t6339
        real t6340
        real t6344
        real t6349
        real t6355
        real t637
        real t6371
        real t6375
        real t6376
        real t6381
        real t6384
        real t639
        real t6394
        real t6398
        integer t64
        real t6401
        real t6405
        real t6409
        real t641
        real t6411
        real t6412
        real t6413
        real t6414
        real t6416
        real t6417
        real t6418
        real t6419
        real t6422
        real t6423
        real t6424
        real t6425
        real t643
        real t6433
        real t6436
        real t6440
        real t6443
        real t6445
        real t6448
        real t645
        real t6451
        real t6455
        real t6457
        real t6460
        real t6463
        real t6465
        real t6467
        real t6469
        real t647
        real t6471
        real t6476
        real t6477
        real t6481
        real t6486
        real t6487
        real t6488
        real t6489
        real t649
        real t6491
        real t6498
        real t6499
        real t65
        real t650
        real t6500
        real t6504
        real t6506
        real t6507
        real t6508
        real t6509
        real t651
        real t6511
        real t6512
        real t6513
        real t6514
        real t6517
        real t6518
        real t6519
        real t652
        real t6520
        real t6528
        real t6531
        real t6535
        real t6537
        real t654
        integer t6542
        real t6543
        real t6545
        real t6547
        real t6548
        real t6549
        real t6550
        real t6553
        real t6554
        real t6556
        real t6558
        real t656
        real t6560
        real t6562
        real t6564
        real t6566
        real t6569
        real t6574
        real t6575
        real t6576
        real t6577
        real t6578
        real t658
        real t6580
        real t6583
        real t6584
        real t6585
        real t6586
        real t6587
        real t6588
        real t6591
        real t6595
        real t6597
        real t6598
        real t6599
        real t66
        real t660
        real t6601
        real t6603
        real t6606
        real t6608
        real t6612
        real t6615
        real t6616
        real t6617
        real t662
        real t6620
        real t6624
        real t6625
        real t6627
        real t6628
        real t6630
        real t6632
        real t6634
        real t6636
        real t6638
        real t664
        real t6640
        real t6645
        real t6646
        real t6648
        real t6650
        real t6651
        real t6655
        real t6658
        real t6662
        real t6664
        real t6665
        real t6667
        real t667
        real t6670
        real t6674
        real t6676
        real t6682
        real t6683
        real t6686
        real t669
        real t6690
        real t6693
        real t6695
        real t6696
        real t6698
        real t6701
        real t6705
        real t6707
        real t6715
        real t6716
        real t6717
        real t6719
        real t672
        real t6721
        real t6723
        real t6725
        real t6727
        real t6729
        real t673
        real t6731
        real t6736
        real t6739
        real t674
        real t6743
        real t6745
        real t6751
        real t6753
        real t6755
        real t6757
        real t6759
        real t6764
        real t6765
        real t6769
        real t6774
        real t6775
        real t6776
        real t6777
        real t6779
        real t678
        real t6786
        real t6787
        real t6788
        real t6792
        real t6795
        real t6799
        real t68
        real t680
        real t6802
        real t6804
        real t6806
        real t6809
        real t6813
        real t6815
        real t682
        real t6821
        real t6823
        real t6825
        real t6827
        real t6829
        real t6834
        real t6836
        real t6837
        real t6838
        real t6839
        real t684
        real t6841
        real t6843
        real t6844
        real t6845
        real t6848
        real t6849
        real t6852
        real t6853
        real t6855
        real t6856
        real t6858
        real t6859
        real t686
        real t6861
        real t6863
        real t6864
        real t6868
        real t687
        real t6870
        real t6872
        real t6875
        real t6877
        real t6878
        real t688
        real t6880
        real t6882
        real t6884
        real t6886
        real t6888
        real t6889
        real t689
        real t6890
        real t6895
        real t6896
        real t6898
        real t6899
        real t690
        real t6900
        real t6901
        real t6903
        real t6904
        real t6905
        real t6906
        real t6909
        real t6911
        real t6912
        real t6913
        real t6914
        real t6916
        real t6917
        real t6918
        real t692
        real t6924
        real t6926
        real t6928
        real t693
        real t6930
        real t6932
        real t6934
        real t6936
        real t6938
        real t6939
        real t694
        real t6941
        real t6943
        real t6945
        real t6947
        real t6949
        real t695
        real t6951
        real t6956
        real t6957
        real t6960
        real t6962
        real t6964
        real t6965
        real t6966
        real t6967
        real t6969
        real t697
        real t6970
        real t6971
        real t6972
        real t6975
        real t6977
        real t6978
        real t6979
        real t6980
        real t6982
        real t6983
        real t6984
        real t6985
        real t6990
        real t6992
        real t6993
        real t6995
        real t6997
        real t6998
        real t6999
        real t7
        real t70
        real t700
        real t7001
        real t7003
        real t7005
        real t7007
        real t7008
        real t701
        real t7010
        real t7011
        real t7013
        real t7015
        real t7017
        real t7019
        real t702
        real t7021
        real t7023
        real t7028
        real t7029
        real t703
        real t7032
        real t7034
        real t7035
        real t7038
        real t704
        real t7040
        real t7041
        real t7042
        real t7048
        real t7050
        real t7053
        real t7055
        real t7057
        real t706
        real t7060
        real t7064
        real t7066
        real t7067
        real t7069
        real t7072
        real t7076
        real t7078
        real t708
        real t7084
        real t7086
        real t7087
        real t709
        real t7091
        real t7094
        real t7096
        real t7098
        real t710
        real t7101
        real t7105
        real t7107
        real t7112
        real t7114
        real t7115
        real t7117
        real t7119
        real t712
        real t7121
        real t7122
        real t7123
        real t7125
        real t7127
        real t7129
        real t7130
        real t7132
        real t7133
        real t7135
        real t7137
        real t7139
        real t7140
        real t7141
        real t7143
        real t7145
        real t715
        real t7151
        real t7152
        real t7154
        real t7156
        real t7159
        real t716
        real t7163
        real t7165
        real t717
        real t7171
        real t7173
        real t7175
        real t7177
        real t7179
        real t7181
        real t7183
        real t7185
        real t7187
        real t7189
        real t719
        real t7191
        real t7193
        real t7195
        real t72
        real t720
        real t7201
        real t7204
        real t7206
        real t7208
        real t7209
        real t7213
        real t7216
        real t7218
        real t722
        real t7220
        real t7222
        real t7223
        real t7225
        real t7228
        real t7232
        real t7234
        real t724
        real t7240
        real t7242
        real t7244
        real t7246
        real t7248
        real t7250
        real t7252
        real t7254
        real t7258
        real t726
        real t7261
        real t7263
        real t7266
        real t7270
        real t7272
        real t7273
        real t7275
        real t7277
        real t7280
        real t7282
        real t7285
        real t7289
        real t7291
        real t7296
        real t7298
        real t730
        real t7300
        real t7301
        real t7302
        real t7304
        real t7306
        real t7307
        real t7308
        real t731
        real t7310
        real t7312
        real t7313
        real t7315
        real t7317
        real t7319
        real t732
        real t7320
        real t7321
        real t7322
        real t7323
        real t7325
        real t7326
        real t7327
        real t7328
        real t7330
        real t7333
        real t7334
        real t7335
        real t7336
        real t7337
        real t7339
        real t734
        real t7342
        real t7343
        real t7345
        real t735
        real t7351
        real t7354
        real t7357
        real t7359
        real t7360
        real t7366
        real t7368
        real t7369
        real t7370
        real t7371
        real t7373
        real t7375
        real t7377
        real t7378
        real t7380
        real t7382
        real t7384
        real t7385
        real t7387
        real t739
        real t7391
        real t7393
        real t7395
        real t7396
        real t74
        real t740
        real t7402
        real t7404
        real t7405
        real t7406
        real t7407
        real t7408
        real t7410
        real t7411
        real t7412
        real t7413
        real t7415
        real t7418
        real t7419
        real t742
        real t7420
        real t7421
        real t7422
        real t7424
        real t7427
        real t7428
        real t743
        real t7430
        real t7431
        real t7432
        real t7433
        real t7434
        real t7436
        real t7437
        real t7439
        real t7445
        real t7446
        real t7449
        real t745
        real t7450
        real t7452
        real t7454
        real t7456
        real t7458
        real t7460
        real t7462
        real t7465
        real t7469
        real t747
        real t7470
        real t7471
        real t7472
        real t7473
        real t7474
        real t7476
        real t7479
        real t7480
        real t7482
        real t7483
        real t7486
        real t7488
        real t749
        real t7490
        real t7492
        real t7494
        real t7496
        real t7497
        real t7498
        real t7502
        real t7504
        real t7505
        real t7507
        real t7509
        real t751
        real t7510
        real t7511
        real t7513
        real t7514
        real t7515
        real t7516
        real t7518
        real t752
        real t7520
        real t7522
        real t7524
        real t7526
        real t7528
        real t753
        real t7531
        real t7536
        real t7537
        real t7538
        real t754
        real t7544
        real t7546
        real t7548
        real t7550
        real t7551
        real t7552
        real t7553
        real t7554
        real t7556
        real t7559
        real t756
        real t7560
        real t7562
        real t7567
        real t7569
        real t7570
        real t7572
        real t7574
        real t7576
        real t7578
        real t7579
        real t758
        real t7580
        real t7581
        real t7583
        real t7585
        real t7586
        real t7587
        real t7589
        real t7591
        real t7593
        real t7596
        real t7599
        real t76
        real t760
        real t7601
        real t7602
        real t7603
        real t7609
        real t7611
        real t7613
        real t7615
        real t7618
        real t7619
        real t762
        real t7620
        real t7622
        real t7624
        real t7626
        real t7628
        real t7630
        real t7632
        real t7635
        real t764
        real t7640
        real t7641
        real t7642
        real t7648
        real t7650
        real t7652
        real t7655
        real t7656
        real t766
        real t7661
        real t7663
        real t7665
        real t7667
        real t7668
        real t7670
        real t7675
        real t7676
        real t7678
        real t7680
        real t7683
        real t7684
        real t7685
        real t7686
        real t7688
        real t7689
        real t769
        real t7690
        real t7691
        real t7692
        real t7693
        real t7696
        real t7697
        real t7698
        real t7699
        real t7700
        real t7702
        real t7705
        real t7706
        real t7709
        real t771
        real t7710
        real t7712
        real t7713
        real t7714
        real t7716
        real t7718
        real t7720
        real t7722
        real t7724
        real t7726
        real t7729
        real t7734
        real t7735
        real t7736
        real t7737
        real t7738
        real t774
        real t7740
        real t7743
        real t7744
        real t7746
        real t7747
        real t775
        real t7752
        real t7754
        real t7756
        real t7758
        real t776
        real t7760
        real t7761
        real t7766
        real t7767
        real t7768
        real t7769
        real t7771
        real t7773
        real t7775
        real t7777
        real t7778
        real t7779
        real t7780
        real t7782
        real t7784
        real t7786
        real t7787
        real t7788
        real t7790
        real t7792
        real t7795
        real t78
        real t780
        real t7800
        real t7801
        real t7802
        real t7808
        real t7810
        real t7812
        real t7814
        real t7815
        real t7816
        real t7817
        real t7818
        real t782
        real t7820
        real t7823
        real t7824
        real t7826
        real t7831
        real t7833
        real t7834
        real t7836
        real t7838
        real t784
        real t7840
        real t7842
        real t7843
        real t7844
        real t7845
        real t7847
        real t7849
        real t7851
        real t7853
        real t7855
        real t7857
        real t786
        real t7860
        real t7865
        real t7866
        real t7867
        real t7873
        real t7874
        real t7875
        real t7877
        real t7879
        real t788
        real t7882
        real t7883
        real t7884
        real t7886
        real t7888
        real t7890
        real t7892
        real t7894
        real t7896
        real t7897
        real t7899
        real t790
        real t7904
        real t7905
        real t7906
        real t791
        real t7912
        real t7914
        real t7916
        real t7919
        real t792
        real t7925
        real t7927
        real t7928
        real t7929
        real t793
        real t7931
        real t7934
        real t7939
        real t7940
        real t7942
        real t7944
        real t7947
        real t7948
        real t7949
        real t795
        real t7950
        real t7952
        real t7953
        real t7954
        real t7955
        real t7957
        real t7960
        real t7961
        real t7962
        real t7963
        real t7964
        real t7966
        real t7969
        real t797
        real t7970
        real t7973
        real t7974
        real t7976
        real t7978
        real t7980
        real t7983
        real t7984
        real t7985
        real t7987
        real t7989
        real t799
        real t7991
        real t7993
        real t7995
        real t7997
        real t8000
        real t8005
        real t8006
        real t8007
        real t8008
        real t8009
        real t801
        real t8011
        real t8013
        real t8014
        real t8015
        real t8017
        real t8018
        real t8024
        real t8026
        real t8028
        real t803
        real t8030
        real t8032
        real t8033
        real t8035
        real t8038
        real t8040
        real t8042
        real t8044
        real t8046
        real t8047
        real t805
        real t8053
        real t8055
        real t8058
        real t8064
        real t8067
        real t8068
        real t8069
        real t8070
        real t8071
        real t8072
        real t8073
        real t8074
        real t8075
        real t8077
        real t808
        real t8080
        real t8081
        real t8082
        real t8083
        real t8084
        real t8086
        real t8089
        real t809
        real t8090
        real t8091
        real t8093
        real t8095
        real t8097
        real t8099
        real t81
        real t8101
        real t8104
        real t8105
        real t8107
        real t8109
        real t8111
        real t8114
        real t8115
        real t8116
        real t8118
        real t8120
        real t8122
        real t8124
        real t8126
        real t8128
        real t813
        real t8131
        real t8136
        real t8137
        real t8138
        real t814
        real t8144
        real t8146
        real t8148
        real t815
        real t8150
        real t8151
        real t8157
        real t8158
        real t8159
        real t8161
        real t8163
        real t8165
        real t8166
        real t8167
        real t8168
        real t8169
        real t8171
        real t8174
        real t8175
        real t8177
        real t8178
        real t8179
        real t8181
        real t8182
        real t8183
        real t8185
        real t8187
        real t8188
        real t8189
        real t819
        real t8191
        real t8193
        real t8195
        real t8198
        real t8203
        real t8204
        real t8205
        real t8206
        real t8207
        real t8209
        real t821
        real t8212
        real t8213
        real t8215
        real t8216
        real t822
        real t8222
        real t8224
        real t8225
        real t8226
        real t8228
        real t823
        real t8230
        real t8231
        real t8236
        real t8238
        real t8239
        real t8240
        real t8242
        real t8244
        real t8245
        real t825
        real t8251
        real t8253
        real t8256
        real t8262
        real t8265
        real t8266
        real t8267
        real t8268
        real t827
        real t8270
        real t8271
        real t8272
        real t8273
        real t8275
        real t8278
        real t8279
        real t828
        real t8280
        real t8281
        real t8282
        real t8284
        real t8287
        real t8288
        real t8291
        real t8293
        real t8295
        real t8297
        real t8299
        real t8302
        real t8303
        real t8305
        real t8307
        real t8308
        real t8309
        real t8312
        real t8313
        real t8314
        real t8316
        real t8318
        real t832
        real t8320
        real t8322
        real t8324
        real t8326
        real t8329
        real t8330
        real t8334
        real t8335
        real t8336
        real t834
        real t8342
        real t8344
        real t8346
        real t8348
        real t8349
        real t8355
        real t8357
        real t8359
        real t836
        real t8361
        real t8363
        real t8364
        real t8365
        real t8366
        real t8367
        real t8368
        real t8369
        real t837
        real t8372
        real t8373
        real t8375
        real t8376
        real t8377
        real t8379
        real t838
        real t8381
        real t8383
        real t8384
        real t8386
        real t8388
        real t8390
        real t8392
        real t8394
        real t8397
        real t8399
        real t840
        real t8401
        real t8403
        real t8406
        real t8407
        real t8408
        real t8411
        real t8412
        real t8413
        real t8415
        real t8418
        real t8419
        real t842
        real t8423
        real t8426
        real t8428
        real t8431
        real t8432
        real t8433
        real t8435
        real t8437
        real t8439
        real t844
        real t8441
        real t8443
        real t8445
        real t8448
        real t845
        real t8453
        real t8454
        real t8455
        real t8461
        real t8463
        real t8465
        real t8467
        real t8468
        real t8469
        real t8470
        real t8471
        real t8473
        real t8476
        real t8477
        real t8479
        real t8484
        real t8486
        real t8488
        real t849
        real t8490
        real t8492
        real t8493
        real t8494
        real t8495
        real t8497
        real t8499
        real t8501
        real t8503
        real t8505
        real t8507
        real t851
        real t8510
        real t8515
        real t8516
        real t8517
        real t8523
        real t8525
        real t8527
        real t8529
        real t853
        real t8530
        real t8536
        real t8538
        real t8540
        real t8542
        real t8543
        real t8544
        real t8545
        real t8546
        real t8548
        real t855
        real t8551
        real t8552
        real t8554
        real t8555
        real t8556
        real t8558
        real t8559
        real t8560
        real t8561
        real t8563
        real t8566
        real t8567
        real t857
        real t8571
        real t8574
        real t8576
        real t8579
        real t8580
        real t8581
        real t8583
        real t8585
        real t8587
        real t8589
        real t859
        real t8591
        real t8593
        real t8596
        real t86
        real t860
        real t8601
        real t8602
        real t8603
        real t8609
        real t861
        real t8611
        real t8613
        real t8615
        real t8616
        real t8617
        real t8618
        real t8619
        real t862
        real t8621
        real t8624
        real t8625
        real t8627
        real t863
        real t8632
        real t8634
        real t8636
        real t8638
        real t8640
        real t8641
        real t8642
        real t8643
        real t8645
        real t8647
        real t8649
        real t865
        real t8651
        real t8653
        real t8655
        real t8658
        real t866
        real t8663
        real t8664
        real t8665
        real t867
        real t8671
        real t8673
        real t8675
        real t8677
        real t8678
        real t868
        real t8684
        real t8686
        real t8688
        real t8690
        real t8691
        real t8692
        real t8693
        real t8694
        real t8696
        real t8699
        real t87
        real t870
        real t8700
        real t8702
        real t8703
        real t8704
        real t8706
        real t8708
        real t8710
        real t8712
        real t8715
        real t8716
        real t8717
        real t8718
        real t8720
        real t8723
        real t8724
        real t8728
        real t873
        real t8731
        real t8733
        real t8736
        real t8737
        real t8738
        real t874
        real t8740
        real t8742
        real t8744
        real t8746
        real t8748
        real t875
        real t8750
        real t8753
        real t8758
        real t8759
        real t876
        real t8760
        real t8766
        real t8768
        real t877
        real t8770
        real t8772
        real t8773
        real t8774
        real t8775
        real t8776
        real t8778
        real t8781
        real t8782
        real t8784
        real t8789
        real t879
        real t8791
        real t8793
        real t8795
        real t8797
        real t8798
        real t8799
        real t88
        real t8800
        real t8802
        real t8804
        real t8806
        real t8808
        real t8810
        real t8812
        real t8815
        real t882
        real t8820
        real t8821
        real t8822
        real t8828
        real t883
        real t8830
        real t8832
        real t8834
        real t8835
        real t8841
        real t8843
        real t8845
        real t8847
        real t8848
        real t8849
        real t885
        real t8850
        real t8851
        real t8853
        real t8856
        real t8857
        real t8859
        real t886
        real t8860
        real t8861
        real t8863
        real t8864
        real t8865
        real t8866
        real t8868
        real t887
        real t8871
        real t8872
        real t8876
        real t8879
        real t888
        real t8881
        real t8884
        real t8885
        real t8886
        real t8888
        real t8890
        real t8892
        real t8894
        real t8896
        real t8898
        real t89
        real t890
        real t8901
        real t8906
        real t8907
        real t8908
        real t8914
        real t8916
        real t8918
        real t8920
        real t8921
        real t8922
        real t8923
        real t8924
        real t8926
        real t8929
        real t893
        real t8930
        real t8932
        real t8937
        real t8939
        real t8941
        real t8943
        real t8945
        real t8946
        real t8947
        real t8948
        real t895
        real t8950
        real t8952
        real t8954
        real t8956
        real t8958
        real t896
        real t8960
        real t8963
        real t8968
        real t8969
        real t897
        real t8970
        real t8976
        real t8978
        real t8980
        real t8982
        real t8983
        real t8989
        real t899
        real t8991
        real t8993
        real t8995
        real t8996
        real t8997
        real t8998
        real t8999
        real t9
        real t90
        real t900
        real t9001
        real t9004
        real t9005
        real t9007
        real t9008
        real t9009
        real t9011
        real t9013
        real t9015
        real t9018
        real t902
        real t9020
        real t9022
        real t9024
        real t9026
        real t9029
        real t903
        real t9031
        real t9033
        real t9035
        real t9038
        real t9040
        real t9042
        real t9044
        real t9046
        real t9048
        real t905
        real t9051
        real t9053
        real t9055
        real t9057
        real t9059
        real t9062
        real t9063
        real t9064
        real t9067
        real t9068
        real t907
        real t9070
        real t9071
        real t9072
        real t9074
        real t9076
        real t9078
        real t9080
        real t9082
        real t9083
        real t9085
        real t9087
        real t9089
        real t909
        real t9090
        real t9091
        real t9092
        real t9094
        real t9095
        real t9097
        real t9098
        real t910
        real t9100
        real t9102
        real t9104
        real t9106
        real t9108
        real t9109
        real t9110
        real t9112
        real t9113
        real t9115
        real t9117
        real t9119
        real t912
        real t9121
        real t9122
        real t9124
        real t9126
        real t9128
        real t913
        real t9130
        real t9131
        real t9133
        real t9135
        real t9137
        real t9138
        real t9140
        real t9142
        real t9144
        real t9146
        real t9148
        real t915
        real t9150
        real t9151
        real t9153
        real t9155
        real t9157
        real t9159
        real t9161
        real t9162
        real t9163
        real t9164
        real t9166
        real t9167
        real t9168
        real t9169
        real t917
        real t9170
        real t9173
        real t9175
        real t9177
        real t9179
        real t9180
        real t9184
        real t9185
        real t9186
        real t919
        real t9196
        real t9197
        real t92
        real t9201
        real t9202
        real t9204
        real t9205
        real t9208
        real t9209
        real t921
        real t9212
        real t9214
        real t9216
        real t922
        real t9221
        real t9228
        real t923
        real t9230
        real t9232
        real t9234
        real t9236
        real t9238
        real t9239
        real t9242
        real t9244
        real t925
        real t9250
        real t9252
        real t9257
        real t9259
        real t926
        real t9260
        real t9264
        real t9272
        real t9277
        real t9279
        real t928
        real t9280
        real t9283
        real t9284
        real t9290
        real t930
        real t9301
        real t9302
        real t9303
        real t9306
        real t9307
        real t9308
        real t9309
        real t9310
        real t9314
        real t9316
        real t932
        real t9320
        real t9323
        real t9324
        real t9331
        real t9336
        real t9338
        real t934
        real t9343
        real t9345
        real t935
        real t9351
        real t9353
        real t9355
        real t9358
        real t936
        real t9360
        real t9362
        real t9363
        real t9367
        real t9369
        real t9372
        real t9374
        real t9375
        real t9379
        real t938
        real t9381
        real t9383
        real t9387
        real t939
        real t9392
        real t9394
        real t9396
        real t94
        real t9403
        real t9407
        real t941
        real t9412
        real t9422
        real t9423
        real t9427
        real t9429
        real t943
        real t9435
        real t9437
        real t9439
        real t9442
        real t9444
        real t9446
        real t9447
        real t945
        real t9451
        real t9453
        real t9456
        real t9458
        real t9459
        real t946
        real t9463
        real t9465
        real t9467
        real t9471
        real t9476
        real t9478
        real t948
        real t9480
        real t9487
        real t949
        real t9491
        real t9496
        real t9506
        real t9507
        real t951
        real t9510
        real t9512
        real t9514
        real t9516
        real t9518
        real t9519
        real t9521
        real t9523
        real t9524
        real t9525
        real t9526
        real t9528
        real t953
        real t9530
        real t9532
        real t9534
        real t9536
        real t9537
        real t9539
        real t9541
        real t9543
        real t9545
        real t9546
        real t9548
        real t955
        real t9550
        real t9552
        real t9554
        real t9555
        real t9557
        real t9559
        real t9561
        real t9562
        real t9563
        real t9564
        real t9566
        real t9567
        real t9568
        real t9569
        real t957
        real t9570
        real t9571
        real t9573
        real t9575
        real t9577
        real t9579
        real t958
        real t9580
        real t9582
        real t9584
        real t9585
        real t9586
        real t9587
        real t9589
        real t959
        real t9591
        real t9593
        real t9595
        real t9597
        real t9598
        integer t96
        real t9600
        real t9602
        real t9604
        real t9606
        real t9607
        real t9609
        real t961
        real t9611
        real t9613
        real t9615
        real t9616
        real t9618
        real t962
        real t9620
        real t9621
        real t9622
        real t9623
        real t9624
        real t9625
        real t9627
        real t9628
        real t9629
        real t9630
        real t9631
        real t9633
        real t9639
        real t964
        real t9643
        real t9646
        real t9649
        real t9651
        real t9653
        real t966
        real t9660
        real t9662
        real t9663
        real t9666
        real t9667
        real t9673
        real t968
        real t9682
        real t9684
        real t9685
        real t9686
        real t9689
        real t9690
        real t9691
        real t9692
        real t9693
        real t9694
        real t9697
        real t9699
        real t97
        real t970
        real t9703
        real t9706
        real t9707
        real t971
        real t9715
        real t9719
        real t9724
        real t9726
        real t9727
        real t973
        real t9731
        real t9739
        real t9744
        real t9746
        real t9747
        real t975
        real t9750
        real t9751
        real t9757
        real t9768
        real t9769
        real t977
        real t9770
        real t9773
        real t9774
        real t9775
        real t9776
        real t9777
        real t9781
        real t9783
        real t9787
        real t979
        real t9790
        real t9791
        real t9798
        real t98
        real t9803
        real t9805
        real t981
        real t9810
        real t9812
        real t9816
        real t9818
        real t9821
        real t9823
        real t9824
        real t983
        real t9830
        real t9832
        real t9834
        real t9837
        real t9839
        real t9841
        real t9842
        real t9846
        real t985
        real t986
        real t9860
        real t9864
        real t9869
        real t9877
        real t9878
        real t988
        real t9882
        real t9884
        real t9888
        real t9890
        real t9892
        real t9893
        real t9895
        real t9896
        real t990
        real t9902
        real t9904
        real t9906
        real t9908
        real t9909
        real t9911
        real t9913
        real t9914
        real t9918
        real t992
        real t9932
        real t9936
        real t994
        real t9941
        real t9949
        real t9950
        real t9953
        real t9955
        real t9957
        real t9959
        real t996
        real t9960
        real t9961
        real t9962
        real t9964
        real t9966
        real t9967
        real t9969
        real t997
        real t9970
        real t9971
        real t9973
        real t9974
        real t9976
        real t9978
        real t9979
        real t998
        real t9980
        real t9981
        real t9983
        real t9985
        real t9987
        real t9989
        real t999
        real t9991
        real t9992
        real t9994
        real t9996
        real t9998
        real t9999
        t1 = u(i,j,k,n)
        t2 = ut(i,j,k,n)
        t4 = cc ** 2
        t5 = i + 1
        t6 = rx(t5,j,k,0,0)
        t7 = rx(t5,j,k,1,1)
        t9 = rx(t5,j,k,2,2)
        t11 = rx(t5,j,k,1,2)
        t13 = rx(t5,j,k,2,1)
        t15 = rx(t5,j,k,1,0)
        t17 = rx(t5,j,k,0,2)
        t19 = rx(t5,j,k,0,1)
        t22 = rx(t5,j,k,2,0)
        t27 = t6 * t7 * t9 - t6 * t11 * t13 + t15 * t13 * t17 - t15 * t1
     #9 * t9 + t22 * t19 * t11 - t22 * t7 * t17
        t28 = 0.1E1 / t27
        t29 = t6 ** 2
        t30 = t19 ** 2
        t31 = t17 ** 2
        t33 = t28 * (t29 + t30 + t31)
        t34 = t33 / 0.2E1
        t35 = rx(i,j,k,0,0)
        t36 = rx(i,j,k,1,1)
        t38 = rx(i,j,k,2,2)
        t40 = rx(i,j,k,1,2)
        t42 = rx(i,j,k,2,1)
        t44 = rx(i,j,k,1,0)
        t46 = rx(i,j,k,0,2)
        t48 = rx(i,j,k,0,1)
        t51 = rx(i,j,k,2,0)
        t56 = t35 * t36 * t38 - t35 * t40 * t42 + t44 * t42 * t46 - t44 
     #* t48 * t38 + t51 * t48 * t40 - t51 * t36 * t46
        t57 = 0.1E1 / t56
        t58 = t35 ** 2
        t59 = t48 ** 2
        t60 = t46 ** 2
        t62 = t57 * (t58 + t59 + t60)
        t63 = t62 / 0.2E1
        t64 = i + 2
        t65 = rx(t64,j,k,0,0)
        t66 = rx(t64,j,k,1,1)
        t68 = rx(t64,j,k,2,2)
        t70 = rx(t64,j,k,1,2)
        t72 = rx(t64,j,k,2,1)
        t74 = rx(t64,j,k,1,0)
        t76 = rx(t64,j,k,0,2)
        t78 = rx(t64,j,k,0,1)
        t81 = rx(t64,j,k,2,0)
        t86 = t65 * t66 * t68 - t65 * t70 * t72 + t74 * t72 * t76 - t74 
     #* t78 * t68 + t81 * t78 * t70 - t81 * t66 * t76
        t87 = 0.1E1 / t86
        t88 = t65 ** 2
        t89 = t78 ** 2
        t90 = t76 ** 2
        t92 = t87 * (t88 + t89 + t90)
        t94 = 0.1E1 / dx
        t96 = i - 1
        t97 = rx(t96,j,k,0,0)
        t98 = rx(t96,j,k,1,1)
        t100 = rx(t96,j,k,2,2)
        t102 = rx(t96,j,k,1,2)
        t104 = rx(t96,j,k,2,1)
        t106 = rx(t96,j,k,1,0)
        t108 = rx(t96,j,k,0,2)
        t110 = rx(t96,j,k,0,1)
        t113 = rx(t96,j,k,2,0)
        t118 = t97 * t98 * t100 - t97 * t102 * t104 + t106 * t104 * t108
     # - t106 * t110 * t100 + t113 * t110 * t102 - t113 * t98 * t108
        t119 = 0.1E1 / t118
        t120 = t97 ** 2
        t121 = t110 ** 2
        t122 = t108 ** 2
        t124 = t119 * (t120 + t121 + t122)
        t126 = (t62 - t124) * t94
        t131 = t34 + t63 - dx * ((t92 - t33) * t94 / 0.2E1 - t126 / 0.2E
     #1) / 0.8E1
        t132 = t4 * t131
        t133 = sqrt(0.3E1)
        t134 = t133 / 0.6E1
        t135 = 0.1E1 / 0.2E1 + t134
        t136 = t135 * dt
        t137 = ut(t5,j,k,n)
        t139 = (t137 - t2) * t94
        t140 = ut(t64,j,k,n)
        t142 = (t140 - t137) * t94
        t144 = (t142 - t139) * t94
        t145 = ut(t96,j,k,n)
        t147 = (t2 - t145) * t94
        t149 = (t139 - t147) * t94
        t150 = t144 - t149
        t153 = t139 - dx * t150 / 0.24E2
        t158 = t4 * (t33 / 0.2E1 + t62 / 0.2E1)
        t159 = t135 ** 2
        t161 = dt ** 2
        t164 = t4 * (t92 / 0.2E1 + t33 / 0.2E1)
        t165 = u(t64,j,k,n)
        t166 = u(t5,j,k,n)
        t168 = (t165 - t166) * t94
        t169 = t164 * t168
        t171 = (t166 - t1) * t94
        t172 = t158 * t171
        t174 = (t169 - t172) * t94
        t175 = t4 * t87
        t179 = t65 * t74 + t78 * t66 + t76 * t70
        t180 = j + 1
        t181 = u(t64,t180,k,n)
        t183 = 0.1E1 / dy
        t184 = (t181 - t165) * t183
        t185 = j - 1
        t186 = u(t64,t185,k,n)
        t188 = (t165 - t186) * t183
        t190 = t184 / 0.2E1 + t188 / 0.2E1
        t178 = t175 * t179
        t192 = t178 * t190
        t193 = t4 * t28
        t197 = t6 * t15 + t19 * t7 + t17 * t11
        t198 = u(t5,t180,k,n)
        t200 = (t198 - t166) * t183
        t201 = u(t5,t185,k,n)
        t203 = (t166 - t201) * t183
        t205 = t200 / 0.2E1 + t203 / 0.2E1
        t196 = t193 * t197
        t207 = t196 * t205
        t209 = (t192 - t207) * t94
        t210 = t209 / 0.2E1
        t211 = t4 * t57
        t215 = t35 * t44 + t48 * t36 + t46 * t40
        t216 = u(i,t180,k,n)
        t218 = (t216 - t1) * t183
        t219 = u(i,t185,k,n)
        t221 = (t1 - t219) * t183
        t223 = t218 / 0.2E1 + t221 / 0.2E1
        t214 = t211 * t215
        t225 = t214 * t223
        t227 = (t207 - t225) * t94
        t228 = t227 / 0.2E1
        t232 = t65 * t81 + t78 * t72 + t76 * t68
        t233 = k + 1
        t234 = u(t64,j,t233,n)
        t236 = 0.1E1 / dz
        t237 = (t234 - t165) * t236
        t238 = k - 1
        t239 = u(t64,j,t238,n)
        t241 = (t165 - t239) * t236
        t243 = t237 / 0.2E1 + t241 / 0.2E1
        t231 = t175 * t232
        t245 = t231 * t243
        t249 = t6 * t22 + t19 * t13 + t17 * t9
        t250 = u(t5,j,t233,n)
        t252 = (t250 - t166) * t236
        t253 = u(t5,j,t238,n)
        t255 = (t166 - t253) * t236
        t257 = t252 / 0.2E1 + t255 / 0.2E1
        t248 = t193 * t249
        t259 = t248 * t257
        t261 = (t245 - t259) * t94
        t262 = t261 / 0.2E1
        t266 = t35 * t51 + t48 * t42 + t46 * t38
        t267 = u(i,j,t233,n)
        t269 = (t267 - t1) * t236
        t270 = u(i,j,t238,n)
        t272 = (t1 - t270) * t236
        t274 = t269 / 0.2E1 + t272 / 0.2E1
        t265 = t211 * t266
        t276 = t265 * t274
        t278 = (t259 - t276) * t94
        t279 = t278 / 0.2E1
        t280 = rx(t5,t180,k,0,0)
        t281 = rx(t5,t180,k,1,1)
        t283 = rx(t5,t180,k,2,2)
        t285 = rx(t5,t180,k,1,2)
        t287 = rx(t5,t180,k,2,1)
        t289 = rx(t5,t180,k,1,0)
        t291 = rx(t5,t180,k,0,2)
        t293 = rx(t5,t180,k,0,1)
        t296 = rx(t5,t180,k,2,0)
        t301 = t280 * t281 * t283 - t280 * t285 * t287 + t289 * t287 * t
     #291 - t289 * t293 * t283 + t296 * t293 * t285 - t296 * t281 * t291
        t302 = 0.1E1 / t301
        t303 = t4 * t302
        t309 = (t181 - t198) * t94
        t311 = (t198 - t216) * t94
        t313 = t309 / 0.2E1 + t311 / 0.2E1
        t306 = t303 * (t280 * t289 + t293 * t281 + t291 * t285)
        t315 = t306 * t313
        t317 = t168 / 0.2E1 + t171 / 0.2E1
        t319 = t196 * t317
        t321 = (t315 - t319) * t183
        t322 = t321 / 0.2E1
        t323 = rx(t5,t185,k,0,0)
        t324 = rx(t5,t185,k,1,1)
        t326 = rx(t5,t185,k,2,2)
        t328 = rx(t5,t185,k,1,2)
        t330 = rx(t5,t185,k,2,1)
        t332 = rx(t5,t185,k,1,0)
        t334 = rx(t5,t185,k,0,2)
        t336 = rx(t5,t185,k,0,1)
        t339 = rx(t5,t185,k,2,0)
        t344 = t323 * t324 * t326 - t323 * t328 * t330 + t332 * t330 * t
     #334 - t332 * t336 * t326 + t339 * t336 * t328 - t339 * t324 * t334
        t345 = 0.1E1 / t344
        t346 = t4 * t345
        t352 = (t186 - t201) * t94
        t354 = (t201 - t219) * t94
        t356 = t352 / 0.2E1 + t354 / 0.2E1
        t348 = t346 * (t323 * t332 + t336 * t324 + t334 * t328)
        t358 = t348 * t356
        t360 = (t319 - t358) * t183
        t361 = t360 / 0.2E1
        t362 = t289 ** 2
        t363 = t281 ** 2
        t364 = t285 ** 2
        t366 = t302 * (t362 + t363 + t364)
        t367 = t15 ** 2
        t368 = t7 ** 2
        t369 = t11 ** 2
        t371 = t28 * (t367 + t368 + t369)
        t374 = t4 * (t366 / 0.2E1 + t371 / 0.2E1)
        t375 = t374 * t200
        t376 = t332 ** 2
        t377 = t324 ** 2
        t378 = t328 ** 2
        t380 = t345 * (t376 + t377 + t378)
        t383 = t4 * (t371 / 0.2E1 + t380 / 0.2E1)
        t384 = t383 * t203
        t386 = (t375 - t384) * t183
        t391 = u(t5,t180,t233,n)
        t393 = (t391 - t198) * t236
        t394 = u(t5,t180,t238,n)
        t396 = (t198 - t394) * t236
        t398 = t393 / 0.2E1 + t396 / 0.2E1
        t388 = t303 * (t289 * t296 + t281 * t287 + t285 * t283)
        t400 = t388 * t398
        t397 = t193 * (t15 * t22 + t7 * t13 + t11 * t9)
        t406 = t397 * t257
        t408 = (t400 - t406) * t183
        t409 = t408 / 0.2E1
        t414 = u(t5,t185,t233,n)
        t416 = (t414 - t201) * t236
        t417 = u(t5,t185,t238,n)
        t419 = (t201 - t417) * t236
        t421 = t416 / 0.2E1 + t419 / 0.2E1
        t411 = t346 * (t332 * t339 + t324 * t330 + t328 * t326)
        t423 = t411 * t421
        t425 = (t406 - t423) * t183
        t426 = t425 / 0.2E1
        t427 = rx(t5,j,t233,0,0)
        t428 = rx(t5,j,t233,1,1)
        t430 = rx(t5,j,t233,2,2)
        t432 = rx(t5,j,t233,1,2)
        t434 = rx(t5,j,t233,2,1)
        t436 = rx(t5,j,t233,1,0)
        t438 = rx(t5,j,t233,0,2)
        t440 = rx(t5,j,t233,0,1)
        t443 = rx(t5,j,t233,2,0)
        t448 = t427 * t428 * t430 - t427 * t432 * t434 + t436 * t434 * t
     #438 - t436 * t440 * t430 + t443 * t440 * t432 - t443 * t428 * t438
        t449 = 0.1E1 / t448
        t450 = t4 * t449
        t456 = (t234 - t250) * t94
        t458 = (t250 - t267) * t94
        t460 = t456 / 0.2E1 + t458 / 0.2E1
        t452 = t450 * (t427 * t443 + t440 * t434 + t438 * t430)
        t462 = t452 * t460
        t464 = t248 * t317
        t466 = (t462 - t464) * t236
        t467 = t466 / 0.2E1
        t468 = rx(t5,j,t238,0,0)
        t469 = rx(t5,j,t238,1,1)
        t471 = rx(t5,j,t238,2,2)
        t473 = rx(t5,j,t238,1,2)
        t475 = rx(t5,j,t238,2,1)
        t477 = rx(t5,j,t238,1,0)
        t479 = rx(t5,j,t238,0,2)
        t481 = rx(t5,j,t238,0,1)
        t484 = rx(t5,j,t238,2,0)
        t489 = t468 * t469 * t471 - t468 * t473 * t475 + t477 * t475 * t
     #479 - t477 * t481 * t471 + t484 * t481 * t473 - t484 * t469 * t479
        t490 = 0.1E1 / t489
        t491 = t4 * t490
        t497 = (t239 - t253) * t94
        t499 = (t253 - t270) * t94
        t501 = t497 / 0.2E1 + t499 / 0.2E1
        t492 = t491 * (t468 * t484 + t481 * t475 + t479 * t471)
        t503 = t492 * t501
        t505 = (t464 - t503) * t236
        t506 = t505 / 0.2E1
        t512 = (t391 - t250) * t183
        t514 = (t250 - t414) * t183
        t516 = t512 / 0.2E1 + t514 / 0.2E1
        t507 = t450 * (t436 * t443 + t428 * t434 + t432 * t430)
        t518 = t507 * t516
        t520 = t397 * t205
        t522 = (t518 - t520) * t236
        t523 = t522 / 0.2E1
        t529 = (t394 - t253) * t183
        t531 = (t253 - t417) * t183
        t533 = t529 / 0.2E1 + t531 / 0.2E1
        t521 = t491 * (t477 * t484 + t469 * t475 + t473 * t471)
        t535 = t521 * t533
        t537 = (t520 - t535) * t236
        t538 = t537 / 0.2E1
        t539 = t443 ** 2
        t540 = t434 ** 2
        t541 = t430 ** 2
        t543 = t449 * (t539 + t540 + t541)
        t544 = t22 ** 2
        t545 = t13 ** 2
        t546 = t9 ** 2
        t548 = t28 * (t544 + t545 + t546)
        t551 = t4 * (t543 / 0.2E1 + t548 / 0.2E1)
        t552 = t551 * t252
        t553 = t484 ** 2
        t554 = t475 ** 2
        t555 = t471 ** 2
        t557 = t490 * (t553 + t554 + t555)
        t560 = t4 * (t548 / 0.2E1 + t557 / 0.2E1)
        t561 = t560 * t255
        t563 = (t552 - t561) * t236
        t564 = t174 + t210 + t228 + t262 + t279 + t322 + t361 + t386 + t
     #409 + t426 + t467 + t506 + t523 + t538 + t563
        t565 = t564 * t27
        t568 = t4 * (t62 / 0.2E1 + t124 / 0.2E1)
        t569 = u(t96,j,k,n)
        t571 = (t1 - t569) * t94
        t572 = t568 * t571
        t574 = (t172 - t572) * t94
        t575 = t4 * t119
        t579 = t97 * t106 + t110 * t98 + t108 * t102
        t580 = u(t96,t180,k,n)
        t582 = (t580 - t569) * t183
        t583 = u(t96,t185,k,n)
        t585 = (t569 - t583) * t183
        t587 = t582 / 0.2E1 + t585 / 0.2E1
        t573 = t575 * t579
        t589 = t573 * t587
        t591 = (t225 - t589) * t94
        t592 = t591 / 0.2E1
        t596 = t97 * t113 + t110 * t104 + t108 * t100
        t597 = u(t96,j,t233,n)
        t599 = (t597 - t569) * t236
        t600 = u(t96,j,t238,n)
        t602 = (t569 - t600) * t236
        t604 = t599 / 0.2E1 + t602 / 0.2E1
        t590 = t575 * t596
        t606 = t590 * t604
        t608 = (t276 - t606) * t94
        t609 = t608 / 0.2E1
        t610 = rx(i,t180,k,0,0)
        t611 = rx(i,t180,k,1,1)
        t613 = rx(i,t180,k,2,2)
        t615 = rx(i,t180,k,1,2)
        t617 = rx(i,t180,k,2,1)
        t619 = rx(i,t180,k,1,0)
        t621 = rx(i,t180,k,0,2)
        t623 = rx(i,t180,k,0,1)
        t626 = rx(i,t180,k,2,0)
        t631 = t610 * t611 * t613 - t610 * t615 * t617 + t619 * t617 * t
     #621 - t619 * t623 * t613 + t626 * t623 * t615 - t626 * t611 * t621
        t632 = 0.1E1 / t631
        t633 = t4 * t632
        t637 = t610 * t619 + t623 * t611 + t621 * t615
        t639 = (t216 - t580) * t94
        t641 = t311 / 0.2E1 + t639 / 0.2E1
        t629 = t633 * t637
        t643 = t629 * t641
        t645 = t171 / 0.2E1 + t571 / 0.2E1
        t647 = t214 * t645
        t649 = (t643 - t647) * t183
        t650 = t649 / 0.2E1
        t651 = rx(i,t185,k,0,0)
        t652 = rx(i,t185,k,1,1)
        t654 = rx(i,t185,k,2,2)
        t656 = rx(i,t185,k,1,2)
        t658 = rx(i,t185,k,2,1)
        t660 = rx(i,t185,k,1,0)
        t662 = rx(i,t185,k,0,2)
        t664 = rx(i,t185,k,0,1)
        t667 = rx(i,t185,k,2,0)
        t672 = t651 * t652 * t654 - t651 * t656 * t658 + t660 * t658 * t
     #662 - t660 * t664 * t654 + t667 * t664 * t656 - t667 * t652 * t662
        t673 = 0.1E1 / t672
        t674 = t4 * t673
        t678 = t651 * t660 + t664 * t652 + t662 * t656
        t680 = (t219 - t583) * t94
        t682 = t354 / 0.2E1 + t680 / 0.2E1
        t669 = t674 * t678
        t684 = t669 * t682
        t686 = (t647 - t684) * t183
        t687 = t686 / 0.2E1
        t688 = t619 ** 2
        t689 = t611 ** 2
        t690 = t615 ** 2
        t692 = t632 * (t688 + t689 + t690)
        t693 = t44 ** 2
        t694 = t36 ** 2
        t695 = t40 ** 2
        t697 = t57 * (t693 + t694 + t695)
        t700 = t4 * (t692 / 0.2E1 + t697 / 0.2E1)
        t701 = t700 * t218
        t702 = t660 ** 2
        t703 = t652 ** 2
        t704 = t656 ** 2
        t706 = t673 * (t702 + t703 + t704)
        t709 = t4 * (t697 / 0.2E1 + t706 / 0.2E1)
        t710 = t709 * t221
        t712 = (t701 - t710) * t183
        t716 = t619 * t626 + t611 * t617 + t615 * t613
        t717 = u(i,t180,t233,n)
        t719 = (t717 - t216) * t236
        t720 = u(i,t180,t238,n)
        t722 = (t216 - t720) * t236
        t724 = t719 / 0.2E1 + t722 / 0.2E1
        t708 = t633 * t716
        t726 = t708 * t724
        t730 = t44 * t51 + t36 * t42 + t40 * t38
        t715 = t211 * t730
        t732 = t715 * t274
        t734 = (t726 - t732) * t183
        t735 = t734 / 0.2E1
        t739 = t660 * t667 + t652 * t658 + t656 * t654
        t740 = u(i,t185,t233,n)
        t742 = (t740 - t219) * t236
        t743 = u(i,t185,t238,n)
        t745 = (t219 - t743) * t236
        t747 = t742 / 0.2E1 + t745 / 0.2E1
        t731 = t674 * t739
        t749 = t731 * t747
        t751 = (t732 - t749) * t183
        t752 = t751 / 0.2E1
        t753 = rx(i,j,t233,0,0)
        t754 = rx(i,j,t233,1,1)
        t756 = rx(i,j,t233,2,2)
        t758 = rx(i,j,t233,1,2)
        t760 = rx(i,j,t233,2,1)
        t762 = rx(i,j,t233,1,0)
        t764 = rx(i,j,t233,0,2)
        t766 = rx(i,j,t233,0,1)
        t769 = rx(i,j,t233,2,0)
        t774 = t753 * t754 * t756 - t753 * t758 * t760 + t762 * t760 * t
     #764 - t762 * t766 * t756 + t769 * t766 * t758 - t769 * t754 * t764
        t775 = 0.1E1 / t774
        t776 = t4 * t775
        t780 = t753 * t769 + t766 * t760 + t764 * t756
        t782 = (t267 - t597) * t94
        t784 = t458 / 0.2E1 + t782 / 0.2E1
        t771 = t776 * t780
        t786 = t771 * t784
        t788 = t265 * t645
        t790 = (t786 - t788) * t236
        t791 = t790 / 0.2E1
        t792 = rx(i,j,t238,0,0)
        t793 = rx(i,j,t238,1,1)
        t795 = rx(i,j,t238,2,2)
        t797 = rx(i,j,t238,1,2)
        t799 = rx(i,j,t238,2,1)
        t801 = rx(i,j,t238,1,0)
        t803 = rx(i,j,t238,0,2)
        t805 = rx(i,j,t238,0,1)
        t808 = rx(i,j,t238,2,0)
        t813 = t792 * t793 * t795 - t792 * t797 * t799 + t801 * t799 * t
     #803 - t801 * t805 * t795 + t808 * t805 * t797 - t808 * t793 * t803
        t814 = 0.1E1 / t813
        t815 = t4 * t814
        t819 = t792 * t808 + t805 * t799 + t803 * t795
        t821 = (t270 - t600) * t94
        t823 = t499 / 0.2E1 + t821 / 0.2E1
        t809 = t815 * t819
        t825 = t809 * t823
        t827 = (t788 - t825) * t236
        t828 = t827 / 0.2E1
        t832 = t762 * t769 + t754 * t760 + t758 * t756
        t834 = (t717 - t267) * t183
        t836 = (t267 - t740) * t183
        t838 = t834 / 0.2E1 + t836 / 0.2E1
        t822 = t776 * t832
        t840 = t822 * t838
        t842 = t715 * t223
        t844 = (t840 - t842) * t236
        t845 = t844 / 0.2E1
        t849 = t801 * t808 + t793 * t799 + t797 * t795
        t851 = (t720 - t270) * t183
        t853 = (t270 - t743) * t183
        t855 = t851 / 0.2E1 + t853 / 0.2E1
        t837 = t815 * t849
        t857 = t837 * t855
        t859 = (t842 - t857) * t236
        t860 = t859 / 0.2E1
        t861 = t769 ** 2
        t862 = t760 ** 2
        t863 = t756 ** 2
        t865 = t775 * (t861 + t862 + t863)
        t866 = t51 ** 2
        t867 = t42 ** 2
        t868 = t38 ** 2
        t870 = t57 * (t866 + t867 + t868)
        t873 = t4 * (t865 / 0.2E1 + t870 / 0.2E1)
        t874 = t873 * t269
        t875 = t808 ** 2
        t876 = t799 ** 2
        t877 = t795 ** 2
        t879 = t814 * (t875 + t876 + t877)
        t882 = t4 * (t870 / 0.2E1 + t879 / 0.2E1)
        t883 = t882 * t272
        t885 = (t874 - t883) * t236
        t886 = t574 + t228 + t592 + t279 + t609 + t650 + t687 + t712 + t
     #735 + t752 + t791 + t828 + t845 + t860 + t885
        t887 = t886 * t56
        t888 = t565 - t887
        t890 = t161 * t888 * t94
        t893 = t159 * t135
        t895 = t161 * dt
        t896 = t164 * t142
        t897 = t158 * t139
        t899 = (t896 - t897) * t94
        t900 = ut(t64,t180,k,n)
        t902 = (t900 - t140) * t183
        t903 = ut(t64,t185,k,n)
        t905 = (t140 - t903) * t183
        t907 = t902 / 0.2E1 + t905 / 0.2E1
        t909 = t178 * t907
        t910 = ut(t5,t180,k,n)
        t912 = (t910 - t137) * t183
        t913 = ut(t5,t185,k,n)
        t915 = (t137 - t913) * t183
        t917 = t912 / 0.2E1 + t915 / 0.2E1
        t919 = t196 * t917
        t921 = (t909 - t919) * t94
        t922 = t921 / 0.2E1
        t923 = ut(i,t180,k,n)
        t925 = (t923 - t2) * t183
        t926 = ut(i,t185,k,n)
        t928 = (t2 - t926) * t183
        t930 = t925 / 0.2E1 + t928 / 0.2E1
        t932 = t214 * t930
        t934 = (t919 - t932) * t94
        t935 = t934 / 0.2E1
        t936 = ut(t64,j,t233,n)
        t938 = (t936 - t140) * t236
        t939 = ut(t64,j,t238,n)
        t941 = (t140 - t939) * t236
        t943 = t938 / 0.2E1 + t941 / 0.2E1
        t945 = t231 * t943
        t946 = ut(t5,j,t233,n)
        t948 = (t946 - t137) * t236
        t949 = ut(t5,j,t238,n)
        t951 = (t137 - t949) * t236
        t953 = t948 / 0.2E1 + t951 / 0.2E1
        t955 = t248 * t953
        t957 = (t945 - t955) * t94
        t958 = t957 / 0.2E1
        t959 = ut(i,j,t233,n)
        t961 = (t959 - t2) * t236
        t962 = ut(i,j,t238,n)
        t964 = (t2 - t962) * t236
        t966 = t961 / 0.2E1 + t964 / 0.2E1
        t968 = t265 * t966
        t970 = (t955 - t968) * t94
        t971 = t970 / 0.2E1
        t973 = (t900 - t910) * t94
        t975 = (t910 - t923) * t94
        t977 = t973 / 0.2E1 + t975 / 0.2E1
        t979 = t306 * t977
        t981 = t142 / 0.2E1 + t139 / 0.2E1
        t983 = t196 * t981
        t985 = (t979 - t983) * t183
        t986 = t985 / 0.2E1
        t988 = (t903 - t913) * t94
        t990 = (t913 - t926) * t94
        t992 = t988 / 0.2E1 + t990 / 0.2E1
        t994 = t348 * t992
        t996 = (t983 - t994) * t183
        t997 = t996 / 0.2E1
        t998 = t374 * t912
        t999 = t383 * t915
        t1001 = (t998 - t999) * t183
        t1002 = ut(t5,t180,t233,n)
        t1004 = (t1002 - t910) * t236
        t1005 = ut(t5,t180,t238,n)
        t1007 = (t910 - t1005) * t236
        t1009 = t1004 / 0.2E1 + t1007 / 0.2E1
        t1011 = t388 * t1009
        t1013 = t397 * t953
        t1015 = (t1011 - t1013) * t183
        t1016 = t1015 / 0.2E1
        t1017 = ut(t5,t185,t233,n)
        t1019 = (t1017 - t913) * t236
        t1020 = ut(t5,t185,t238,n)
        t1022 = (t913 - t1020) * t236
        t1024 = t1019 / 0.2E1 + t1022 / 0.2E1
        t1026 = t411 * t1024
        t1028 = (t1013 - t1026) * t183
        t1029 = t1028 / 0.2E1
        t1031 = (t936 - t946) * t94
        t1033 = (t946 - t959) * t94
        t1035 = t1031 / 0.2E1 + t1033 / 0.2E1
        t1037 = t452 * t1035
        t1039 = t248 * t981
        t1041 = (t1037 - t1039) * t236
        t1042 = t1041 / 0.2E1
        t1044 = (t939 - t949) * t94
        t1046 = (t949 - t962) * t94
        t1048 = t1044 / 0.2E1 + t1046 / 0.2E1
        t1050 = t492 * t1048
        t1052 = (t1039 - t1050) * t236
        t1053 = t1052 / 0.2E1
        t1055 = (t1002 - t946) * t183
        t1057 = (t946 - t1017) * t183
        t1059 = t1055 / 0.2E1 + t1057 / 0.2E1
        t1061 = t507 * t1059
        t1063 = t397 * t917
        t1065 = (t1061 - t1063) * t236
        t1066 = t1065 / 0.2E1
        t1068 = (t1005 - t949) * t183
        t1070 = (t949 - t1020) * t183
        t1072 = t1068 / 0.2E1 + t1070 / 0.2E1
        t1074 = t521 * t1072
        t1076 = (t1063 - t1074) * t236
        t1077 = t1076 / 0.2E1
        t1078 = t551 * t948
        t1079 = t560 * t951
        t1081 = (t1078 - t1079) * t236
        t1082 = t899 + t922 + t935 + t958 + t971 + t986 + t997 + t1001 +
     # t1016 + t1029 + t1042 + t1053 + t1066 + t1077 + t1081
        t1083 = t1082 * t27
        t1084 = t568 * t147
        t1086 = (t897 - t1084) * t94
        t1087 = ut(t96,t180,k,n)
        t1089 = (t1087 - t145) * t183
        t1090 = ut(t96,t185,k,n)
        t1092 = (t145 - t1090) * t183
        t1094 = t1089 / 0.2E1 + t1092 / 0.2E1
        t1096 = t573 * t1094
        t1098 = (t932 - t1096) * t94
        t1099 = t1098 / 0.2E1
        t1100 = ut(t96,j,t233,n)
        t1102 = (t1100 - t145) * t236
        t1103 = ut(t96,j,t238,n)
        t1105 = (t145 - t1103) * t236
        t1107 = t1102 / 0.2E1 + t1105 / 0.2E1
        t1109 = t590 * t1107
        t1111 = (t968 - t1109) * t94
        t1112 = t1111 / 0.2E1
        t1114 = (t923 - t1087) * t94
        t1116 = t975 / 0.2E1 + t1114 / 0.2E1
        t1118 = t629 * t1116
        t1120 = t139 / 0.2E1 + t147 / 0.2E1
        t1122 = t214 * t1120
        t1124 = (t1118 - t1122) * t183
        t1125 = t1124 / 0.2E1
        t1127 = (t926 - t1090) * t94
        t1129 = t990 / 0.2E1 + t1127 / 0.2E1
        t1131 = t669 * t1129
        t1133 = (t1122 - t1131) * t183
        t1134 = t1133 / 0.2E1
        t1135 = t700 * t925
        t1136 = t709 * t928
        t1138 = (t1135 - t1136) * t183
        t1139 = ut(i,t180,t233,n)
        t1141 = (t1139 - t923) * t236
        t1142 = ut(i,t180,t238,n)
        t1144 = (t923 - t1142) * t236
        t1146 = t1141 / 0.2E1 + t1144 / 0.2E1
        t1148 = t708 * t1146
        t1150 = t715 * t966
        t1152 = (t1148 - t1150) * t183
        t1153 = t1152 / 0.2E1
        t1154 = ut(i,t185,t233,n)
        t1156 = (t1154 - t926) * t236
        t1157 = ut(i,t185,t238,n)
        t1159 = (t926 - t1157) * t236
        t1161 = t1156 / 0.2E1 + t1159 / 0.2E1
        t1163 = t731 * t1161
        t1165 = (t1150 - t1163) * t183
        t1166 = t1165 / 0.2E1
        t1168 = (t959 - t1100) * t94
        t1170 = t1033 / 0.2E1 + t1168 / 0.2E1
        t1172 = t771 * t1170
        t1174 = t265 * t1120
        t1176 = (t1172 - t1174) * t236
        t1177 = t1176 / 0.2E1
        t1179 = (t962 - t1103) * t94
        t1181 = t1046 / 0.2E1 + t1179 / 0.2E1
        t1183 = t809 * t1181
        t1185 = (t1174 - t1183) * t236
        t1186 = t1185 / 0.2E1
        t1188 = (t1139 - t959) * t183
        t1190 = (t959 - t1154) * t183
        t1192 = t1188 / 0.2E1 + t1190 / 0.2E1
        t1194 = t822 * t1192
        t1196 = t715 * t930
        t1198 = (t1194 - t1196) * t236
        t1199 = t1198 / 0.2E1
        t1201 = (t1142 - t962) * t183
        t1203 = (t962 - t1157) * t183
        t1205 = t1201 / 0.2E1 + t1203 / 0.2E1
        t1207 = t837 * t1205
        t1209 = (t1196 - t1207) * t236
        t1210 = t1209 / 0.2E1
        t1211 = t873 * t961
        t1212 = t882 * t964
        t1214 = (t1211 - t1212) * t236
        t1215 = t1086 + t935 + t1099 + t971 + t1112 + t1125 + t1134 + t1
     #138 + t1153 + t1166 + t1177 + t1186 + t1199 + t1210 + t1214
        t1216 = t1215 * t56
        t1217 = t1083 - t1216
        t1219 = t895 * t1217 * t94
        t1222 = t899 - t1086
        t1223 = dx * t1222
        t1226 = cc * t131
        t1227 = dx ** 2
        t1228 = i + 3
        t1229 = u(t1228,t180,k,n)
        t1231 = (t1229 - t181) * t94
        t1237 = (t309 / 0.2E1 - t639 / 0.2E1) * t94
        t1242 = u(t1228,j,k,n)
        t1244 = (t1242 - t165) * t94
        t1250 = (t168 / 0.2E1 - t571 / 0.2E1) * t94
        t1130 = ((t1244 / 0.2E1 - t171 / 0.2E1) * t94 - t1250) * t94
        t1254 = t196 * t1130
        t1257 = u(t1228,t185,k,n)
        t1259 = (t1257 - t186) * t94
        t1265 = (t352 / 0.2E1 - t680 / 0.2E1) * t94
        t1276 = rx(t1228,j,k,0,0)
        t1277 = rx(t1228,j,k,1,1)
        t1279 = rx(t1228,j,k,2,2)
        t1281 = rx(t1228,j,k,1,2)
        t1283 = rx(t1228,j,k,2,1)
        t1285 = rx(t1228,j,k,1,0)
        t1287 = rx(t1228,j,k,0,2)
        t1289 = rx(t1228,j,k,0,1)
        t1292 = rx(t1228,j,k,2,0)
        t1298 = 0.1E1 / (t1276 * t1277 * t1279 - t1276 * t1281 * t1283 +
     # t1285 * t1283 * t1287 - t1285 * t1289 * t1279 + t1292 * t1289 * t
     #1281 - t1292 * t1277 * t1287)
        t1299 = t4 * t1298
        t1304 = u(t1228,j,t233,n)
        t1306 = (t1304 - t1242) * t236
        t1307 = u(t1228,j,t238,n)
        t1309 = (t1242 - t1307) * t236
        t1184 = t1299 * (t1276 * t1292 + t1289 * t1283 + t1287 * t1279)
        t1315 = (t1184 * (t1306 / 0.2E1 + t1309 / 0.2E1) - t245) * t94
        t1319 = (t261 - t278) * t94
        t1323 = (t278 - t608) * t94
        t1325 = (t1319 - t1323) * t94
        t1330 = dy ** 2
        t1331 = j + 2
        t1332 = rx(t5,t1331,k,0,0)
        t1333 = rx(t5,t1331,k,1,1)
        t1335 = rx(t5,t1331,k,2,2)
        t1337 = rx(t5,t1331,k,1,2)
        t1339 = rx(t5,t1331,k,2,1)
        t1341 = rx(t5,t1331,k,1,0)
        t1343 = rx(t5,t1331,k,0,2)
        t1345 = rx(t5,t1331,k,0,1)
        t1348 = rx(t5,t1331,k,2,0)
        t1353 = t1332 * t1333 * t1335 - t1332 * t1337 * t1339 + t1341 * 
     #t1339 * t1343 - t1341 * t1345 * t1335 + t1348 * t1345 * t1337 - t1
     #348 * t1333 * t1343
        t1354 = 0.1E1 / t1353
        t1355 = t4 * t1354
        t1360 = u(t5,t1331,t233,n)
        t1361 = u(t5,t1331,k,n)
        t1363 = (t1360 - t1361) * t236
        t1364 = u(t5,t1331,t238,n)
        t1366 = (t1361 - t1364) * t236
        t1368 = t1363 / 0.2E1 + t1366 / 0.2E1
        t1240 = t1355 * (t1341 * t1348 + t1333 * t1339 + t1337 * t1335)
        t1370 = t1240 * t1368
        t1372 = (t1370 - t400) * t183
        t1376 = (t408 - t425) * t183
        t1379 = j - 2
        t1380 = rx(t5,t1379,k,0,0)
        t1381 = rx(t5,t1379,k,1,1)
        t1383 = rx(t5,t1379,k,2,2)
        t1385 = rx(t5,t1379,k,1,2)
        t1387 = rx(t5,t1379,k,2,1)
        t1389 = rx(t5,t1379,k,1,0)
        t1391 = rx(t5,t1379,k,0,2)
        t1393 = rx(t5,t1379,k,0,1)
        t1396 = rx(t5,t1379,k,2,0)
        t1401 = t1380 * t1381 * t1383 - t1380 * t1385 * t1387 + t1389 * 
     #t1387 * t1391 - t1389 * t1393 * t1383 + t1396 * t1393 * t1385 - t1
     #396 * t1381 * t1391
        t1402 = 0.1E1 / t1401
        t1403 = t4 * t1402
        t1408 = u(t5,t1379,t233,n)
        t1409 = u(t5,t1379,k,n)
        t1411 = (t1408 - t1409) * t236
        t1412 = u(t5,t1379,t238,n)
        t1414 = (t1409 - t1412) * t236
        t1416 = t1411 / 0.2E1 + t1414 / 0.2E1
        t1269 = t1403 * (t1389 * t1396 + t1381 * t1387 + t1385 * t1383)
        t1418 = t1269 * t1416
        t1420 = (t423 - t1418) * t183
        t1429 = dz ** 2
        t1430 = k + 2
        t1431 = u(t5,j,t1430,n)
        t1433 = (t1431 - t250) * t236
        t1437 = (t252 - t255) * t236
        t1439 = ((t1433 - t252) * t236 - t1437) * t236
        t1441 = k - 2
        t1442 = u(t5,j,t1441,n)
        t1444 = (t253 - t1442) * t236
        t1448 = (t1437 - (t255 - t1444) * t236) * t236
        t1452 = rx(t5,j,t1430,0,0)
        t1453 = rx(t5,j,t1430,1,1)
        t1455 = rx(t5,j,t1430,2,2)
        t1457 = rx(t5,j,t1430,1,2)
        t1459 = rx(t5,j,t1430,2,1)
        t1461 = rx(t5,j,t1430,1,0)
        t1463 = rx(t5,j,t1430,0,2)
        t1465 = rx(t5,j,t1430,0,1)
        t1468 = rx(t5,j,t1430,2,0)
        t1473 = t1452 * t1453 * t1455 - t1452 * t1457 * t1459 + t1461 * 
     #t1459 * t1463 - t1461 * t1465 * t1455 + t1468 * t1465 * t1457 - t1
     #468 * t1453 * t1463
        t1474 = 0.1E1 / t1473
        t1475 = t1468 ** 2
        t1476 = t1459 ** 2
        t1477 = t1455 ** 2
        t1479 = t1474 * (t1475 + t1476 + t1477)
        t1482 = t4 * (t1479 / 0.2E1 + t543 / 0.2E1)
        t1483 = t1482 * t1433
        t1485 = (t1483 - t552) * t236
        t1488 = rx(t5,j,t1441,0,0)
        t1489 = rx(t5,j,t1441,1,1)
        t1491 = rx(t5,j,t1441,2,2)
        t1493 = rx(t5,j,t1441,1,2)
        t1495 = rx(t5,j,t1441,2,1)
        t1497 = rx(t5,j,t1441,1,0)
        t1499 = rx(t5,j,t1441,0,2)
        t1501 = rx(t5,j,t1441,0,1)
        t1504 = rx(t5,j,t1441,2,0)
        t1509 = t1488 * t1489 * t1491 - t1488 * t1493 * t1495 + t1497 * 
     #t1495 * t1499 - t1497 * t1501 * t1491 + t1504 * t1501 * t1493 - t1
     #504 * t1489 * t1499
        t1510 = 0.1E1 / t1509
        t1511 = t1504 ** 2
        t1512 = t1495 ** 2
        t1513 = t1491 ** 2
        t1515 = t1510 * (t1511 + t1512 + t1513)
        t1518 = t4 * (t557 / 0.2E1 + t1515 / 0.2E1)
        t1519 = t1518 * t1444
        t1521 = (t561 - t1519) * t236
        t1529 = t4 * t1474
        t1534 = u(t5,t180,t1430,n)
        t1536 = (t1534 - t1431) * t183
        t1537 = u(t5,t185,t1430,n)
        t1539 = (t1431 - t1537) * t183
        t1541 = t1536 / 0.2E1 + t1539 / 0.2E1
        t1349 = t1529 * (t1461 * t1468 + t1453 * t1459 + t1457 * t1455)
        t1543 = t1349 * t1541
        t1545 = (t1543 - t518) * t236
        t1549 = (t522 - t537) * t236
        t1552 = t4 * t1510
        t1557 = u(t5,t180,t1441,n)
        t1559 = (t1557 - t1442) * t183
        t1560 = u(t5,t185,t1441,n)
        t1562 = (t1442 - t1560) * t183
        t1564 = t1559 / 0.2E1 + t1562 / 0.2E1
        t1367 = t1552 * (t1497 * t1504 + t1489 * t1495 + t1493 * t1491)
        t1566 = t1367 * t1564
        t1568 = (t535 - t1566) * t236
        t1578 = t548 / 0.2E1
        t1588 = t4 * (t543 / 0.2E1 + t1578 - dz * ((t1479 - t543) * t236
     # / 0.2E1 - (t548 - t557) * t236 / 0.2E1) / 0.8E1)
        t1600 = t4 * (t1578 + t557 / 0.2E1 - dz * ((t543 - t548) * t236 
     #/ 0.2E1 - (t557 - t1515) * t236 / 0.2E1) / 0.8E1)
        t1607 = (t168 - t171) * t94
        t1612 = (t171 - t571) * t94
        t1613 = t1607 - t1612
        t1614 = t1613 * t94
        t1615 = t158 * t1614
        t1618 = t1276 ** 2
        t1619 = t1289 ** 2
        t1620 = t1287 ** 2
        t1622 = t1298 * (t1618 + t1619 + t1620)
        t1625 = t4 * (t1622 / 0.2E1 + t92 / 0.2E1)
        t1628 = (t1625 * t1244 - t169) * t94
        t1631 = t174 - t574
        t1632 = t1631 * t94
        t1638 = t279 + t506 + t523 + t322 - t1227 * ((t306 * ((t1231 / 0
     #.2E1 - t311 / 0.2E1) * t94 - t1237) * t94 - t1254) * t183 / 0.2E1 
     #+ (t1254 - t348 * ((t1259 / 0.2E1 - t354 / 0.2E1) * t94 - t1265) *
     # t94) * t183 / 0.2E1) / 0.6E1 - t1227 * (((t1315 - t261) * t94 - t
     #1319) * t94 / 0.2E1 + t1325 / 0.2E1) / 0.6E1 - t1330 * (((t1372 - 
     #t408) * t183 - t1376) * t183 / 0.2E1 + (t1376 - (t425 - t1420) * t
     #183) * t183 / 0.2E1) / 0.6E1 - t1429 * ((t551 * t1439 - t560 * t14
     #48) * t236 + ((t1485 - t563) * t236 - (t563 - t1521) * t236) * t23
     #6) / 0.24E2 + t228 + t538 + t361 + t426 - t1429 * (((t1545 - t522)
     # * t236 - t1549) * t236 / 0.2E1 + (t1549 - (t537 - t1568) * t236) 
     #* t236 / 0.2E1) / 0.6E1 + (t1588 * t252 - t1600 * t255) * t236 - t
     #1227 * ((t164 * ((t1244 - t168) * t94 - t1607) * t94 - t1615) * t9
     #4 + ((t1628 - t174) * t94 - t1632) * t94) / 0.24E2
        t1643 = (t33 - t62) * t94
        t1649 = t4 * (t92 / 0.2E1 + t34 - dx * ((t1622 - t92) * t94 / 0.
     #2E1 - t1643 / 0.2E1) / 0.8E1)
        t1651 = t132 * t171
        t1659 = (t1229 - t1242) * t183
        t1661 = (t1242 - t1257) * t183
        t1585 = t1299 * (t1276 * t1285 + t1289 * t1277 + t1287 * t1281)
        t1667 = (t1585 * (t1659 / 0.2E1 + t1661 / 0.2E1) - t192) * t94
        t1671 = (t209 - t227) * t94
        t1675 = (t227 - t591) * t94
        t1677 = (t1671 - t1675) * t94
        t1682 = u(t64,j,t1430,n)
        t1684 = (t1682 - t234) * t236
        t1688 = u(t64,j,t1441,n)
        t1690 = (t239 - t1688) * t236
        t1700 = (t1433 / 0.2E1 - t255 / 0.2E1) * t236
        t1703 = (t252 / 0.2E1 - t1444 / 0.2E1) * t236
        t1601 = (t1700 - t1703) * t236
        t1707 = t248 * t1601
        t1710 = u(i,j,t1430,n)
        t1712 = (t1710 - t267) * t236
        t1715 = (t1712 / 0.2E1 - t272 / 0.2E1) * t236
        t1716 = u(i,j,t1441,n)
        t1718 = (t270 - t1716) * t236
        t1721 = (t269 / 0.2E1 - t1718 / 0.2E1) * t236
        t1609 = (t1715 - t1721) * t236
        t1725 = t265 * t1609
        t1727 = (t1707 - t1725) * t94
        t1737 = (t1682 - t1431) * t94
        t1739 = (t1431 - t1710) * t94
        t1741 = t1737 / 0.2E1 + t1739 / 0.2E1
        t1626 = t1529 * (t1452 * t1468 + t1465 * t1459 + t1463 * t1455)
        t1743 = t1626 * t1741
        t1745 = (t1743 - t462) * t236
        t1749 = (t466 - t505) * t236
        t1757 = (t1688 - t1442) * t94
        t1759 = (t1442 - t1716) * t94
        t1761 = t1757 / 0.2E1 + t1759 / 0.2E1
        t1639 = t1552 * (t1488 * t1504 + t1501 * t1495 + t1499 * t1491)
        t1763 = t1639 * t1761
        t1765 = (t503 - t1763) * t236
        t1778 = u(t64,t1331,k,n)
        t1780 = (t1778 - t1361) * t94
        t1781 = u(i,t1331,k,n)
        t1783 = (t1361 - t1781) * t94
        t1785 = t1780 / 0.2E1 + t1783 / 0.2E1
        t1648 = t1355 * (t1332 * t1341 + t1345 * t1333 + t1343 * t1337)
        t1787 = t1648 * t1785
        t1789 = (t1787 - t315) * t183
        t1793 = (t321 - t360) * t183
        t1800 = u(t64,t1379,k,n)
        t1802 = (t1800 - t1409) * t94
        t1803 = u(i,t1379,k,n)
        t1805 = (t1409 - t1803) * t94
        t1807 = t1802 / 0.2E1 + t1805 / 0.2E1
        t1662 = t1403 * (t1380 * t1389 + t1393 * t1381 + t1391 * t1385)
        t1809 = t1662 * t1807
        t1811 = (t358 - t1809) * t183
        t1821 = t371 / 0.2E1
        t1822 = t1341 ** 2
        t1823 = t1333 ** 2
        t1824 = t1337 ** 2
        t1826 = t1354 * (t1822 + t1823 + t1824)
        t1836 = t4 * (t366 / 0.2E1 + t1821 - dy * ((t1826 - t366) * t183
     # / 0.2E1 - (t371 - t380) * t183 / 0.2E1) / 0.8E1)
        t1841 = t1389 ** 2
        t1842 = t1381 ** 2
        t1843 = t1385 ** 2
        t1845 = t1402 * (t1841 + t1842 + t1843)
        t1853 = t4 * (t1821 + t380 / 0.2E1 - dy * ((t366 - t371) * t183 
     #/ 0.2E1 - (t380 - t1845) * t183 / 0.2E1) / 0.8E1)
        t1858 = (t1361 - t198) * t183
        t1862 = (t200 - t203) * t183
        t1864 = ((t1858 - t200) * t183 - t1862) * t183
        t1867 = (t201 - t1409) * t183
        t1871 = (t1862 - (t203 - t1867) * t183) * t183
        t1877 = t4 * (t1826 / 0.2E1 + t366 / 0.2E1)
        t1878 = t1877 * t1858
        t1880 = (t1878 - t375) * t183
        t1885 = t4 * (t380 / 0.2E1 + t1845 / 0.2E1)
        t1886 = t1885 * t1867
        t1888 = (t384 - t1886) * t183
        t1897 = (t1304 - t234) * t94
        t1903 = (t456 / 0.2E1 - t782 / 0.2E1) * t94
        t1910 = t248 * t1130
        t1914 = (t1307 - t239) * t94
        t1920 = (t497 / 0.2E1 - t821 / 0.2E1) * t94
        t1932 = (t1360 - t391) * t183
        t1937 = (t414 - t1408) * t183
        t1947 = (t1858 / 0.2E1 - t203 / 0.2E1) * t183
        t1950 = (t200 / 0.2E1 - t1867 / 0.2E1) * t183
        t1746 = (t1947 - t1950) * t183
        t1954 = t397 * t1746
        t1958 = (t1364 - t394) * t183
        t1963 = (t417 - t1412) * t183
        t1978 = (t1534 - t391) * t236
        t1983 = (t394 - t1557) * t236
        t1993 = t397 * t1601
        t1997 = (t1537 - t414) * t236
        t2002 = (t417 - t1560) * t236
        t2017 = (t1778 - t181) * t183
        t2022 = (t186 - t1800) * t183
        t2032 = t196 * t1746
        t2036 = (t1781 - t216) * t183
        t2039 = (t2036 / 0.2E1 - t221 / 0.2E1) * t183
        t2041 = (t219 - t1803) * t183
        t2044 = (t218 / 0.2E1 - t2041 / 0.2E1) * t183
        t1795 = (t2039 - t2044) * t183
        t2048 = t214 * t1795
        t2050 = (t2032 - t2048) * t94
        t1918 = ((t1932 / 0.2E1 - t514 / 0.2E1) * t183 - (t512 / 0.2E1 -
     # t1937 / 0.2E1) * t183) * t183
        t1923 = ((t1958 / 0.2E1 - t531 / 0.2E1) * t183 - (t529 / 0.2E1 -
     # t1963 / 0.2E1) * t183) * t183
        t1931 = ((t1978 / 0.2E1 - t396 / 0.2E1) * t236 - (t393 / 0.2E1 -
     # t1983 / 0.2E1) * t236) * t236
        t1936 = ((t1997 / 0.2E1 - t419 / 0.2E1) * t236 - (t416 / 0.2E1 -
     # t2002 / 0.2E1) * t236) * t236
        t2055 = (t1649 * t168 - t1651) * t94 - t1227 * (((t1667 - t209) 
     #* t94 - t1671) * t94 / 0.2E1 + t1677 / 0.2E1) / 0.6E1 + t409 + t46
     #7 + t210 - t1429 * ((t231 * ((t1684 / 0.2E1 - t241 / 0.2E1) * t236
     # - (t237 / 0.2E1 - t1690 / 0.2E1) * t236) * t236 - t1707) * t94 / 
     #0.2E1 + t1727 / 0.2E1) / 0.6E1 - t1429 * (((t1745 - t466) * t236 -
     # t1749) * t236 / 0.2E1 + (t1749 - (t505 - t1765) * t236) * t236 / 
     #0.2E1) / 0.6E1 - t1330 * (((t1789 - t321) * t183 - t1793) * t183 /
     # 0.2E1 + (t1793 - (t360 - t1811) * t183) * t183 / 0.2E1) / 0.6E1 +
     # (t1836 * t200 - t1853 * t203) * t183 + t262 - t1330 * ((t374 * t1
     #864 - t383 * t1871) * t183 + ((t1880 - t386) * t183 - (t386 - t188
     #8) * t183) * t183) / 0.24E2 - t1227 * ((t452 * ((t1897 / 0.2E1 - t
     #458 / 0.2E1) * t94 - t1903) * t94 - t1910) * t236 / 0.2E1 + (t1910
     # - t492 * ((t1914 / 0.2E1 - t499 / 0.2E1) * t94 - t1920) * t94) * 
     #t236 / 0.2E1) / 0.6E1 - t1330 * ((t507 * t1918 - t1954) * t236 / 0
     #.2E1 + (t1954 - t521 * t1923) * t236 / 0.2E1) / 0.6E1 - t1429 * ((
     #t388 * t1931 - t1993) * t183 / 0.2E1 + (t1993 - t411 * t1936) * t1
     #83 / 0.2E1) / 0.6E1 - t1330 * ((t178 * ((t2017 / 0.2E1 - t188 / 0.
     #2E1) * t183 - (t184 / 0.2E1 - t2022 / 0.2E1) * t183) * t183 - t203
     #2) * t94 / 0.2E1 + t2050 / 0.2E1) / 0.6E1
        t2057 = (t1638 + t2055) * t27
        t2060 = t139 / 0.2E1
        t2061 = ut(t1228,j,k,n)
        t2063 = (t2061 - t140) * t94
        t2067 = ((t2063 - t142) * t94 - t144) * t94
        t2068 = t150 * t94
        t2075 = dx * (t142 / 0.2E1 + t2060 - t1227 * (t2067 / 0.2E1 + t2
     #068 / 0.2E1) / 0.6E1) / 0.2E1
        t2076 = t159 * t161
        t2077 = ut(t64,t1331,k,n)
        t2079 = (t2077 - t900) * t183
        t2083 = ut(t64,t1379,k,n)
        t2085 = (t903 - t2083) * t183
        t2093 = ut(t5,t1331,k,n)
        t2095 = (t2093 - t910) * t183
        t2098 = (t2095 / 0.2E1 - t915 / 0.2E1) * t183
        t2099 = ut(t5,t1379,k,n)
        t2101 = (t913 - t2099) * t183
        t2104 = (t912 / 0.2E1 - t2101 / 0.2E1) * t183
        t1991 = (t2098 - t2104) * t183
        t2108 = t196 * t1991
        t2111 = ut(i,t1331,k,n)
        t2113 = (t2111 - t923) * t183
        t2116 = (t2113 / 0.2E1 - t928 / 0.2E1) * t183
        t2117 = ut(i,t1379,k,n)
        t2119 = (t926 - t2117) * t183
        t2122 = (t925 / 0.2E1 - t2119 / 0.2E1) * t183
        t2000 = (t2116 - t2122) * t183
        t2126 = t214 * t2000
        t2128 = (t2108 - t2126) * t94
        t2133 = ut(t1228,t180,k,n)
        t2136 = ut(t1228,t185,k,n)
        t2144 = (t1585 * ((t2133 - t2061) * t183 / 0.2E1 + (t2061 - t213
     #6) * t183 / 0.2E1) - t909) * t94
        t2148 = (t921 - t934) * t94
        t2152 = (t934 - t1098) * t94
        t2154 = (t2148 - t2152) * t94
        t2159 = ut(t1228,j,t233,n)
        t2161 = (t2159 - t936) * t94
        t2167 = (t1031 / 0.2E1 - t1168 / 0.2E1) * t94
        t2177 = (t142 / 0.2E1 - t147 / 0.2E1) * t94
        t2025 = ((t2063 / 0.2E1 - t139 / 0.2E1) * t94 - t2177) * t94
        t2181 = t248 * t2025
        t2184 = ut(t1228,j,t238,n)
        t2186 = (t2184 - t939) * t94
        t2192 = (t1044 / 0.2E1 - t1179 / 0.2E1) * t94
        t2204 = t132 * t139
        t2207 = ut(t64,j,t1430,n)
        t2209 = (t2207 - t936) * t236
        t2213 = ut(t64,j,t1441,n)
        t2215 = (t939 - t2213) * t236
        t2223 = ut(t5,j,t1430,n)
        t2225 = (t2223 - t946) * t236
        t2228 = (t2225 / 0.2E1 - t951 / 0.2E1) * t236
        t2229 = ut(t5,j,t1441,n)
        t2231 = (t949 - t2229) * t236
        t2234 = (t948 / 0.2E1 - t2231 / 0.2E1) * t236
        t2040 = (t2228 - t2234) * t236
        t2238 = t248 * t2040
        t2241 = ut(i,j,t1430,n)
        t2243 = (t2241 - t959) * t236
        t2246 = (t2243 / 0.2E1 - t964 / 0.2E1) * t236
        t2247 = ut(i,j,t1441,n)
        t2249 = (t962 - t2247) * t236
        t2252 = (t961 / 0.2E1 - t2249 / 0.2E1) * t236
        t2052 = (t2246 - t2252) * t236
        t2256 = t265 * t2052
        t2258 = (t2238 - t2256) * t94
        t2263 = ut(t5,t180,t1430,n)
        t2266 = ut(t5,t185,t1430,n)
        t2270 = (t2263 - t2223) * t183 / 0.2E1 + (t2223 - t2266) * t183 
     #/ 0.2E1
        t2274 = (t1349 * t2270 - t1061) * t236
        t2278 = (t1065 - t1076) * t236
        t2281 = ut(t5,t180,t1441,n)
        t2284 = ut(t5,t185,t1441,n)
        t2288 = (t2281 - t2229) * t183 / 0.2E1 + (t2229 - t2284) * t183 
     #/ 0.2E1
        t2292 = (t1074 - t1367 * t2288) * t236
        t2302 = t158 * t2068
        t2307 = (t1625 * t2063 - t896) * t94
        t2310 = t1222 * t94
        t2325 = (t1184 * ((t2159 - t2061) * t236 / 0.2E1 + (t2061 - t218
     #4) * t236 / 0.2E1) - t945) * t94
        t2329 = (t957 - t970) * t94
        t2333 = (t970 - t1111) * t94
        t2335 = (t2329 - t2333) * t94
        t2340 = t997 + t1016 + t922 + t935 + t1029 + t1042 + t1053 - t13
     #30 * ((t178 * ((t2079 / 0.2E1 - t905 / 0.2E1) * t183 - (t902 / 0.2
     #E1 - t2085 / 0.2E1) * t183) * t183 - t2108) * t94 / 0.2E1 + t2128 
     #/ 0.2E1) / 0.6E1 - t1227 * (((t2144 - t921) * t94 - t2148) * t94 /
     # 0.2E1 + t2154 / 0.2E1) / 0.6E1 - t1227 * ((t452 * ((t2161 / 0.2E1
     # - t1033 / 0.2E1) * t94 - t2167) * t94 - t2181) * t236 / 0.2E1 + (
     #t2181 - t492 * ((t2186 / 0.2E1 - t1046 / 0.2E1) * t94 - t2192) * t
     #94) * t236 / 0.2E1) / 0.6E1 + (t1649 * t142 - t2204) * t94 - t1429
     # * ((t231 * ((t2209 / 0.2E1 - t941 / 0.2E1) * t236 - (t938 / 0.2E1
     # - t2215 / 0.2E1) * t236) * t236 - t2238) * t94 / 0.2E1 + t2258 / 
     #0.2E1) / 0.6E1 - t1429 * (((t2274 - t1065) * t236 - t2278) * t236 
     #/ 0.2E1 + (t2278 - (t1076 - t2292) * t236) * t236 / 0.2E1) / 0.6E1
     # - t1227 * ((t164 * t2067 - t2302) * t94 + ((t2307 - t899) * t94 -
     # t2310) * t94) / 0.24E2 - t1227 * (((t2325 - t957) * t94 - t2329) 
     #* t94 / 0.2E1 + t2335 / 0.2E1) / 0.6E1
        t2345 = ut(t5,t1331,t233,n)
        t2348 = ut(t5,t1331,t238,n)
        t2352 = (t2345 - t2093) * t236 / 0.2E1 + (t2093 - t2348) * t236 
     #/ 0.2E1
        t2356 = (t1240 * t2352 - t1011) * t183
        t2360 = (t1015 - t1028) * t183
        t2363 = ut(t5,t1379,t233,n)
        t2366 = ut(t5,t1379,t238,n)
        t2370 = (t2363 - t2099) * t236 / 0.2E1 + (t2099 - t2366) * t236 
     #/ 0.2E1
        t2374 = (t1026 - t1269 * t2370) * t183
        t2384 = (t2263 - t1002) * t236
        t2389 = (t1005 - t2281) * t236
        t2399 = t397 * t2040
        t2403 = (t2266 - t1017) * t236
        t2408 = (t1020 - t2284) * t236
        t2425 = (t912 - t915) * t183
        t2427 = ((t2095 - t912) * t183 - t2425) * t183
        t2432 = (t2425 - (t915 - t2101) * t183) * t183
        t2438 = (t1877 * t2095 - t998) * t183
        t2443 = (t999 - t1885 * t2101) * t183
        t2452 = (t2207 - t2223) * t94
        t2454 = (t2223 - t2241) * t94
        t2460 = (t1626 * (t2452 / 0.2E1 + t2454 / 0.2E1) - t1037) * t236
        t2464 = (t1041 - t1052) * t236
        t2468 = (t2213 - t2229) * t94
        t2470 = (t2229 - t2247) * t94
        t2476 = (t1050 - t1639 * (t2468 / 0.2E1 + t2470 / 0.2E1)) * t236
        t2488 = (t948 - t951) * t236
        t2490 = ((t2225 - t948) * t236 - t2488) * t236
        t2495 = (t2488 - (t951 - t2231) * t236) * t236
        t2501 = (t1482 * t2225 - t1078) * t236
        t2506 = (t1079 - t1518 * t2231) * t236
        t2515 = (t2133 - t900) * t94
        t2521 = (t973 / 0.2E1 - t1114 / 0.2E1) * t94
        t2528 = t196 * t2025
        t2532 = (t2136 - t903) * t94
        t2538 = (t988 / 0.2E1 - t1127 / 0.2E1) * t94
        t2550 = (t2345 - t1002) * t183
        t2555 = (t1017 - t2363) * t183
        t2565 = t397 * t1991
        t2569 = (t2348 - t1005) * t183
        t2574 = (t1020 - t2366) * t183
        t2589 = (t2077 - t2093) * t94
        t2591 = (t2093 - t2111) * t94
        t2597 = (t1648 * (t2589 / 0.2E1 + t2591 / 0.2E1) - t979) * t183
        t2601 = (t985 - t996) * t183
        t2605 = (t2083 - t2099) * t94
        t2607 = (t2099 - t2117) * t94
        t2613 = (t994 - t1662 * (t2605 / 0.2E1 + t2607 / 0.2E1)) * t183
        t2383 = ((t2384 / 0.2E1 - t1007 / 0.2E1) * t236 - (t1004 / 0.2E1
     # - t2389 / 0.2E1) * t236) * t236
        t2388 = ((t2403 / 0.2E1 - t1022 / 0.2E1) * t236 - (t1019 / 0.2E1
     # - t2408 / 0.2E1) * t236) * t236
        t2471 = ((t2550 / 0.2E1 - t1057 / 0.2E1) * t183 - (t1055 / 0.2E1
     # - t2555 / 0.2E1) * t183) * t183
        t2475 = ((t2569 / 0.2E1 - t1070 / 0.2E1) * t183 - (t1068 / 0.2E1
     # - t2574 / 0.2E1) * t183) * t183
        t2626 = (t1588 * t948 - t1600 * t951) * t236 - t1330 * (((t2356 
     #- t1015) * t183 - t2360) * t183 / 0.2E1 + (t2360 - (t1028 - t2374)
     # * t183) * t183 / 0.2E1) / 0.6E1 - t1429 * ((t388 * t2383 - t2399)
     # * t183 / 0.2E1 + (t2399 - t411 * t2388) * t183 / 0.2E1) / 0.6E1 -
     # t1330 * ((t374 * t2427 - t383 * t2432) * t183 + ((t2438 - t1001) 
     #* t183 - (t1001 - t2443) * t183) * t183) / 0.24E2 + t1066 + t1077 
     #- t1429 * (((t2460 - t1041) * t236 - t2464) * t236 / 0.2E1 + (t246
     #4 - (t1052 - t2476) * t236) * t236 / 0.2E1) / 0.6E1 - t1429 * ((t5
     #51 * t2490 - t560 * t2495) * t236 + ((t2501 - t1081) * t236 - (t10
     #81 - t2506) * t236) * t236) / 0.24E2 - t1227 * ((t306 * ((t2515 / 
     #0.2E1 - t975 / 0.2E1) * t94 - t2521) * t94 - t2528) * t183 / 0.2E1
     # + (t2528 - t348 * ((t2532 / 0.2E1 - t990 / 0.2E1) * t94 - t2538) 
     #* t94) * t183 / 0.2E1) / 0.6E1 - t1330 * ((t507 * t2471 - t2565) *
     # t236 / 0.2E1 + (t2565 - t521 * t2475) * t236 / 0.2E1) / 0.6E1 - t
     #1330 * (((t2597 - t985) * t183 - t2601) * t183 / 0.2E1 + (t2601 - 
     #(t996 - t2613) * t183) * t183 / 0.2E1) / 0.6E1 + (t1836 * t912 - t
     #1853 * t915) * t183 + t958 + t971 + t986
        t2628 = (t2340 + t2626) * t27
        t2633 = rx(t64,t180,k,0,0)
        t2634 = rx(t64,t180,k,1,1)
        t2636 = rx(t64,t180,k,2,2)
        t2638 = rx(t64,t180,k,1,2)
        t2640 = rx(t64,t180,k,2,1)
        t2642 = rx(t64,t180,k,1,0)
        t2644 = rx(t64,t180,k,0,2)
        t2646 = rx(t64,t180,k,0,1)
        t2649 = rx(t64,t180,k,2,0)
        t2654 = t2633 * t2634 * t2636 - t2633 * t2638 * t2640 + t2642 * 
     #t2640 * t2644 - t2642 * t2646 * t2636 + t2649 * t2646 * t2638 - t2
     #649 * t2634 * t2644
        t2655 = 0.1E1 / t2654
        t2656 = t4 * t2655
        t2662 = t1231 / 0.2E1 + t309 / 0.2E1
        t2524 = t2656 * (t2633 * t2642 + t2646 * t2634 + t2644 * t2638)
        t2664 = t2524 * t2662
        t2666 = t1244 / 0.2E1 + t168 / 0.2E1
        t2668 = t178 * t2666
        t2671 = (t2664 - t2668) * t183 / 0.2E1
        t2672 = rx(t64,t185,k,0,0)
        t2673 = rx(t64,t185,k,1,1)
        t2675 = rx(t64,t185,k,2,2)
        t2677 = rx(t64,t185,k,1,2)
        t2679 = rx(t64,t185,k,2,1)
        t2681 = rx(t64,t185,k,1,0)
        t2683 = rx(t64,t185,k,0,2)
        t2685 = rx(t64,t185,k,0,1)
        t2688 = rx(t64,t185,k,2,0)
        t2693 = t2672 * t2673 * t2675 - t2672 * t2677 * t2679 + t2681 * 
     #t2679 * t2683 - t2681 * t2685 * t2675 + t2688 * t2685 * t2677 - t2
     #688 * t2673 * t2683
        t2694 = 0.1E1 / t2693
        t2695 = t4 * t2694
        t2701 = t1259 / 0.2E1 + t352 / 0.2E1
        t2547 = t2695 * (t2672 * t2681 + t2685 * t2673 + t2683 * t2677)
        t2703 = t2547 * t2701
        t2706 = (t2668 - t2703) * t183 / 0.2E1
        t2707 = t2642 ** 2
        t2708 = t2634 ** 2
        t2709 = t2638 ** 2
        t2711 = t2655 * (t2707 + t2708 + t2709)
        t2712 = t74 ** 2
        t2713 = t66 ** 2
        t2714 = t70 ** 2
        t2716 = t87 * (t2712 + t2713 + t2714)
        t2719 = t4 * (t2711 / 0.2E1 + t2716 / 0.2E1)
        t2720 = t2719 * t184
        t2721 = t2681 ** 2
        t2722 = t2673 ** 2
        t2723 = t2677 ** 2
        t2725 = t2694 * (t2721 + t2722 + t2723)
        t2728 = t4 * (t2716 / 0.2E1 + t2725 / 0.2E1)
        t2729 = t2728 * t188
        t2736 = u(t64,t180,t233,n)
        t2738 = (t2736 - t181) * t236
        t2739 = u(t64,t180,t238,n)
        t2741 = (t181 - t2739) * t236
        t2743 = t2738 / 0.2E1 + t2741 / 0.2E1
        t2567 = t2656 * (t2642 * t2649 + t2634 * t2640 + t2638 * t2636)
        t2745 = t2567 * t2743
        t2572 = t175 * (t74 * t81 + t66 * t72 + t70 * t68)
        t2751 = t2572 * t243
        t2754 = (t2745 - t2751) * t183 / 0.2E1
        t2759 = u(t64,t185,t233,n)
        t2761 = (t2759 - t186) * t236
        t2762 = u(t64,t185,t238,n)
        t2764 = (t186 - t2762) * t236
        t2766 = t2761 / 0.2E1 + t2764 / 0.2E1
        t2583 = t2695 * (t2681 * t2688 + t2673 * t2679 + t2677 * t2675)
        t2768 = t2583 * t2766
        t2771 = (t2751 - t2768) * t183 / 0.2E1
        t2772 = rx(t64,j,t233,0,0)
        t2773 = rx(t64,j,t233,1,1)
        t2775 = rx(t64,j,t233,2,2)
        t2777 = rx(t64,j,t233,1,2)
        t2779 = rx(t64,j,t233,2,1)
        t2781 = rx(t64,j,t233,1,0)
        t2783 = rx(t64,j,t233,0,2)
        t2785 = rx(t64,j,t233,0,1)
        t2788 = rx(t64,j,t233,2,0)
        t2793 = t2772 * t2773 * t2775 - t2772 * t2777 * t2779 + t2781 * 
     #t2779 * t2783 - t2781 * t2785 * t2775 + t2788 * t2785 * t2777 - t2
     #788 * t2773 * t2783
        t2794 = 0.1E1 / t2793
        t2795 = t4 * t2794
        t2801 = t1897 / 0.2E1 + t456 / 0.2E1
        t2608 = t2795 * (t2772 * t2788 + t2785 * t2779 + t2783 * t2775)
        t2803 = t2608 * t2801
        t2805 = t231 * t2666
        t2808 = (t2803 - t2805) * t236 / 0.2E1
        t2809 = rx(t64,j,t238,0,0)
        t2810 = rx(t64,j,t238,1,1)
        t2812 = rx(t64,j,t238,2,2)
        t2814 = rx(t64,j,t238,1,2)
        t2816 = rx(t64,j,t238,2,1)
        t2818 = rx(t64,j,t238,1,0)
        t2820 = rx(t64,j,t238,0,2)
        t2822 = rx(t64,j,t238,0,1)
        t2825 = rx(t64,j,t238,2,0)
        t2830 = t2809 * t2810 * t2812 - t2816 * t2814 * t2809 + t2818 * 
     #t2816 * t2820 - t2818 * t2822 * t2812 + t2825 * t2822 * t2814 - t2
     #825 * t2810 * t2820
        t2831 = 0.1E1 / t2830
        t2832 = t4 * t2831
        t2838 = t1914 / 0.2E1 + t497 / 0.2E1
        t2630 = t2832 * (t2809 * t2825 + t2822 * t2816 + t2820 * t2812)
        t2840 = t2630 * t2838
        t2843 = (t2805 - t2840) * t236 / 0.2E1
        t2849 = (t2736 - t234) * t183
        t2851 = (t234 - t2759) * t183
        t2853 = t2849 / 0.2E1 + t2851 / 0.2E1
        t2647 = t2795 * (t2781 * t2788 + t2773 * t2779 + t2777 * t2775)
        t2855 = t2647 * t2853
        t2857 = t2572 * t190
        t2860 = (t2855 - t2857) * t236 / 0.2E1
        t2866 = (t2739 - t239) * t183
        t2868 = (t239 - t2762) * t183
        t2870 = t2866 / 0.2E1 + t2868 / 0.2E1
        t2661 = t2832 * (t2818 * t2825 + t2810 * t2816 + t2814 * t2812)
        t2872 = t2661 * t2870
        t2875 = (t2857 - t2872) * t236 / 0.2E1
        t2876 = t2788 ** 2
        t2877 = t2779 ** 2
        t2878 = t2775 ** 2
        t2880 = t2794 * (t2876 + t2877 + t2878)
        t2881 = t81 ** 2
        t2882 = t72 ** 2
        t2883 = t68 ** 2
        t2885 = t87 * (t2881 + t2882 + t2883)
        t2888 = t4 * (t2880 / 0.2E1 + t2885 / 0.2E1)
        t2889 = t2888 * t237
        t2890 = t2825 ** 2
        t2891 = t2816 ** 2
        t2892 = t2812 ** 2
        t2894 = t2831 * (t2890 + t2891 + t2892)
        t2897 = t4 * (t2885 / 0.2E1 + t2894 / 0.2E1)
        t2898 = t2897 * t241
        t2901 = t1628 + t1667 / 0.2E1 + t210 + t1315 / 0.2E1 + t262 + t2
     #671 + t2706 + (t2720 - t2729) * t183 + t2754 + t2771 + t2808 + t28
     #43 + t2860 + t2875 + (t2889 - t2898) * t236
        t2902 = t2901 * t86
        t2904 = (t2902 - t565) * t94
        t2905 = t888 * t94
        t2907 = t2904 / 0.2E1 + t2905 / 0.2E1
        t2908 = dx * t2907
        t2916 = t1227 * (t144 - dx * (t2067 - t2068) / 0.12E2) / 0.12E2
        t2917 = t893 * t895
        t2919 = t158 * t2905
        t2922 = rx(t1228,t180,k,0,0)
        t2923 = rx(t1228,t180,k,1,1)
        t2925 = rx(t1228,t180,k,2,2)
        t2927 = rx(t1228,t180,k,1,2)
        t2929 = rx(t1228,t180,k,2,1)
        t2931 = rx(t1228,t180,k,1,0)
        t2933 = rx(t1228,t180,k,0,2)
        t2935 = rx(t1228,t180,k,0,1)
        t2938 = rx(t1228,t180,k,2,0)
        t2944 = 0.1E1 / (t2922 * t2923 * t2925 - t2922 * t2927 * t2929 +
     # t2931 * t2929 * t2933 - t2931 * t2935 * t2925 + t2938 * t2935 * t
     #2927 - t2938 * t2923 * t2933)
        t2945 = t2922 ** 2
        t2946 = t2935 ** 2
        t2947 = t2933 ** 2
        t2950 = t2633 ** 2
        t2951 = t2646 ** 2
        t2952 = t2644 ** 2
        t2954 = t2655 * (t2950 + t2951 + t2952)
        t2959 = t280 ** 2
        t2960 = t293 ** 2
        t2961 = t291 ** 2
        t2963 = t302 * (t2959 + t2960 + t2961)
        t2966 = t4 * (t2954 / 0.2E1 + t2963 / 0.2E1)
        t2967 = t2966 * t309
        t2970 = t4 * t2944
        t2975 = u(t1228,t1331,k,n)
        t2983 = t2017 / 0.2E1 + t184 / 0.2E1
        t2985 = t2524 * t2983
        t2990 = t1858 / 0.2E1 + t200 / 0.2E1
        t2992 = t306 * t2990
        t2994 = (t2985 - t2992) * t94
        t2995 = t2994 / 0.2E1
        t3000 = u(t1228,t180,t233,n)
        t3003 = u(t1228,t180,t238,n)
        t2753 = t2656 * (t2633 * t2649 + t2646 * t2640 + t2644 * t2636)
        t3015 = t2753 * t2743
        t2760 = t303 * (t280 * t296 + t293 * t287 + t291 * t283)
        t3024 = t2760 * t398
        t3026 = (t3015 - t3024) * t94
        t3027 = t3026 / 0.2E1
        t3028 = rx(t64,t1331,k,0,0)
        t3029 = rx(t64,t1331,k,1,1)
        t3031 = rx(t64,t1331,k,2,2)
        t3033 = rx(t64,t1331,k,1,2)
        t3035 = rx(t64,t1331,k,2,1)
        t3037 = rx(t64,t1331,k,1,0)
        t3039 = rx(t64,t1331,k,0,2)
        t3041 = rx(t64,t1331,k,0,1)
        t3044 = rx(t64,t1331,k,2,0)
        t3050 = 0.1E1 / (t3028 * t3029 * t3031 - t3028 * t3033 * t3035 +
     # t3037 * t3035 * t3039 - t3037 * t3041 * t3031 + t3044 * t3041 * t
     #3033 - t3044 * t3029 * t3039)
        t3051 = t4 * t3050
        t3065 = t3037 ** 2
        t3066 = t3029 ** 2
        t3067 = t3033 ** 2
        t3080 = u(t64,t1331,t233,n)
        t3083 = u(t64,t1331,t238,n)
        t3087 = (t3080 - t1778) * t236 / 0.2E1 + (t1778 - t3083) * t236 
     #/ 0.2E1
        t3093 = rx(t64,t180,t233,0,0)
        t3094 = rx(t64,t180,t233,1,1)
        t3096 = rx(t64,t180,t233,2,2)
        t3098 = rx(t64,t180,t233,1,2)
        t3100 = rx(t64,t180,t233,2,1)
        t3102 = rx(t64,t180,t233,1,0)
        t3104 = rx(t64,t180,t233,0,2)
        t3106 = rx(t64,t180,t233,0,1)
        t3109 = rx(t64,t180,t233,2,0)
        t3115 = 0.1E1 / (t3093 * t3094 * t3096 - t3093 * t3098 * t3100 +
     # t3102 * t3100 * t3104 - t3102 * t3106 * t3096 + t3109 * t3106 * t
     #3098 - t3109 * t3094 * t3104)
        t3116 = t4 * t3115
        t3124 = (t2736 - t391) * t94
        t3126 = (t3000 - t2736) * t94 / 0.2E1 + t3124 / 0.2E1
        t3130 = t2753 * t2662
        t3134 = rx(t64,t180,t238,0,0)
        t3135 = rx(t64,t180,t238,1,1)
        t3137 = rx(t64,t180,t238,2,2)
        t3139 = rx(t64,t180,t238,1,2)
        t3141 = rx(t64,t180,t238,2,1)
        t3143 = rx(t64,t180,t238,1,0)
        t3145 = rx(t64,t180,t238,0,2)
        t3147 = rx(t64,t180,t238,0,1)
        t3150 = rx(t64,t180,t238,2,0)
        t3156 = 0.1E1 / (t3135 * t3134 * t3137 - t3134 * t3139 * t3141 +
     # t3143 * t3141 * t3145 - t3143 * t3147 * t3137 + t3150 * t3147 * t
     #3139 - t3150 * t3135 * t3145)
        t3157 = t4 * t3156
        t3165 = (t2739 - t394) * t94
        t3167 = (t3003 - t2739) * t94 / 0.2E1 + t3165 / 0.2E1
        t3180 = (t3080 - t2736) * t183 / 0.2E1 + t2849 / 0.2E1
        t3184 = t2567 * t2983
        t3195 = (t3083 - t2739) * t183 / 0.2E1 + t2866 / 0.2E1
        t3201 = t3109 ** 2
        t3202 = t3100 ** 2
        t3203 = t3096 ** 2
        t3206 = t2649 ** 2
        t3207 = t2640 ** 2
        t3208 = t2636 ** 2
        t3210 = t2655 * (t3206 + t3207 + t3208)
        t3215 = t3150 ** 2
        t3216 = t3141 ** 2
        t3217 = t3137 ** 2
        t2973 = t3051 * (t3028 * t3037 + t3041 * t3029 + t3039 * t3033)
        t3008 = t3116 * (t3093 * t3109 + t3106 * t3100 + t3104 * t3096)
        t3014 = t3157 * (t3134 * t3150 + t3147 * t3141 + t3145 * t3137)
        t3020 = t3116 * (t3102 * t3109 + t3094 * t3100 + t3098 * t3096)
        t3032 = t3157 * (t3143 * t3150 + t3135 * t3141 + t3139 * t3137)
        t3226 = (t4 * (t2944 * (t2945 + t2946 + t2947) / 0.2E1 + t2954 /
     # 0.2E1) * t1231 - t2967) * t94 + (t2970 * (t2922 * t2931 + t2935 *
     # t2923 + t2933 * t2927) * ((t2975 - t1229) * t183 / 0.2E1 + t1659 
     #/ 0.2E1) - t2985) * t94 / 0.2E1 + t2995 + (t2970 * (t2922 * t2938 
     #+ t2935 * t2929 + t2933 * t2925) * ((t3000 - t1229) * t236 / 0.2E1
     # + (t1229 - t3003) * t236 / 0.2E1) - t3015) * t94 / 0.2E1 + t3027 
     #+ (t2973 * ((t2975 - t1778) * t94 / 0.2E1 + t1780 / 0.2E1) - t2664
     #) * t183 / 0.2E1 + t2671 + (t4 * (t3050 * (t3065 + t3066 + t3067) 
     #/ 0.2E1 + t2711 / 0.2E1) * t2017 - t2720) * t183 + (t3051 * (t3037
     # * t3044 + t3029 * t3035 + t3033 * t3031) * t3087 - t2745) * t183 
     #/ 0.2E1 + t2754 + (t3008 * t3126 - t3130) * t236 / 0.2E1 + (t3130 
     #- t3014 * t3167) * t236 / 0.2E1 + (t3020 * t3180 - t3184) * t236 /
     # 0.2E1 + (t3184 - t3032 * t3195) * t236 / 0.2E1 + (t4 * (t3115 * (
     #t3201 + t3202 + t3203) / 0.2E1 + t3210 / 0.2E1) * t2738 - t4 * (t3
     #210 / 0.2E1 + t3156 * (t3215 + t3216 + t3217) / 0.2E1) * t2741) * 
     #t236
        t3227 = t3226 * t2654
        t3230 = rx(t1228,t185,k,0,0)
        t3231 = rx(t1228,t185,k,1,1)
        t3233 = rx(t1228,t185,k,2,2)
        t3235 = rx(t1228,t185,k,1,2)
        t3237 = rx(t1228,t185,k,2,1)
        t3239 = rx(t1228,t185,k,1,0)
        t3241 = rx(t1228,t185,k,0,2)
        t3243 = rx(t1228,t185,k,0,1)
        t3246 = rx(t1228,t185,k,2,0)
        t3252 = 0.1E1 / (t3230 * t3231 * t3233 - t3230 * t3235 * t3237 +
     # t3239 * t3237 * t3241 - t3239 * t3243 * t3233 + t3246 * t3243 * t
     #3235 - t3246 * t3231 * t3241)
        t3253 = t3230 ** 2
        t3254 = t3243 ** 2
        t3255 = t3241 ** 2
        t3258 = t2672 ** 2
        t3259 = t2685 ** 2
        t3260 = t2683 ** 2
        t3262 = t2694 * (t3258 + t3259 + t3260)
        t3267 = t323 ** 2
        t3268 = t336 ** 2
        t3269 = t334 ** 2
        t3271 = t345 * (t3267 + t3268 + t3269)
        t3274 = t4 * (t3262 / 0.2E1 + t3271 / 0.2E1)
        t3275 = t3274 * t352
        t3278 = t4 * t3252
        t3283 = u(t1228,t1379,k,n)
        t3291 = t188 / 0.2E1 + t2022 / 0.2E1
        t3293 = t2547 * t3291
        t3298 = t203 / 0.2E1 + t1867 / 0.2E1
        t3300 = t348 * t3298
        t3302 = (t3293 - t3300) * t94
        t3303 = t3302 / 0.2E1
        t3308 = u(t1228,t185,t233,n)
        t3311 = u(t1228,t185,t238,n)
        t3089 = t2695 * (t2672 * t2688 + t2685 * t2679 + t2683 * t2675)
        t3323 = t3089 * t2766
        t3095 = t346 * (t323 * t339 + t336 * t330 + t334 * t326)
        t3332 = t3095 * t421
        t3334 = (t3323 - t3332) * t94
        t3335 = t3334 / 0.2E1
        t3336 = rx(t64,t1379,k,0,0)
        t3337 = rx(t64,t1379,k,1,1)
        t3339 = rx(t64,t1379,k,2,2)
        t3341 = rx(t64,t1379,k,1,2)
        t3343 = rx(t64,t1379,k,2,1)
        t3345 = rx(t64,t1379,k,1,0)
        t3347 = rx(t64,t1379,k,0,2)
        t3349 = rx(t64,t1379,k,0,1)
        t3352 = rx(t64,t1379,k,2,0)
        t3358 = 0.1E1 / (t3336 * t3337 * t3339 - t3343 * t3341 * t3336 +
     # t3345 * t3343 * t3347 - t3345 * t3349 * t3339 + t3352 * t3349 * t
     #3341 - t3352 * t3337 * t3347)
        t3359 = t4 * t3358
        t3373 = t3345 ** 2
        t3374 = t3337 ** 2
        t3375 = t3341 ** 2
        t3388 = u(t64,t1379,t233,n)
        t3391 = u(t64,t1379,t238,n)
        t3395 = (t3388 - t1800) * t236 / 0.2E1 + (t1800 - t3391) * t236 
     #/ 0.2E1
        t3401 = rx(t64,t185,t233,0,0)
        t3402 = rx(t64,t185,t233,1,1)
        t3404 = rx(t64,t185,t233,2,2)
        t3406 = rx(t64,t185,t233,1,2)
        t3408 = rx(t64,t185,t233,2,1)
        t3410 = rx(t64,t185,t233,1,0)
        t3412 = rx(t64,t185,t233,0,2)
        t3414 = rx(t64,t185,t233,0,1)
        t3417 = rx(t64,t185,t233,2,0)
        t3423 = 0.1E1 / (t3401 * t3402 * t3404 - t3401 * t3406 * t3408 +
     # t3410 * t3408 * t3412 - t3410 * t3414 * t3404 + t3417 * t3414 * t
     #3406 - t3417 * t3402 * t3412)
        t3424 = t4 * t3423
        t3432 = (t2759 - t414) * t94
        t3434 = (t3308 - t2759) * t94 / 0.2E1 + t3432 / 0.2E1
        t3438 = t3089 * t2701
        t3442 = rx(t64,t185,t238,0,0)
        t3443 = rx(t64,t185,t238,1,1)
        t3445 = rx(t64,t185,t238,2,2)
        t3447 = rx(t64,t185,t238,1,2)
        t3449 = rx(t64,t185,t238,2,1)
        t3451 = rx(t64,t185,t238,1,0)
        t3453 = rx(t64,t185,t238,0,2)
        t3455 = rx(t64,t185,t238,0,1)
        t3458 = rx(t64,t185,t238,2,0)
        t3464 = 0.1E1 / (t3442 * t3443 * t3445 - t3442 * t3447 * t3449 +
     # t3451 * t3449 * t3453 - t3451 * t3455 * t3445 + t3458 * t3455 * t
     #3447 - t3458 * t3443 * t3453)
        t3465 = t4 * t3464
        t3473 = (t2762 - t417) * t94
        t3475 = (t3311 - t2762) * t94 / 0.2E1 + t3473 / 0.2E1
        t3488 = t2851 / 0.2E1 + (t2759 - t3388) * t183 / 0.2E1
        t3492 = t2583 * t3291
        t3503 = t2868 / 0.2E1 + (t2762 - t3391) * t183 / 0.2E1
        t3509 = t3417 ** 2
        t3510 = t3408 ** 2
        t3511 = t3404 ** 2
        t3514 = t2688 ** 2
        t3515 = t2679 ** 2
        t3516 = t2675 ** 2
        t3518 = t2694 * (t3514 + t3515 + t3516)
        t3523 = t3458 ** 2
        t3524 = t3449 ** 2
        t3525 = t3445 ** 2
        t3272 = t3359 * (t3336 * t3345 + t3349 * t3337 + t3347 * t3341)
        t3309 = t3424 * (t3401 * t3417 + t3414 * t3408 + t3412 * t3404)
        t3315 = t3465 * (t3442 * t3458 + t3455 * t3449 + t3453 * t3445)
        t3320 = t3424 * (t3410 * t3417 + t3402 * t3408 + t3406 * t3404)
        t3327 = t3465 * (t3458 * t3451 + t3443 * t3449 + t3447 * t3445)
        t3534 = (t4 * (t3252 * (t3253 + t3254 + t3255) / 0.2E1 + t3262 /
     # 0.2E1) * t1259 - t3275) * t94 + (t3278 * (t3230 * t3239 + t3243 *
     # t3231 + t3241 * t3235) * (t1661 / 0.2E1 + (t1257 - t3283) * t183 
     #/ 0.2E1) - t3293) * t94 / 0.2E1 + t3303 + (t3278 * (t3230 * t3246 
     #+ t3243 * t3237 + t3241 * t3233) * ((t3308 - t1257) * t236 / 0.2E1
     # + (t1257 - t3311) * t236 / 0.2E1) - t3323) * t94 / 0.2E1 + t3335 
     #+ t2706 + (t2703 - t3272 * ((t3283 - t1800) * t94 / 0.2E1 + t1802 
     #/ 0.2E1)) * t183 / 0.2E1 + (t2729 - t4 * (t2725 / 0.2E1 + t3358 * 
     #(t3373 + t3374 + t3375) / 0.2E1) * t2022) * t183 + t2771 + (t2768 
     #- t3359 * (t3345 * t3352 + t3337 * t3343 + t3341 * t3339) * t3395)
     # * t183 / 0.2E1 + (t3309 * t3434 - t3438) * t236 / 0.2E1 + (t3438 
     #- t3315 * t3475) * t236 / 0.2E1 + (t3320 * t3488 - t3492) * t236 /
     # 0.2E1 + (t3492 - t3327 * t3503) * t236 / 0.2E1 + (t4 * (t3423 * (
     #t3509 + t3510 + t3511) / 0.2E1 + t3518 / 0.2E1) * t2761 - t4 * (t3
     #518 / 0.2E1 + t3464 * (t3523 + t3524 + t3525) / 0.2E1) * t2764) * 
     #t236
        t3535 = t3534 * t2693
        t3542 = t610 ** 2
        t3543 = t623 ** 2
        t3544 = t621 ** 2
        t3546 = t632 * (t3542 + t3543 + t3544)
        t3549 = t4 * (t2963 / 0.2E1 + t3546 / 0.2E1)
        t3550 = t3549 * t311
        t3552 = (t2967 - t3550) * t94
        t3554 = t2036 / 0.2E1 + t218 / 0.2E1
        t3556 = t629 * t3554
        t3558 = (t2992 - t3556) * t94
        t3559 = t3558 / 0.2E1
        t3371 = t633 * (t610 * t626 + t623 * t617 + t621 * t613)
        t3565 = t3371 * t724
        t3567 = (t3024 - t3565) * t94
        t3568 = t3567 / 0.2E1
        t3569 = t1789 / 0.2E1
        t3570 = t1372 / 0.2E1
        t3571 = rx(t5,t180,t233,0,0)
        t3572 = rx(t5,t180,t233,1,1)
        t3574 = rx(t5,t180,t233,2,2)
        t3576 = rx(t5,t180,t233,1,2)
        t3578 = rx(t5,t180,t233,2,1)
        t3580 = rx(t5,t180,t233,1,0)
        t3582 = rx(t5,t180,t233,0,2)
        t3584 = rx(t5,t180,t233,0,1)
        t3587 = rx(t5,t180,t233,2,0)
        t3592 = t3571 * t3572 * t3574 - t3571 * t3576 * t3578 + t3580 * 
     #t3578 * t3582 - t3580 * t3584 * t3574 + t3587 * t3584 * t3576 - t3
     #587 * t3572 * t3582
        t3593 = 0.1E1 / t3592
        t3594 = t4 * t3593
        t3600 = (t391 - t717) * t94
        t3602 = t3124 / 0.2E1 + t3600 / 0.2E1
        t3396 = t3594 * (t3571 * t3587 + t3584 * t3578 + t3582 * t3574)
        t3604 = t3396 * t3602
        t3606 = t2760 * t313
        t3609 = (t3604 - t3606) * t236 / 0.2E1
        t3610 = rx(t5,t180,t238,0,0)
        t3611 = rx(t5,t180,t238,1,1)
        t3613 = rx(t5,t180,t238,2,2)
        t3615 = rx(t5,t180,t238,1,2)
        t3617 = rx(t5,t180,t238,2,1)
        t3619 = rx(t5,t180,t238,1,0)
        t3621 = rx(t5,t180,t238,0,2)
        t3623 = rx(t5,t180,t238,0,1)
        t3626 = rx(t5,t180,t238,2,0)
        t3631 = t3610 * t3611 * t3613 - t3610 * t3615 * t3617 + t3619 * 
     #t3617 * t3621 - t3619 * t3623 * t3613 + t3626 * t3623 * t3615 - t3
     #626 * t3611 * t3621
        t3632 = 0.1E1 / t3631
        t3633 = t4 * t3632
        t3639 = (t394 - t720) * t94
        t3641 = t3165 / 0.2E1 + t3639 / 0.2E1
        t3427 = t3633 * (t3610 * t3626 + t3623 * t3617 + t3621 * t3613)
        t3643 = t3427 * t3641
        t3646 = (t3606 - t3643) * t236 / 0.2E1
        t3652 = t1932 / 0.2E1 + t512 / 0.2E1
        t3437 = t3594 * (t3580 * t3587 + t3572 * t3578 + t3576 * t3574)
        t3654 = t3437 * t3652
        t3656 = t388 * t2990
        t3659 = (t3654 - t3656) * t236 / 0.2E1
        t3665 = t1958 / 0.2E1 + t529 / 0.2E1
        t3450 = t3633 * (t3619 * t3626 + t3611 * t3617 + t3615 * t3613)
        t3667 = t3450 * t3665
        t3670 = (t3656 - t3667) * t236 / 0.2E1
        t3671 = t3587 ** 2
        t3672 = t3578 ** 2
        t3673 = t3574 ** 2
        t3675 = t3593 * (t3671 + t3672 + t3673)
        t3676 = t296 ** 2
        t3677 = t287 ** 2
        t3678 = t283 ** 2
        t3680 = t302 * (t3676 + t3677 + t3678)
        t3683 = t4 * (t3675 / 0.2E1 + t3680 / 0.2E1)
        t3684 = t3683 * t393
        t3685 = t3626 ** 2
        t3686 = t3617 ** 2
        t3687 = t3613 ** 2
        t3689 = t3632 * (t3685 + t3686 + t3687)
        t3692 = t4 * (t3680 / 0.2E1 + t3689 / 0.2E1)
        t3693 = t3692 * t396
        t3696 = t3552 + t2995 + t3559 + t3027 + t3568 + t3569 + t322 + t
     #1880 + t3570 + t409 + t3609 + t3646 + t3659 + t3670 + (t3684 - t36
     #93) * t236
        t3697 = t3696 * t301
        t3699 = (t3697 - t565) * t183
        t3700 = t651 ** 2
        t3701 = t664 ** 2
        t3702 = t662 ** 2
        t3704 = t673 * (t3700 + t3701 + t3702)
        t3707 = t4 * (t3271 / 0.2E1 + t3704 / 0.2E1)
        t3708 = t3707 * t354
        t3710 = (t3275 - t3708) * t94
        t3712 = t221 / 0.2E1 + t2041 / 0.2E1
        t3714 = t669 * t3712
        t3716 = (t3300 - t3714) * t94
        t3717 = t3716 / 0.2E1
        t3481 = t674 * (t651 * t667 + t664 * t658 + t662 * t654)
        t3723 = t3481 * t747
        t3725 = (t3332 - t3723) * t94
        t3726 = t3725 / 0.2E1
        t3727 = t1811 / 0.2E1
        t3728 = t1420 / 0.2E1
        t3729 = rx(t5,t185,t233,0,0)
        t3730 = rx(t5,t185,t233,1,1)
        t3732 = rx(t5,t185,t233,2,2)
        t3734 = rx(t5,t185,t233,1,2)
        t3736 = rx(t5,t185,t233,2,1)
        t3738 = rx(t5,t185,t233,1,0)
        t3740 = rx(t5,t185,t233,0,2)
        t3742 = rx(t5,t185,t233,0,1)
        t3745 = rx(t5,t185,t233,2,0)
        t3750 = t3729 * t3730 * t3732 - t3729 * t3734 * t3736 + t3738 * 
     #t3736 * t3740 - t3738 * t3742 * t3732 + t3745 * t3742 * t3734 - t3
     #745 * t3730 * t3740
        t3751 = 0.1E1 / t3750
        t3752 = t4 * t3751
        t3758 = (t414 - t740) * t94
        t3760 = t3432 / 0.2E1 + t3758 / 0.2E1
        t3505 = t3752 * (t3729 * t3745 + t3742 * t3736 + t3740 * t3732)
        t3762 = t3505 * t3760
        t3764 = t3095 * t356
        t3767 = (t3762 - t3764) * t236 / 0.2E1
        t3768 = rx(t5,t185,t238,0,0)
        t3769 = rx(t5,t185,t238,1,1)
        t3771 = rx(t5,t185,t238,2,2)
        t3773 = rx(t5,t185,t238,1,2)
        t3775 = rx(t5,t185,t238,2,1)
        t3777 = rx(t5,t185,t238,1,0)
        t3779 = rx(t5,t185,t238,0,2)
        t3781 = rx(t5,t185,t238,0,1)
        t3784 = rx(t5,t185,t238,2,0)
        t3789 = t3768 * t3769 * t3771 - t3768 * t3773 * t3775 + t3777 * 
     #t3775 * t3779 - t3777 * t3781 * t3771 + t3784 * t3781 * t3773 - t3
     #784 * t3769 * t3779
        t3790 = 0.1E1 / t3789
        t3791 = t4 * t3790
        t3797 = (t417 - t743) * t94
        t3799 = t3473 / 0.2E1 + t3797 / 0.2E1
        t3537 = t3791 * (t3768 * t3784 + t3781 * t3775 + t3779 * t3771)
        t3801 = t3537 * t3799
        t3804 = (t3764 - t3801) * t236 / 0.2E1
        t3810 = t514 / 0.2E1 + t1937 / 0.2E1
        t3548 = t3752 * (t3745 * t3738 + t3730 * t3736 + t3734 * t3732)
        t3812 = t3548 * t3810
        t3814 = t411 * t3298
        t3817 = (t3812 - t3814) * t236 / 0.2E1
        t3823 = t531 / 0.2E1 + t1963 / 0.2E1
        t3562 = t3791 * (t3777 * t3784 + t3775 * t3769 + t3773 * t3771)
        t3825 = t3562 * t3823
        t3828 = (t3814 - t3825) * t236 / 0.2E1
        t3829 = t3745 ** 2
        t3830 = t3736 ** 2
        t3831 = t3732 ** 2
        t3833 = t3751 * (t3829 + t3830 + t3831)
        t3834 = t339 ** 2
        t3835 = t330 ** 2
        t3836 = t326 ** 2
        t3838 = t345 * (t3834 + t3835 + t3836)
        t3841 = t4 * (t3833 / 0.2E1 + t3838 / 0.2E1)
        t3842 = t3841 * t416
        t3843 = t3784 ** 2
        t3844 = t3775 ** 2
        t3845 = t3771 ** 2
        t3847 = t3790 * (t3843 + t3844 + t3845)
        t3850 = t4 * (t3838 / 0.2E1 + t3847 / 0.2E1)
        t3851 = t3850 * t419
        t3854 = t3710 + t3303 + t3717 + t3335 + t3726 + t361 + t3727 + t
     #1888 + t426 + t3728 + t3767 + t3804 + t3817 + t3828 + (t3842 - t38
     #51) * t236
        t3855 = t3854 * t344
        t3857 = (t565 - t3855) * t183
        t3859 = t3699 / 0.2E1 + t3857 / 0.2E1
        t3861 = t196 * t3859
        t3865 = rx(t96,t180,k,0,0)
        t3866 = rx(t96,t180,k,1,1)
        t3868 = rx(t96,t180,k,2,2)
        t3870 = rx(t96,t180,k,1,2)
        t3872 = rx(t96,t180,k,2,1)
        t3874 = rx(t96,t180,k,1,0)
        t3876 = rx(t96,t180,k,0,2)
        t3878 = rx(t96,t180,k,0,1)
        t3881 = rx(t96,t180,k,2,0)
        t3886 = t3865 * t3866 * t3868 - t3865 * t3870 * t3872 + t3874 * 
     #t3872 * t3876 - t3874 * t3878 * t3868 + t3881 * t3878 * t3870 - t3
     #881 * t3866 * t3876
        t3887 = 0.1E1 / t3886
        t3888 = t3865 ** 2
        t3889 = t3878 ** 2
        t3890 = t3876 ** 2
        t3892 = t3887 * (t3888 + t3889 + t3890)
        t3895 = t4 * (t3546 / 0.2E1 + t3892 / 0.2E1)
        t3896 = t3895 * t639
        t3898 = (t3550 - t3896) * t94
        t3899 = t4 * t3887
        t3904 = u(t96,t1331,k,n)
        t3906 = (t3904 - t580) * t183
        t3908 = t3906 / 0.2E1 + t582 / 0.2E1
        t3630 = t3899 * (t3865 * t3874 + t3878 * t3866 + t3876 * t3870)
        t3910 = t3630 * t3908
        t3912 = (t3556 - t3910) * t94
        t3913 = t3912 / 0.2E1
        t3918 = u(t96,t180,t233,n)
        t3920 = (t3918 - t580) * t236
        t3921 = u(t96,t180,t238,n)
        t3923 = (t580 - t3921) * t236
        t3925 = t3920 / 0.2E1 + t3923 / 0.2E1
        t3645 = t3899 * (t3865 * t3881 + t3878 * t3872 + t3876 * t3868)
        t3927 = t3645 * t3925
        t3929 = (t3565 - t3927) * t94
        t3930 = t3929 / 0.2E1
        t3931 = rx(i,t1331,k,0,0)
        t3932 = rx(i,t1331,k,1,1)
        t3934 = rx(i,t1331,k,2,2)
        t3936 = rx(i,t1331,k,1,2)
        t3938 = rx(i,t1331,k,2,1)
        t3940 = rx(i,t1331,k,1,0)
        t3942 = rx(i,t1331,k,0,2)
        t3944 = rx(i,t1331,k,0,1)
        t3947 = rx(i,t1331,k,2,0)
        t3952 = t3931 * t3932 * t3934 - t3931 * t3936 * t3938 + t3940 * 
     #t3938 * t3942 - t3940 * t3944 * t3934 + t3947 * t3944 * t3936 - t3
     #947 * t3932 * t3942
        t3953 = 0.1E1 / t3952
        t3954 = t4 * t3953
        t3958 = t3931 * t3940 + t3944 * t3932 + t3942 * t3936
        t3960 = (t1781 - t3904) * t94
        t3962 = t1783 / 0.2E1 + t3960 / 0.2E1
        t3682 = t3954 * t3958
        t3964 = t3682 * t3962
        t3966 = (t3964 - t643) * t183
        t3967 = t3966 / 0.2E1
        t3968 = t3940 ** 2
        t3969 = t3932 ** 2
        t3970 = t3936 ** 2
        t3972 = t3953 * (t3968 + t3969 + t3970)
        t3975 = t4 * (t3972 / 0.2E1 + t692 / 0.2E1)
        t3976 = t3975 * t2036
        t3978 = (t3976 - t701) * t183
        t3982 = t3940 * t3947 + t3932 * t3938 + t3936 * t3934
        t3983 = u(i,t1331,t233,n)
        t3985 = (t3983 - t1781) * t236
        t3986 = u(i,t1331,t238,n)
        t3988 = (t1781 - t3986) * t236
        t3990 = t3985 / 0.2E1 + t3988 / 0.2E1
        t3713 = t3954 * t3982
        t3992 = t3713 * t3990
        t3994 = (t3992 - t726) * t183
        t3995 = t3994 / 0.2E1
        t3996 = rx(i,t180,t233,0,0)
        t3997 = rx(i,t180,t233,1,1)
        t3999 = rx(i,t180,t233,2,2)
        t4001 = rx(i,t180,t233,1,2)
        t4003 = rx(i,t180,t233,2,1)
        t4005 = rx(i,t180,t233,1,0)
        t4007 = rx(i,t180,t233,0,2)
        t4009 = rx(i,t180,t233,0,1)
        t4012 = rx(i,t180,t233,2,0)
        t4017 = t3996 * t3997 * t3999 - t3996 * t4001 * t4003 + t4005 * 
     #t4003 * t4007 - t4005 * t4009 * t3999 + t4012 * t4009 * t4001 - t4
     #012 * t3997 * t4007
        t4018 = 0.1E1 / t4017
        t4019 = t4 * t4018
        t4025 = (t717 - t3918) * t94
        t4027 = t3600 / 0.2E1 + t4025 / 0.2E1
        t3753 = t4019 * (t3996 * t4012 + t4009 * t4003 + t4007 * t3999)
        t4029 = t3753 * t4027
        t4031 = t3371 * t641
        t4033 = (t4029 - t4031) * t236
        t4034 = t4033 / 0.2E1
        t4035 = rx(i,t180,t238,0,0)
        t4036 = rx(i,t180,t238,1,1)
        t4038 = rx(i,t180,t238,2,2)
        t4040 = rx(i,t180,t238,1,2)
        t4042 = rx(i,t180,t238,2,1)
        t4044 = rx(i,t180,t238,1,0)
        t4046 = rx(i,t180,t238,0,2)
        t4048 = rx(i,t180,t238,0,1)
        t4051 = rx(i,t180,t238,2,0)
        t4056 = t4035 * t4036 * t4038 - t4035 * t4040 * t4042 + t4044 * 
     #t4042 * t4046 - t4044 * t4048 * t4038 + t4051 * t4048 * t4040 - t4
     #051 * t4036 * t4046
        t4057 = 0.1E1 / t4056
        t4058 = t4 * t4057
        t4064 = (t720 - t3921) * t94
        t4066 = t3639 / 0.2E1 + t4064 / 0.2E1
        t3787 = t4058 * (t4035 * t4051 + t4048 * t4042 + t4046 * t4038)
        t4068 = t3787 * t4066
        t4070 = (t4031 - t4068) * t236
        t4071 = t4070 / 0.2E1
        t4077 = (t3983 - t717) * t183
        t4079 = t4077 / 0.2E1 + t834 / 0.2E1
        t3800 = t4019 * (t4005 * t4012 + t3997 * t4003 + t4001 * t3999)
        t4081 = t3800 * t4079
        t4083 = t708 * t3554
        t4085 = (t4081 - t4083) * t236
        t4086 = t4085 / 0.2E1
        t4092 = (t3986 - t720) * t183
        t4094 = t4092 / 0.2E1 + t851 / 0.2E1
        t3811 = t4058 * (t4044 * t4051 + t4036 * t4042 + t4040 * t4038)
        t4096 = t3811 * t4094
        t4098 = (t4083 - t4096) * t236
        t4099 = t4098 / 0.2E1
        t4100 = t4012 ** 2
        t4101 = t4003 ** 2
        t4102 = t3999 ** 2
        t4104 = t4018 * (t4100 + t4101 + t4102)
        t4105 = t626 ** 2
        t4106 = t617 ** 2
        t4107 = t613 ** 2
        t4109 = t632 * (t4105 + t4106 + t4107)
        t4112 = t4 * (t4104 / 0.2E1 + t4109 / 0.2E1)
        t4113 = t4112 * t719
        t4114 = t4051 ** 2
        t4115 = t4042 ** 2
        t4116 = t4038 ** 2
        t4118 = t4057 * (t4114 + t4115 + t4116)
        t4121 = t4 * (t4109 / 0.2E1 + t4118 / 0.2E1)
        t4122 = t4121 * t722
        t4124 = (t4113 - t4122) * t236
        t4125 = t3898 + t3559 + t3913 + t3568 + t3930 + t3967 + t650 + t
     #3978 + t3995 + t735 + t4034 + t4071 + t4086 + t4099 + t4124
        t4126 = t4125 * t631
        t4127 = t4126 - t887
        t4128 = t4127 * t183
        t4129 = rx(t96,t185,k,0,0)
        t4130 = rx(t96,t185,k,1,1)
        t4132 = rx(t96,t185,k,2,2)
        t4134 = rx(t96,t185,k,1,2)
        t4136 = rx(t96,t185,k,2,1)
        t4138 = rx(t96,t185,k,1,0)
        t4140 = rx(t96,t185,k,0,2)
        t4142 = rx(t96,t185,k,0,1)
        t4145 = rx(t96,t185,k,2,0)
        t4150 = t4129 * t4130 * t4132 - t4129 * t4134 * t4136 + t4138 * 
     #t4136 * t4140 - t4138 * t4142 * t4132 + t4145 * t4142 * t4134 - t4
     #145 * t4130 * t4140
        t4151 = 0.1E1 / t4150
        t4152 = t4129 ** 2
        t4153 = t4142 ** 2
        t4154 = t4140 ** 2
        t4156 = t4151 * (t4152 + t4153 + t4154)
        t4159 = t4 * (t3704 / 0.2E1 + t4156 / 0.2E1)
        t4160 = t4159 * t680
        t4162 = (t3708 - t4160) * t94
        t4163 = t4 * t4151
        t4168 = u(t96,t1379,k,n)
        t4170 = (t583 - t4168) * t183
        t4172 = t585 / 0.2E1 + t4170 / 0.2E1
        t3877 = t4163 * (t4129 * t4138 + t4142 * t4130 + t4140 * t4134)
        t4174 = t3877 * t4172
        t4176 = (t3714 - t4174) * t94
        t4177 = t4176 / 0.2E1
        t4182 = u(t96,t185,t233,n)
        t4184 = (t4182 - t583) * t236
        t4185 = u(t96,t185,t238,n)
        t4187 = (t583 - t4185) * t236
        t4189 = t4184 / 0.2E1 + t4187 / 0.2E1
        t3893 = t4163 * (t4129 * t4145 + t4142 * t4136 + t4140 * t4132)
        t4191 = t3893 * t4189
        t4193 = (t3723 - t4191) * t94
        t4194 = t4193 / 0.2E1
        t4195 = rx(i,t1379,k,0,0)
        t4196 = rx(i,t1379,k,1,1)
        t4198 = rx(i,t1379,k,2,2)
        t4200 = rx(i,t1379,k,1,2)
        t4202 = rx(i,t1379,k,2,1)
        t4204 = rx(i,t1379,k,1,0)
        t4206 = rx(i,t1379,k,0,2)
        t4208 = rx(i,t1379,k,0,1)
        t4211 = rx(i,t1379,k,2,0)
        t4216 = t4195 * t4196 * t4198 - t4195 * t4200 * t4202 + t4204 * 
     #t4202 * t4206 - t4204 * t4208 * t4198 + t4211 * t4208 * t4200 - t4
     #211 * t4196 * t4206
        t4217 = 0.1E1 / t4216
        t4218 = t4 * t4217
        t4222 = t4195 * t4204 + t4208 * t4196 + t4206 * t4200
        t4224 = (t1803 - t4168) * t94
        t4226 = t1805 / 0.2E1 + t4224 / 0.2E1
        t3935 = t4218 * t4222
        t4228 = t3935 * t4226
        t4230 = (t684 - t4228) * t183
        t4231 = t4230 / 0.2E1
        t4232 = t4204 ** 2
        t4233 = t4196 ** 2
        t4234 = t4200 ** 2
        t4236 = t4217 * (t4232 + t4233 + t4234)
        t4239 = t4 * (t706 / 0.2E1 + t4236 / 0.2E1)
        t4240 = t4239 * t2041
        t4242 = (t710 - t4240) * t183
        t4246 = t4204 * t4211 + t4196 * t4202 + t4200 * t4198
        t4247 = u(i,t1379,t233,n)
        t4249 = (t4247 - t1803) * t236
        t4250 = u(i,t1379,t238,n)
        t4252 = (t1803 - t4250) * t236
        t4254 = t4249 / 0.2E1 + t4252 / 0.2E1
        t3956 = t4218 * t4246
        t4256 = t3956 * t4254
        t4258 = (t749 - t4256) * t183
        t4259 = t4258 / 0.2E1
        t4260 = rx(i,t185,t233,0,0)
        t4261 = rx(i,t185,t233,1,1)
        t4263 = rx(i,t185,t233,2,2)
        t4265 = rx(i,t185,t233,1,2)
        t4267 = rx(i,t185,t233,2,1)
        t4269 = rx(i,t185,t233,1,0)
        t4271 = rx(i,t185,t233,0,2)
        t4273 = rx(i,t185,t233,0,1)
        t4276 = rx(i,t185,t233,2,0)
        t4281 = t4260 * t4261 * t4263 - t4260 * t4265 * t4267 + t4267 * 
     #t4269 * t4271 - t4269 * t4273 * t4263 + t4276 * t4273 * t4265 - t4
     #276 * t4261 * t4271
        t4282 = 0.1E1 / t4281
        t4283 = t4 * t4282
        t4289 = (t740 - t4182) * t94
        t4291 = t3758 / 0.2E1 + t4289 / 0.2E1
        t4000 = t4283 * (t4260 * t4276 + t4273 * t4267 + t4263 * t4271)
        t4293 = t4000 * t4291
        t4295 = t3481 * t682
        t4297 = (t4293 - t4295) * t236
        t4298 = t4297 / 0.2E1
        t4299 = rx(i,t185,t238,0,0)
        t4300 = rx(i,t185,t238,1,1)
        t4302 = rx(i,t185,t238,2,2)
        t4304 = rx(i,t185,t238,1,2)
        t4306 = rx(i,t185,t238,2,1)
        t4308 = rx(i,t185,t238,1,0)
        t4310 = rx(i,t185,t238,0,2)
        t4312 = rx(i,t185,t238,0,1)
        t4315 = rx(i,t185,t238,2,0)
        t4320 = t4299 * t4300 * t4302 - t4299 * t4304 * t4306 + t4308 * 
     #t4306 * t4310 - t4308 * t4312 * t4302 + t4315 * t4312 * t4304 - t4
     #315 * t4300 * t4310
        t4321 = 0.1E1 / t4320
        t4322 = t4 * t4321
        t4328 = (t743 - t4185) * t94
        t4330 = t3797 / 0.2E1 + t4328 / 0.2E1
        t4037 = t4322 * (t4299 * t4315 + t4312 * t4306 + t4310 * t4302)
        t4332 = t4037 * t4330
        t4334 = (t4295 - t4332) * t236
        t4335 = t4334 / 0.2E1
        t4341 = (t740 - t4247) * t183
        t4343 = t836 / 0.2E1 + t4341 / 0.2E1
        t4050 = t4283 * (t4269 * t4276 + t4261 * t4267 + t4265 * t4263)
        t4345 = t4050 * t4343
        t4347 = t731 * t3712
        t4349 = (t4345 - t4347) * t236
        t4350 = t4349 / 0.2E1
        t4356 = (t743 - t4250) * t183
        t4358 = t853 / 0.2E1 + t4356 / 0.2E1
        t4061 = t4322 * (t4308 * t4315 + t4300 * t4306 + t4304 * t4302)
        t4360 = t4061 * t4358
        t4362 = (t4347 - t4360) * t236
        t4363 = t4362 / 0.2E1
        t4364 = t4276 ** 2
        t4365 = t4267 ** 2
        t4366 = t4263 ** 2
        t4368 = t4282 * (t4364 + t4365 + t4366)
        t4369 = t667 ** 2
        t4370 = t658 ** 2
        t4371 = t654 ** 2
        t4373 = t673 * (t4369 + t4370 + t4371)
        t4376 = t4 * (t4368 / 0.2E1 + t4373 / 0.2E1)
        t4377 = t4376 * t742
        t4378 = t4315 ** 2
        t4379 = t4306 ** 2
        t4380 = t4302 ** 2
        t4382 = t4321 * (t4378 + t4379 + t4380)
        t4385 = t4 * (t4373 / 0.2E1 + t4382 / 0.2E1)
        t4386 = t4385 * t745
        t4388 = (t4377 - t4386) * t236
        t4389 = t4162 + t3717 + t4177 + t3726 + t4194 + t687 + t4231 + t
     #4242 + t752 + t4259 + t4298 + t4335 + t4350 + t4363 + t4388
        t4390 = t4389 * t672
        t4391 = t887 - t4390
        t4392 = t4391 * t183
        t4394 = t4128 / 0.2E1 + t4392 / 0.2E1
        t4396 = t214 * t4394
        t4399 = (t3861 - t4396) * t94 / 0.2E1
        t4400 = rx(t1228,j,t233,0,0)
        t4401 = rx(t1228,j,t233,1,1)
        t4403 = rx(t1228,j,t233,2,2)
        t4405 = rx(t1228,j,t233,1,2)
        t4407 = rx(t1228,j,t233,2,1)
        t4409 = rx(t1228,j,t233,1,0)
        t4411 = rx(t1228,j,t233,0,2)
        t4413 = rx(t1228,j,t233,0,1)
        t4416 = rx(t1228,j,t233,2,0)
        t4422 = 0.1E1 / (t4400 * t4401 * t4403 - t4400 * t4405 * t4407 +
     # t4409 * t4407 * t4411 - t4409 * t4413 * t4403 + t4416 * t4413 * t
     #4405 - t4416 * t4401 * t4411)
        t4423 = t4400 ** 2
        t4424 = t4413 ** 2
        t4425 = t4411 ** 2
        t4428 = t2772 ** 2
        t4429 = t2785 ** 2
        t4430 = t2783 ** 2
        t4432 = t2794 * (t4428 + t4429 + t4430)
        t4437 = t427 ** 2
        t4438 = t440 ** 2
        t4439 = t438 ** 2
        t4441 = t449 * (t4437 + t4438 + t4439)
        t4444 = t4 * (t4432 / 0.2E1 + t4441 / 0.2E1)
        t4445 = t4444 * t456
        t4448 = t4 * t4422
        t4141 = t2795 * (t2772 * t2781 + t2785 * t2773 + t2783 * t2777)
        t4466 = t4141 * t2853
        t4147 = t450 * (t427 * t436 + t440 * t428 + t438 * t432)
        t4475 = t4147 * t516
        t4477 = (t4466 - t4475) * t94
        t4478 = t4477 / 0.2E1
        t4483 = u(t1228,j,t1430,n)
        t4491 = t1684 / 0.2E1 + t237 / 0.2E1
        t4493 = t2608 * t4491
        t4498 = t1433 / 0.2E1 + t252 / 0.2E1
        t4500 = t452 * t4498
        t4502 = (t4493 - t4500) * t94
        t4503 = t4502 / 0.2E1
        t4511 = t4141 * t2801
        t4524 = t3102 ** 2
        t4525 = t3094 ** 2
        t4526 = t3098 ** 2
        t4529 = t2781 ** 2
        t4530 = t2773 ** 2
        t4531 = t2777 ** 2
        t4533 = t2794 * (t4529 + t4530 + t4531)
        t4538 = t3410 ** 2
        t4539 = t3402 ** 2
        t4540 = t3406 ** 2
        t4549 = u(t64,t180,t1430,n)
        t4553 = (t4549 - t2736) * t236 / 0.2E1 + t2738 / 0.2E1
        t4557 = t2647 * t4491
        t4561 = u(t64,t185,t1430,n)
        t4565 = (t4561 - t2759) * t236 / 0.2E1 + t2761 / 0.2E1
        t4571 = rx(t64,j,t1430,0,0)
        t4572 = rx(t64,j,t1430,1,1)
        t4574 = rx(t64,j,t1430,2,2)
        t4576 = rx(t64,j,t1430,1,2)
        t4578 = rx(t64,j,t1430,2,1)
        t4580 = rx(t64,j,t1430,1,0)
        t4582 = rx(t64,j,t1430,0,2)
        t4584 = rx(t64,j,t1430,0,1)
        t4587 = rx(t64,j,t1430,2,0)
        t4593 = 0.1E1 / (t4571 * t4572 * t4574 - t4571 * t4576 * t4578 +
     # t4580 * t4578 * t4582 - t4580 * t4584 * t4574 + t4587 * t4584 * t
     #4576 - t4587 * t4572 * t4582)
        t4594 = t4 * t4593
        t4617 = (t4549 - t1682) * t183 / 0.2E1 + (t1682 - t4561) * t183 
     #/ 0.2E1
        t4623 = t4587 ** 2
        t4624 = t4578 ** 2
        t4625 = t4574 ** 2
        t4305 = t3116 * (t3093 * t3102 + t3106 * t3094 + t3104 * t3098)
        t4314 = t3424 * (t3401 * t3410 + t3414 * t3402 + t3412 * t3406)
        t4367 = t4594 * (t4571 * t4587 + t4584 * t4578 + t4582 * t4574)
        t4634 = (t4 * (t4422 * (t4423 + t4424 + t4425) / 0.2E1 + t4432 /
     # 0.2E1) * t1897 - t4445) * t94 + (t4448 * (t4400 * t4409 + t4413 *
     # t4401 + t4411 * t4405) * ((t3000 - t1304) * t183 / 0.2E1 + (t1304
     # - t3308) * t183 / 0.2E1) - t4466) * t94 / 0.2E1 + t4478 + (t4448 
     #* (t4400 * t4416 + t4413 * t4407 + t4411 * t4403) * ((t4483 - t130
     #4) * t236 / 0.2E1 + t1306 / 0.2E1) - t4493) * t94 / 0.2E1 + t4503 
     #+ (t4305 * t3126 - t4511) * t183 / 0.2E1 + (t4511 - t4314 * t3434)
     # * t183 / 0.2E1 + (t4 * (t3115 * (t4524 + t4525 + t4526) / 0.2E1 +
     # t4533 / 0.2E1) * t2849 - t4 * (t4533 / 0.2E1 + t3423 * (t4538 + t
     #4539 + t4540) / 0.2E1) * t2851) * t183 + (t3020 * t4553 - t4557) *
     # t183 / 0.2E1 + (t4557 - t3320 * t4565) * t183 / 0.2E1 + (t4367 * 
     #((t4483 - t1682) * t94 / 0.2E1 + t1737 / 0.2E1) - t2803) * t236 / 
     #0.2E1 + t2808 + (t4594 * (t4580 * t4587 + t4572 * t4578 + t4576 * 
     #t4574) * t4617 - t2855) * t236 / 0.2E1 + t2860 + (t4 * (t4593 * (t
     #4623 + t4624 + t4625) / 0.2E1 + t2880 / 0.2E1) * t1684 - t2889) * 
     #t236
        t4635 = t4634 * t2793
        t4638 = rx(t1228,j,t238,0,0)
        t4639 = rx(t1228,j,t238,1,1)
        t4641 = rx(t1228,j,t238,2,2)
        t4643 = rx(t1228,j,t238,1,2)
        t4645 = rx(t1228,j,t238,2,1)
        t4647 = rx(t1228,j,t238,1,0)
        t4649 = rx(t1228,j,t238,0,2)
        t4651 = rx(t1228,j,t238,0,1)
        t4654 = rx(t1228,j,t238,2,0)
        t4660 = 0.1E1 / (t4638 * t4639 * t4641 - t4638 * t4643 * t4645 +
     # t4647 * t4645 * t4649 - t4647 * t4651 * t4641 + t4654 * t4651 * t
     #4643 - t4654 * t4639 * t4649)
        t4661 = t4638 ** 2
        t4662 = t4651 ** 2
        t4663 = t4649 ** 2
        t4666 = t2809 ** 2
        t4667 = t2822 ** 2
        t4668 = t2820 ** 2
        t4670 = t2831 * (t4666 + t4667 + t4668)
        t4675 = t468 ** 2
        t4676 = t481 ** 2
        t4677 = t479 ** 2
        t4679 = t490 * (t4675 + t4676 + t4677)
        t4682 = t4 * (t4670 / 0.2E1 + t4679 / 0.2E1)
        t4683 = t4682 * t497
        t4686 = t4 * t4660
        t4458 = t2832 * (t2809 * t2818 + t2822 * t2810 + t2820 * t2814)
        t4704 = t4458 * t2870
        t4462 = t491 * (t468 * t477 + t481 * t469 + t479 * t473)
        t4713 = t4462 * t533
        t4715 = (t4704 - t4713) * t94
        t4716 = t4715 / 0.2E1
        t4721 = u(t1228,j,t1441,n)
        t4729 = t241 / 0.2E1 + t1690 / 0.2E1
        t4731 = t2630 * t4729
        t4736 = t255 / 0.2E1 + t1444 / 0.2E1
        t4738 = t492 * t4736
        t4740 = (t4731 - t4738) * t94
        t4741 = t4740 / 0.2E1
        t4749 = t4458 * t2838
        t4762 = t3143 ** 2
        t4763 = t3135 ** 2
        t4764 = t3139 ** 2
        t4767 = t2818 ** 2
        t4768 = t2810 ** 2
        t4769 = t2814 ** 2
        t4771 = t2831 * (t4767 + t4768 + t4769)
        t4776 = t3451 ** 2
        t4777 = t3443 ** 2
        t4778 = t3447 ** 2
        t4787 = u(t64,t180,t1441,n)
        t4791 = t2741 / 0.2E1 + (t2739 - t4787) * t236 / 0.2E1
        t4795 = t2661 * t4729
        t4799 = u(t64,t185,t1441,n)
        t4803 = t2764 / 0.2E1 + (t2762 - t4799) * t236 / 0.2E1
        t4809 = rx(t64,j,t1441,0,0)
        t4810 = rx(t64,j,t1441,1,1)
        t4812 = rx(t64,j,t1441,2,2)
        t4814 = rx(t64,j,t1441,1,2)
        t4816 = rx(t64,j,t1441,2,1)
        t4818 = rx(t64,j,t1441,1,0)
        t4820 = rx(t64,j,t1441,0,2)
        t4822 = rx(t64,j,t1441,0,1)
        t4825 = rx(t64,j,t1441,2,0)
        t4831 = 0.1E1 / (t4809 * t4810 * t4812 - t4809 * t4814 * t4816 +
     # t4818 * t4816 * t4820 - t4818 * t4822 * t4812 + t4825 * t4822 * t
     #4814 - t4825 * t4810 * t4820)
        t4832 = t4 * t4831
        t4855 = (t4787 - t1688) * t183 / 0.2E1 + (t1688 - t4799) * t183 
     #/ 0.2E1
        t4861 = t4825 ** 2
        t4862 = t4816 ** 2
        t4863 = t4812 ** 2
        t4568 = t3157 * (t3134 * t3143 + t3147 * t3135 + t3145 * t3139)
        t4577 = t3465 * (t3442 * t3451 + t3455 * t3443 + t3453 * t3447)
        t4612 = t4832 * (t4809 * t4825 + t4822 * t4816 + t4820 * t4812)
        t4872 = (t4 * (t4660 * (t4661 + t4662 + t4663) / 0.2E1 + t4670 /
     # 0.2E1) * t1914 - t4683) * t94 + (t4686 * (t4638 * t4647 + t4651 *
     # t4639 + t4649 * t4643) * ((t3003 - t1307) * t183 / 0.2E1 + (t1307
     # - t3311) * t183 / 0.2E1) - t4704) * t94 / 0.2E1 + t4716 + (t4686 
     #* (t4638 * t4654 + t4651 * t4645 + t4649 * t4641) * (t1309 / 0.2E1
     # + (t1307 - t4721) * t236 / 0.2E1) - t4731) * t94 / 0.2E1 + t4741 
     #+ (t4568 * t3167 - t4749) * t183 / 0.2E1 + (t4749 - t4577 * t3475)
     # * t183 / 0.2E1 + (t4 * (t3156 * (t4762 + t4763 + t4764) / 0.2E1 +
     # t4771 / 0.2E1) * t2866 - t4 * (t4771 / 0.2E1 + t3464 * (t4776 + t
     #4777 + t4778) / 0.2E1) * t2868) * t183 + (t3032 * t4791 - t4795) *
     # t183 / 0.2E1 + (t4795 - t3327 * t4803) * t183 / 0.2E1 + t2843 + (
     #t2840 - t4612 * ((t4721 - t1688) * t94 / 0.2E1 + t1757 / 0.2E1)) *
     # t236 / 0.2E1 + t2875 + (t2872 - t4832 * (t4818 * t4825 + t4810 * 
     #t4816 + t4814 * t4812) * t4855) * t236 / 0.2E1 + (t2898 - t4 * (t2
     #894 / 0.2E1 + t4831 * (t4861 + t4862 + t4863) / 0.2E1) * t1690) * 
     #t236
        t4873 = t4872 * t2830
        t4880 = t753 ** 2
        t4881 = t766 ** 2
        t4882 = t764 ** 2
        t4884 = t775 * (t4880 + t4881 + t4882)
        t4887 = t4 * (t4441 / 0.2E1 + t4884 / 0.2E1)
        t4888 = t4887 * t458
        t4890 = (t4445 - t4888) * t94
        t4664 = t776 * (t753 * t762 + t766 * t754 + t764 * t758)
        t4896 = t4664 * t838
        t4898 = (t4475 - t4896) * t94
        t4899 = t4898 / 0.2E1
        t4901 = t1712 / 0.2E1 + t269 / 0.2E1
        t4903 = t771 * t4901
        t4905 = (t4500 - t4903) * t94
        t4906 = t4905 / 0.2E1
        t4678 = t3594 * (t3571 * t3580 + t3584 * t3572 + t3582 * t3576)
        t4912 = t4678 * t3602
        t4914 = t4147 * t460
        t4917 = (t4912 - t4914) * t183 / 0.2E1
        t4688 = t3752 * (t3729 * t3738 + t3742 * t3730 + t3740 * t3734)
        t4923 = t4688 * t3760
        t4926 = (t4914 - t4923) * t183 / 0.2E1
        t4927 = t3580 ** 2
        t4928 = t3572 ** 2
        t4929 = t3576 ** 2
        t4931 = t3593 * (t4927 + t4928 + t4929)
        t4932 = t436 ** 2
        t4933 = t428 ** 2
        t4934 = t432 ** 2
        t4936 = t449 * (t4932 + t4933 + t4934)
        t4939 = t4 * (t4931 / 0.2E1 + t4936 / 0.2E1)
        t4940 = t4939 * t512
        t4941 = t3738 ** 2
        t4942 = t3730 ** 2
        t4943 = t3734 ** 2
        t4945 = t3751 * (t4941 + t4942 + t4943)
        t4948 = t4 * (t4936 / 0.2E1 + t4945 / 0.2E1)
        t4949 = t4948 * t514
        t4953 = t1978 / 0.2E1 + t393 / 0.2E1
        t4955 = t3437 * t4953
        t4957 = t507 * t4498
        t4960 = (t4955 - t4957) * t183 / 0.2E1
        t4962 = t1997 / 0.2E1 + t416 / 0.2E1
        t4964 = t3548 * t4962
        t4967 = (t4957 - t4964) * t183 / 0.2E1
        t4968 = t1745 / 0.2E1
        t4969 = t1545 / 0.2E1
        t4970 = t4890 + t4478 + t4899 + t4503 + t4906 + t4917 + t4926 + 
     #(t4940 - t4949) * t183 + t4960 + t4967 + t4968 + t467 + t4969 + t5
     #23 + t1485
        t4971 = t4970 * t448
        t4973 = (t4971 - t565) * t236
        t4974 = t792 ** 2
        t4975 = t805 ** 2
        t4976 = t803 ** 2
        t4978 = t814 * (t4974 + t4975 + t4976)
        t4981 = t4 * (t4679 / 0.2E1 + t4978 / 0.2E1)
        t4982 = t499 * t4981
        t4984 = (t4683 - t4982) * t94
        t4720 = t815 * (t792 * t801 + t805 * t793 + t803 * t797)
        t4990 = t4720 * t855
        t4992 = (t4713 - t4990) * t94
        t4993 = t4992 / 0.2E1
        t4995 = t272 / 0.2E1 + t1718 / 0.2E1
        t4997 = t809 * t4995
        t4999 = (t4738 - t4997) * t94
        t5000 = t4999 / 0.2E1
        t4728 = t3633 * (t3610 * t3619 + t3623 * t3611 + t3621 * t3615)
        t5006 = t4728 * t3641
        t5008 = t4462 * t501
        t5011 = (t5006 - t5008) * t183 / 0.2E1
        t4737 = t3791 * (t3768 * t3777 + t3781 * t3769 + t3773 * t3779)
        t5017 = t4737 * t3799
        t5020 = (t5008 - t5017) * t183 / 0.2E1
        t5021 = t3619 ** 2
        t5022 = t3611 ** 2
        t5023 = t3615 ** 2
        t5025 = t3632 * (t5021 + t5022 + t5023)
        t5026 = t477 ** 2
        t5027 = t469 ** 2
        t5028 = t473 ** 2
        t5030 = t490 * (t5026 + t5027 + t5028)
        t5033 = t4 * (t5025 / 0.2E1 + t5030 / 0.2E1)
        t5034 = t5033 * t529
        t5035 = t3777 ** 2
        t5036 = t3769 ** 2
        t5037 = t3773 ** 2
        t5039 = t3790 * (t5035 + t5036 + t5037)
        t5042 = t4 * (t5030 / 0.2E1 + t5039 / 0.2E1)
        t5043 = t5042 * t531
        t5047 = t396 / 0.2E1 + t1983 / 0.2E1
        t5049 = t3450 * t5047
        t5051 = t521 * t4736
        t5054 = (t5049 - t5051) * t183 / 0.2E1
        t5056 = t419 / 0.2E1 + t2002 / 0.2E1
        t5058 = t3562 * t5056
        t5061 = (t5051 - t5058) * t183 / 0.2E1
        t5062 = t1765 / 0.2E1
        t5063 = t1568 / 0.2E1
        t5064 = t4984 + t4716 + t4993 + t4741 + t5000 + t5011 + t5020 + 
     #(t5034 - t5043) * t183 + t5054 + t5061 + t506 + t5062 + t538 + t50
     #63 + t1521
        t5065 = t5064 * t489
        t5067 = (t565 - t5065) * t236
        t5069 = t4973 / 0.2E1 + t5067 / 0.2E1
        t5071 = t248 * t5069
        t5075 = rx(t96,j,t233,0,0)
        t5076 = rx(t96,j,t233,1,1)
        t5078 = rx(t96,j,t233,2,2)
        t5080 = rx(t96,j,t233,1,2)
        t5082 = rx(t96,j,t233,2,1)
        t5084 = rx(t96,j,t233,1,0)
        t5086 = rx(t96,j,t233,0,2)
        t5088 = rx(t96,j,t233,0,1)
        t5091 = rx(t96,j,t233,2,0)
        t5096 = t5075 * t5076 * t5078 - t5075 * t5080 * t5082 + t5084 * 
     #t5082 * t5086 - t5084 * t5088 * t5078 + t5091 * t5088 * t5080 - t5
     #091 * t5076 * t5086
        t5097 = 0.1E1 / t5096
        t5098 = t5075 ** 2
        t5099 = t5088 ** 2
        t5100 = t5086 ** 2
        t5102 = t5097 * (t5098 + t5099 + t5100)
        t5105 = t4 * (t4884 / 0.2E1 + t5102 / 0.2E1)
        t5106 = t5105 * t782
        t5108 = (t4888 - t5106) * t94
        t5109 = t4 * t5097
        t5115 = (t3918 - t597) * t183
        t5117 = (t597 - t4182) * t183
        t5119 = t5115 / 0.2E1 + t5117 / 0.2E1
        t4798 = t5109 * (t5075 * t5084 + t5076 * t5088 + t5086 * t5080)
        t5121 = t4798 * t5119
        t5123 = (t4896 - t5121) * t94
        t5124 = t5123 / 0.2E1
        t5129 = u(t96,j,t1430,n)
        t5131 = (t5129 - t597) * t236
        t5133 = t5131 / 0.2E1 + t599 / 0.2E1
        t4807 = t5109 * (t5075 * t5091 + t5088 * t5082 + t5086 * t5078)
        t5135 = t4807 * t5133
        t5137 = (t4903 - t5135) * t94
        t5138 = t5137 / 0.2E1
        t4817 = t4019 * (t3996 * t4005 + t4009 * t3997 + t4007 * t4001)
        t5144 = t4817 * t4027
        t5146 = t4664 * t784
        t5148 = (t5144 - t5146) * t183
        t5149 = t5148 / 0.2E1
        t4826 = t4283 * (t4260 * t4269 + t4261 * t4273 + t4271 * t4265)
        t5155 = t4826 * t4291
        t5157 = (t5146 - t5155) * t183
        t5158 = t5157 / 0.2E1
        t5159 = t4005 ** 2
        t5160 = t3997 ** 2
        t5161 = t4001 ** 2
        t5163 = t4018 * (t5159 + t5160 + t5161)
        t5164 = t762 ** 2
        t5165 = t754 ** 2
        t5166 = t758 ** 2
        t5168 = t775 * (t5164 + t5165 + t5166)
        t5171 = t4 * (t5163 / 0.2E1 + t5168 / 0.2E1)
        t5172 = t5171 * t834
        t5173 = t4269 ** 2
        t5174 = t4261 ** 2
        t5175 = t4265 ** 2
        t5177 = t4282 * (t5173 + t5174 + t5175)
        t5180 = t4 * (t5168 / 0.2E1 + t5177 / 0.2E1)
        t5181 = t5180 * t836
        t5183 = (t5172 - t5181) * t183
        t5184 = u(i,t180,t1430,n)
        t5186 = (t5184 - t717) * t236
        t5188 = t5186 / 0.2E1 + t719 / 0.2E1
        t5190 = t3800 * t5188
        t5192 = t822 * t4901
        t5194 = (t5190 - t5192) * t183
        t5195 = t5194 / 0.2E1
        t5196 = u(i,t185,t1430,n)
        t5198 = (t5196 - t740) * t236
        t5200 = t5198 / 0.2E1 + t742 / 0.2E1
        t5202 = t4050 * t5200
        t5204 = (t5192 - t5202) * t183
        t5205 = t5204 / 0.2E1
        t5206 = rx(i,j,t1430,0,0)
        t5207 = rx(i,j,t1430,1,1)
        t5209 = rx(i,j,t1430,2,2)
        t5211 = rx(i,j,t1430,1,2)
        t5213 = rx(i,j,t1430,2,1)
        t5215 = rx(i,j,t1430,1,0)
        t5217 = rx(i,j,t1430,0,2)
        t5219 = rx(i,j,t1430,0,1)
        t5222 = rx(i,j,t1430,2,0)
        t5227 = t5206 * t5207 * t5209 - t5206 * t5211 * t5213 + t5215 * 
     #t5213 * t5217 - t5215 * t5219 * t5209 + t5222 * t5219 * t5211 - t5
     #222 * t5207 * t5217
        t5228 = 0.1E1 / t5227
        t5229 = t4 * t5228
        t5233 = t5206 * t5222 + t5219 * t5213 + t5217 * t5209
        t5235 = (t1710 - t5129) * t94
        t5237 = t1739 / 0.2E1 + t5235 / 0.2E1
        t4866 = t5229 * t5233
        t5239 = t4866 * t5237
        t5241 = (t5239 - t786) * t236
        t5242 = t5241 / 0.2E1
        t5246 = t5215 * t5222 + t5207 * t5213 + t5211 * t5209
        t5248 = (t5184 - t1710) * t183
        t5250 = (t1710 - t5196) * t183
        t5252 = t5248 / 0.2E1 + t5250 / 0.2E1
        t4876 = t5229 * t5246
        t5254 = t4876 * t5252
        t5256 = (t5254 - t840) * t236
        t5257 = t5256 / 0.2E1
        t5258 = t5222 ** 2
        t5259 = t5213 ** 2
        t5260 = t5209 ** 2
        t5262 = t5228 * (t5258 + t5259 + t5260)
        t5265 = t4 * (t5262 / 0.2E1 + t865 / 0.2E1)
        t5266 = t5265 * t1712
        t5268 = (t5266 - t874) * t236
        t5269 = t5108 + t4899 + t5124 + t4906 + t5138 + t5149 + t5158 + 
     #t5183 + t5195 + t5205 + t5242 + t791 + t5257 + t845 + t5268
        t5270 = t5269 * t774
        t5271 = t5270 - t887
        t5272 = t5271 * t236
        t5273 = rx(t96,j,t238,0,0)
        t5274 = rx(t96,j,t238,1,1)
        t5276 = rx(t96,j,t238,2,2)
        t5278 = rx(t96,j,t238,1,2)
        t5280 = rx(t96,j,t238,2,1)
        t5282 = rx(t96,j,t238,1,0)
        t5284 = rx(t96,j,t238,0,2)
        t5286 = rx(t96,j,t238,0,1)
        t5289 = rx(t96,j,t238,2,0)
        t5294 = t5273 * t5274 * t5276 - t5273 * t5278 * t5280 + t5282 * 
     #t5280 * t5284 - t5282 * t5286 * t5276 + t5289 * t5286 * t5278 - t5
     #289 * t5274 * t5284
        t5295 = 0.1E1 / t5294
        t5296 = t5273 ** 2
        t5297 = t5286 ** 2
        t5298 = t5284 ** 2
        t5300 = t5295 * (t5296 + t5297 + t5298)
        t5303 = t4 * (t4978 / 0.2E1 + t5300 / 0.2E1)
        t5304 = t5303 * t821
        t5306 = (t4982 - t5304) * t94
        t5307 = t4 * t5295
        t5313 = (t3921 - t600) * t183
        t5315 = (t600 - t4185) * t183
        t5317 = t5313 / 0.2E1 + t5315 / 0.2E1
        t4925 = t5307 * (t5273 * t5282 + t5286 * t5274 + t5284 * t5278)
        t5319 = t4925 * t5317
        t5321 = (t4990 - t5319) * t94
        t5322 = t5321 / 0.2E1
        t5327 = u(t96,j,t1441,n)
        t5329 = (t600 - t5327) * t236
        t5331 = t602 / 0.2E1 + t5329 / 0.2E1
        t4947 = t5307 * (t5273 * t5289 + t5286 * t5280 + t5284 * t5276)
        t5333 = t4947 * t5331
        t5335 = (t4997 - t5333) * t94
        t5336 = t5335 / 0.2E1
        t4956 = t4058 * (t4035 * t4044 + t4048 * t4036 + t4046 * t4040)
        t5342 = t4956 * t4066
        t5344 = t4720 * t823
        t5346 = (t5342 - t5344) * t183
        t5347 = t5346 / 0.2E1
        t4965 = t4322 * (t4299 * t4308 + t4312 * t4300 + t4310 * t4304)
        t5353 = t4965 * t4330
        t5355 = (t5344 - t5353) * t183
        t5356 = t5355 / 0.2E1
        t5357 = t4044 ** 2
        t5358 = t4036 ** 2
        t5359 = t4040 ** 2
        t5361 = t4057 * (t5357 + t5358 + t5359)
        t5362 = t801 ** 2
        t5363 = t793 ** 2
        t5364 = t797 ** 2
        t5366 = t814 * (t5362 + t5363 + t5364)
        t5369 = t4 * (t5361 / 0.2E1 + t5366 / 0.2E1)
        t5370 = t5369 * t851
        t5371 = t4308 ** 2
        t5372 = t4300 ** 2
        t5373 = t4304 ** 2
        t5375 = t4321 * (t5371 + t5372 + t5373)
        t5378 = t4 * (t5366 / 0.2E1 + t5375 / 0.2E1)
        t5379 = t5378 * t853
        t5381 = (t5370 - t5379) * t183
        t5382 = u(i,t180,t1441,n)
        t5384 = (t720 - t5382) * t236
        t5386 = t722 / 0.2E1 + t5384 / 0.2E1
        t5388 = t3811 * t5386
        t5390 = t837 * t4995
        t5392 = (t5388 - t5390) * t183
        t5393 = t5392 / 0.2E1
        t5394 = u(i,t185,t1441,n)
        t5396 = (t743 - t5394) * t236
        t5398 = t745 / 0.2E1 + t5396 / 0.2E1
        t5400 = t4061 * t5398
        t5402 = (t5390 - t5400) * t183
        t5403 = t5402 / 0.2E1
        t5404 = rx(i,j,t1441,0,0)
        t5405 = rx(i,j,t1441,1,1)
        t5407 = rx(i,j,t1441,2,2)
        t5409 = rx(i,j,t1441,1,2)
        t5411 = rx(i,j,t1441,2,1)
        t5413 = rx(i,j,t1441,1,0)
        t5415 = rx(i,j,t1441,0,2)
        t5417 = rx(i,j,t1441,0,1)
        t5420 = rx(i,j,t1441,2,0)
        t5425 = t5404 * t5405 * t5407 - t5404 * t5409 * t5411 + t5413 * 
     #t5411 * t5415 - t5413 * t5417 * t5407 + t5420 * t5417 * t5409 - t5
     #420 * t5405 * t5415
        t5426 = 0.1E1 / t5425
        t5427 = t4 * t5426
        t5431 = t5404 * t5420 + t5417 * t5411 + t5415 * t5407
        t5433 = (t1716 - t5327) * t94
        t5435 = t1759 / 0.2E1 + t5433 / 0.2E1
        t5040 = t5427 * t5431
        t5437 = t5040 * t5435
        t5439 = (t825 - t5437) * t236
        t5440 = t5439 / 0.2E1
        t5444 = t5413 * t5420 + t5405 * t5411 + t5409 * t5407
        t5446 = (t5382 - t1716) * t183
        t5448 = (t1716 - t5394) * t183
        t5450 = t5446 / 0.2E1 + t5448 / 0.2E1
        t5053 = t5427 * t5444
        t5452 = t5053 * t5450
        t5454 = (t857 - t5452) * t236
        t5455 = t5454 / 0.2E1
        t5456 = t5420 ** 2
        t5457 = t5411 ** 2
        t5458 = t5407 ** 2
        t5460 = t5426 * (t5456 + t5457 + t5458)
        t5463 = t4 * (t879 / 0.2E1 + t5460 / 0.2E1)
        t5464 = t5463 * t1718
        t5466 = (t883 - t5464) * t236
        t5467 = t5306 + t4993 + t5322 + t5000 + t5336 + t5347 + t5356 + 
     #t5381 + t5393 + t5403 + t828 + t5440 + t860 + t5455 + t5466
        t5468 = t5467 * t813
        t5469 = t887 - t5468
        t5470 = t5469 * t236
        t5472 = t5272 / 0.2E1 + t5470 / 0.2E1
        t5474 = t265 * t5472
        t5477 = (t5071 - t5474) * t94 / 0.2E1
        t5481 = (t3697 - t4126) * t94
        t5487 = t196 * t2907
        t5494 = (t3855 - t4390) * t94
        t5506 = t3093 ** 2
        t5507 = t3106 ** 2
        t5508 = t3104 ** 2
        t5511 = t3571 ** 2
        t5512 = t3584 ** 2
        t5513 = t3582 ** 2
        t5515 = t3593 * (t5511 + t5512 + t5513)
        t5520 = t3996 ** 2
        t5521 = t4009 ** 2
        t5522 = t4007 ** 2
        t5524 = t4018 * (t5520 + t5521 + t5522)
        t5527 = t4 * (t5515 / 0.2E1 + t5524 / 0.2E1)
        t5528 = t5527 * t3600
        t5534 = t4678 * t3652
        t5539 = t4817 * t4079
        t5542 = (t5534 - t5539) * t94 / 0.2E1
        t5546 = t3396 * t4953
        t5551 = t3753 * t5188
        t5554 = (t5546 - t5551) * t94 / 0.2E1
        t5555 = rx(t5,t1331,t233,0,0)
        t5556 = rx(t5,t1331,t233,1,1)
        t5558 = rx(t5,t1331,t233,2,2)
        t5560 = rx(t5,t1331,t233,1,2)
        t5562 = rx(t5,t1331,t233,2,1)
        t5564 = rx(t5,t1331,t233,1,0)
        t5566 = rx(t5,t1331,t233,0,2)
        t5568 = rx(t5,t1331,t233,0,1)
        t5571 = rx(t5,t1331,t233,2,0)
        t5577 = 0.1E1 / (t5555 * t5556 * t5558 - t5555 * t5560 * t5562 +
     # t5564 * t5562 * t5566 - t5564 * t5568 * t5558 + t5571 * t5568 * t
     #5560 - t5571 * t5556 * t5566)
        t5578 = t4 * t5577
        t5586 = (t1360 - t3983) * t94
        t5588 = (t3080 - t1360) * t94 / 0.2E1 + t5586 / 0.2E1
        t5594 = t5564 ** 2
        t5595 = t5556 ** 2
        t5596 = t5560 ** 2
        t5609 = u(t5,t1331,t1430,n)
        t5613 = (t5609 - t1360) * t236 / 0.2E1 + t1363 / 0.2E1
        t5619 = rx(t5,t180,t1430,0,0)
        t5620 = rx(t5,t180,t1430,1,1)
        t5622 = rx(t5,t180,t1430,2,2)
        t5624 = rx(t5,t180,t1430,1,2)
        t5626 = rx(t5,t180,t1430,2,1)
        t5628 = rx(t5,t180,t1430,1,0)
        t5630 = rx(t5,t180,t1430,0,2)
        t5632 = rx(t5,t180,t1430,0,1)
        t5635 = rx(t5,t180,t1430,2,0)
        t5641 = 0.1E1 / (t5619 * t5620 * t5622 - t5619 * t5624 * t5626 +
     # t5628 * t5626 * t5630 - t5628 * t5632 * t5622 + t5635 * t5632 * t
     #5624 - t5635 * t5620 * t5630)
        t5642 = t4 * t5641
        t5650 = (t1534 - t5184) * t94
        t5652 = (t4549 - t1534) * t94 / 0.2E1 + t5650 / 0.2E1
        t5665 = (t5609 - t1534) * t183 / 0.2E1 + t1536 / 0.2E1
        t5671 = t5635 ** 2
        t5672 = t5626 ** 2
        t5673 = t5622 ** 2
        t5238 = t5578 * (t5555 * t5564 + t5568 * t5556 + t5566 * t5560)
        t5267 = t5578 * (t5564 * t5571 + t5556 * t5562 + t5560 * t5558)
        t5283 = t5642 * (t5619 * t5635 + t5632 * t5626 + t5630 * t5622)
        t5291 = t5642 * (t5628 * t5635 + t5620 * t5626 + t5624 * t5622)
        t5682 = (t4 * (t3115 * (t5506 + t5507 + t5508) / 0.2E1 + t5515 /
     # 0.2E1) * t3124 - t5528) * t94 + (t4305 * t3180 - t5534) * t94 / 0
     #.2E1 + t5542 + (t3008 * t4553 - t5546) * t94 / 0.2E1 + t5554 + (t5
     #238 * t5588 - t4912) * t183 / 0.2E1 + t4917 + (t4 * (t5577 * (t559
     #4 + t5595 + t5596) / 0.2E1 + t4931 / 0.2E1) * t1932 - t4940) * t18
     #3 + (t5267 * t5613 - t4955) * t183 / 0.2E1 + t4960 + (t5283 * t565
     #2 - t3604) * t236 / 0.2E1 + t3609 + (t5291 * t5665 - t3654) * t236
     # / 0.2E1 + t3659 + (t4 * (t5641 * (t5671 + t5672 + t5673) / 0.2E1 
     #+ t3675 / 0.2E1) * t1978 - t3684) * t236
        t5683 = t5682 * t3592
        t5686 = t3134 ** 2
        t5687 = t3147 ** 2
        t5688 = t3145 ** 2
        t5691 = t3610 ** 2
        t5692 = t3623 ** 2
        t5693 = t3621 ** 2
        t5695 = t3632 * (t5691 + t5692 + t5693)
        t5700 = t4035 ** 2
        t5701 = t4048 ** 2
        t5702 = t4046 ** 2
        t5704 = t4057 * (t5700 + t5701 + t5702)
        t5707 = t4 * (t5695 / 0.2E1 + t5704 / 0.2E1)
        t5708 = t5707 * t3639
        t5714 = t4728 * t3665
        t5719 = t4956 * t4094
        t5722 = (t5714 - t5719) * t94 / 0.2E1
        t5726 = t3427 * t5047
        t5731 = t3787 * t5386
        t5734 = (t5726 - t5731) * t94 / 0.2E1
        t5735 = rx(t5,t1331,t238,0,0)
        t5736 = rx(t5,t1331,t238,1,1)
        t5738 = rx(t5,t1331,t238,2,2)
        t5740 = rx(t5,t1331,t238,1,2)
        t5742 = rx(t5,t1331,t238,2,1)
        t5744 = rx(t5,t1331,t238,1,0)
        t5746 = rx(t5,t1331,t238,0,2)
        t5748 = rx(t5,t1331,t238,0,1)
        t5751 = rx(t5,t1331,t238,2,0)
        t5757 = 0.1E1 / (t5735 * t5736 * t5738 - t5735 * t5740 * t5742 +
     # t5744 * t5742 * t5746 - t5744 * t5748 * t5738 + t5751 * t5748 * t
     #5740 - t5751 * t5736 * t5746)
        t5758 = t4 * t5757
        t5766 = (t1364 - t3986) * t94
        t5768 = (t3083 - t1364) * t94 / 0.2E1 + t5766 / 0.2E1
        t5774 = t5744 ** 2
        t5775 = t5736 ** 2
        t5776 = t5740 ** 2
        t5789 = u(t5,t1331,t1441,n)
        t5793 = t1366 / 0.2E1 + (t1364 - t5789) * t236 / 0.2E1
        t5799 = rx(t5,t180,t1441,0,0)
        t5800 = rx(t5,t180,t1441,1,1)
        t5802 = rx(t5,t180,t1441,2,2)
        t5804 = rx(t5,t180,t1441,1,2)
        t5806 = rx(t5,t180,t1441,2,1)
        t5808 = rx(t5,t180,t1441,1,0)
        t5810 = rx(t5,t180,t1441,0,2)
        t5812 = rx(t5,t180,t1441,0,1)
        t5815 = rx(t5,t180,t1441,2,0)
        t5821 = 0.1E1 / (t5799 * t5800 * t5802 - t5799 * t5804 * t5806 +
     # t5808 * t5806 * t5810 - t5808 * t5812 * t5802 + t5815 * t5812 * t
     #5804 - t5815 * t5800 * t5810)
        t5822 = t4 * t5821
        t5830 = (t1557 - t5382) * t94
        t5832 = (t4787 - t1557) * t94 / 0.2E1 + t5830 / 0.2E1
        t5845 = (t5789 - t1557) * t183 / 0.2E1 + t1559 / 0.2E1
        t5851 = t5815 ** 2
        t5852 = t5806 ** 2
        t5853 = t5802 ** 2
        t5483 = t5758 * (t5735 * t5744 + t5748 * t5736 + t5746 * t5740)
        t5498 = t5758 * (t5744 * t5751 + t5736 * t5742 + t5740 * t5738)
        t5503 = t5822 * (t5799 * t5815 + t5812 * t5806 + t5810 * t5802)
        t5514 = t5822 * (t5808 * t5815 + t5806 * t5800 + t5804 * t5802)
        t5862 = (t4 * (t3156 * (t5686 + t5687 + t5688) / 0.2E1 + t5695 /
     # 0.2E1) * t3165 - t5708) * t94 + (t4568 * t3195 - t5714) * t94 / 0
     #.2E1 + t5722 + (t3014 * t4791 - t5726) * t94 / 0.2E1 + t5734 + (t5
     #483 * t5768 - t5006) * t183 / 0.2E1 + t5011 + (t4 * (t5757 * (t577
     #4 + t5775 + t5776) / 0.2E1 + t5025 / 0.2E1) * t1958 - t5034) * t18
     #3 + (t5498 * t5793 - t5049) * t183 / 0.2E1 + t5054 + t3646 + (t364
     #3 - t5503 * t5832) * t236 / 0.2E1 + t3670 + (t3667 - t5514 * t5845
     #) * t236 / 0.2E1 + (t3693 - t4 * (t3689 / 0.2E1 + t5821 * (t5851 +
     # t5852 + t5853) / 0.2E1) * t1983) * t236
        t5863 = t5862 * t3631
        t5867 = (t5683 - t3697) * t236 / 0.2E1 + (t3697 - t5863) * t236 
     #/ 0.2E1
        t5871 = t397 * t5069
        t5875 = t3401 ** 2
        t5876 = t3414 ** 2
        t5877 = t3412 ** 2
        t5880 = t3729 ** 2
        t5881 = t3742 ** 2
        t5882 = t3740 ** 2
        t5884 = t3751 * (t5880 + t5881 + t5882)
        t5889 = t4260 ** 2
        t5890 = t4273 ** 2
        t5891 = t4271 ** 2
        t5893 = t4282 * (t5889 + t5890 + t5891)
        t5896 = t4 * (t5884 / 0.2E1 + t5893 / 0.2E1)
        t5897 = t5896 * t3758
        t5903 = t4688 * t3810
        t5908 = t4826 * t4343
        t5911 = (t5903 - t5908) * t94 / 0.2E1
        t5915 = t3505 * t4962
        t5920 = t4000 * t5200
        t5923 = (t5915 - t5920) * t94 / 0.2E1
        t5924 = rx(t5,t1379,t233,0,0)
        t5925 = rx(t5,t1379,t233,1,1)
        t5927 = rx(t5,t1379,t233,2,2)
        t5929 = rx(t5,t1379,t233,1,2)
        t5931 = rx(t5,t1379,t233,2,1)
        t5933 = rx(t5,t1379,t233,1,0)
        t5935 = rx(t5,t1379,t233,0,2)
        t5937 = rx(t5,t1379,t233,0,1)
        t5940 = rx(t5,t1379,t233,2,0)
        t5946 = 0.1E1 / (t5924 * t5925 * t5927 - t5924 * t5929 * t5931 +
     # t5933 * t5931 * t5935 - t5933 * t5937 * t5927 + t5940 * t5937 * t
     #5929 - t5940 * t5925 * t5935)
        t5947 = t4 * t5946
        t5955 = (t1408 - t4247) * t94
        t5957 = (t3388 - t1408) * t94 / 0.2E1 + t5955 / 0.2E1
        t5963 = t5933 ** 2
        t5964 = t5925 ** 2
        t5965 = t5929 ** 2
        t5978 = u(t5,t1379,t1430,n)
        t5982 = (t5978 - t1408) * t236 / 0.2E1 + t1411 / 0.2E1
        t5988 = rx(t5,t185,t1430,0,0)
        t5989 = rx(t5,t185,t1430,1,1)
        t5991 = rx(t5,t185,t1430,2,2)
        t5993 = rx(t5,t185,t1430,1,2)
        t5995 = rx(t5,t185,t1430,2,1)
        t5997 = rx(t5,t185,t1430,1,0)
        t5999 = rx(t5,t185,t1430,0,2)
        t6001 = rx(t5,t185,t1430,0,1)
        t6004 = rx(t5,t185,t1430,2,0)
        t6010 = 0.1E1 / (t5988 * t5989 * t5991 - t5988 * t5993 * t5995 +
     # t5997 * t5995 * t5999 - t5997 * t6001 * t5991 + t6004 * t6001 * t
     #5993 - t6004 * t5989 * t5999)
        t6011 = t4 * t6010
        t6019 = (t1537 - t5196) * t94
        t6021 = (t4561 - t1537) * t94 / 0.2E1 + t6019 / 0.2E1
        t6034 = t1539 / 0.2E1 + (t1537 - t5978) * t183 / 0.2E1
        t6040 = t6004 ** 2
        t6041 = t5995 ** 2
        t6042 = t5991 ** 2
        t5655 = t5947 * (t5924 * t5933 + t5937 * t5925 + t5935 * t5929)
        t5670 = t5947 * (t5933 * t5940 + t5925 * t5931 + t5929 * t5927)
        t5678 = t6011 * (t5988 * t6004 + t6001 * t5995 + t5999 * t5991)
        t5685 = t6011 * (t5997 * t6004 + t5989 * t5995 + t5993 * t5991)
        t6051 = (t4 * (t3423 * (t5875 + t5876 + t5877) / 0.2E1 + t5884 /
     # 0.2E1) * t3432 - t5897) * t94 + (t4314 * t3488 - t5903) * t94 / 0
     #.2E1 + t5911 + (t3309 * t4565 - t5915) * t94 / 0.2E1 + t5923 + t49
     #26 + (t4923 - t5655 * t5957) * t183 / 0.2E1 + (t4949 - t4 * (t4945
     # / 0.2E1 + t5946 * (t5963 + t5964 + t5965) / 0.2E1) * t1937) * t18
     #3 + t4967 + (t4964 - t5670 * t5982) * t183 / 0.2E1 + (t5678 * t602
     #1 - t3762) * t236 / 0.2E1 + t3767 + (t5685 * t6034 - t3812) * t236
     # / 0.2E1 + t3817 + (t4 * (t6010 * (t6040 + t6041 + t6042) / 0.2E1 
     #+ t3833 / 0.2E1) * t1997 - t3842) * t236
        t6052 = t6051 * t3750
        t6055 = t3442 ** 2
        t6056 = t3455 ** 2
        t6057 = t3453 ** 2
        t6060 = t3768 ** 2
        t6061 = t3781 ** 2
        t6062 = t3779 ** 2
        t6064 = t3790 * (t6060 + t6061 + t6062)
        t6069 = t4299 ** 2
        t6070 = t4312 ** 2
        t6071 = t4310 ** 2
        t6073 = t4321 * (t6069 + t6070 + t6071)
        t6076 = t4 * (t6064 / 0.2E1 + t6073 / 0.2E1)
        t6077 = t6076 * t3797
        t6083 = t4737 * t3823
        t6088 = t4965 * t4358
        t6091 = (t6083 - t6088) * t94 / 0.2E1
        t6095 = t3537 * t5056
        t6100 = t4037 * t5398
        t6103 = (t6095 - t6100) * t94 / 0.2E1
        t6104 = rx(t5,t1379,t238,0,0)
        t6105 = rx(t5,t1379,t238,1,1)
        t6107 = rx(t5,t1379,t238,2,2)
        t6109 = rx(t5,t1379,t238,1,2)
        t6111 = rx(t5,t1379,t238,2,1)
        t6113 = rx(t5,t1379,t238,1,0)
        t6115 = rx(t5,t1379,t238,0,2)
        t6117 = rx(t5,t1379,t238,0,1)
        t6120 = rx(t5,t1379,t238,2,0)
        t6126 = 0.1E1 / (t6104 * t6105 * t6107 - t6104 * t6109 * t6111 +
     # t6113 * t6111 * t6115 - t6113 * t6117 * t6107 + t6120 * t6117 * t
     #6109 - t6120 * t6105 * t6115)
        t6127 = t4 * t6126
        t6135 = (t1412 - t4250) * t94
        t6137 = (t3391 - t1412) * t94 / 0.2E1 + t6135 / 0.2E1
        t6143 = t6113 ** 2
        t6144 = t6105 ** 2
        t6145 = t6109 ** 2
        t6158 = u(t5,t1379,t1441,n)
        t6162 = t1414 / 0.2E1 + (t1412 - t6158) * t236 / 0.2E1
        t6168 = rx(t5,t185,t1441,0,0)
        t6169 = rx(t5,t185,t1441,1,1)
        t6171 = rx(t5,t185,t1441,2,2)
        t6173 = rx(t5,t185,t1441,1,2)
        t6175 = rx(t5,t185,t1441,2,1)
        t6177 = rx(t5,t185,t1441,1,0)
        t6179 = rx(t5,t185,t1441,0,2)
        t6181 = rx(t5,t185,t1441,0,1)
        t6184 = rx(t5,t185,t1441,2,0)
        t6190 = 0.1E1 / (t6168 * t6169 * t6171 - t6168 * t6173 * t6175 +
     # t6177 * t6175 * t6179 - t6177 * t6181 * t6171 + t6184 * t6181 * t
     #6173 - t6184 * t6169 * t6179)
        t6191 = t4 * t6190
        t6199 = (t1560 - t5394) * t94
        t6201 = (t4799 - t1560) * t94 / 0.2E1 + t6199 / 0.2E1
        t6214 = t1562 / 0.2E1 + (t1560 - t6158) * t183 / 0.2E1
        t6220 = t6184 ** 2
        t6221 = t6175 ** 2
        t6222 = t6171 ** 2
        t5824 = t6127 * (t6104 * t6113 + t6117 * t6105 + t6115 * t6109)
        t5840 = t6127 * (t6113 * t6120 + t6105 * t6111 + t6109 * t6107)
        t5847 = t6191 * (t6168 * t6184 + t6181 * t6175 + t6179 * t6171)
        t5855 = t6191 * (t6177 * t6184 + t6169 * t6175 + t6173 * t6171)
        t6231 = (t4 * (t3464 * (t6055 + t6056 + t6057) / 0.2E1 + t6064 /
     # 0.2E1) * t3473 - t6077) * t94 + (t4577 * t3503 - t6083) * t94 / 0
     #.2E1 + t6091 + (t3315 * t4803 - t6095) * t94 / 0.2E1 + t6103 + t50
     #20 + (t5017 - t5824 * t6137) * t183 / 0.2E1 + (t5043 - t4 * (t5039
     # / 0.2E1 + t6126 * (t6143 + t6144 + t6145) / 0.2E1) * t1963) * t18
     #3 + t5061 + (t5058 - t5840 * t6162) * t183 / 0.2E1 + t3804 + (t380
     #1 - t5847 * t6201) * t236 / 0.2E1 + t3828 + (t3825 - t5855 * t6214
     #) * t236 / 0.2E1 + (t3851 - t4 * (t3847 / 0.2E1 + t6190 * (t6220 +
     # t6221 + t6222) / 0.2E1) * t2002) * t236
        t6232 = t6231 * t3789
        t6236 = (t6052 - t3855) * t236 / 0.2E1 + (t3855 - t6232) * t236 
     #/ 0.2E1
        t6245 = (t4971 - t5270) * t94
        t6251 = t248 * t2907
        t6258 = (t5065 - t5468) * t94
        t6271 = (t5683 - t4971) * t183 / 0.2E1 + (t4971 - t6052) * t183 
     #/ 0.2E1
        t6275 = t397 * t3859
        t6284 = (t5863 - t5065) * t183 / 0.2E1 + (t5065 - t6232) * t183 
     #/ 0.2E1
        t6294 = (t164 * t2904 - t2919) * t94 + (t178 * ((t3227 - t2902) 
     #* t183 / 0.2E1 + (t2902 - t3535) * t183 / 0.2E1) - t3861) * t94 / 
     #0.2E1 + t4399 + (t231 * ((t4635 - t2902) * t236 / 0.2E1 + (t2902 -
     # t4873) * t236 / 0.2E1) - t5071) * t94 / 0.2E1 + t5477 + (t306 * (
     #(t3227 - t3697) * t94 / 0.2E1 + t5481 / 0.2E1) - t5487) * t183 / 0
     #.2E1 + (t5487 - t348 * ((t3535 - t3855) * t94 / 0.2E1 + t5494 / 0.
     #2E1)) * t183 / 0.2E1 + (t374 * t3699 - t383 * t3857) * t183 + (t38
     #8 * t5867 - t5871) * t183 / 0.2E1 + (t5871 - t411 * t6236) * t183 
     #/ 0.2E1 + (t452 * ((t4635 - t4971) * t94 / 0.2E1 + t6245 / 0.2E1) 
     #- t6251) * t236 / 0.2E1 + (t6251 - t492 * ((t4873 - t5065) * t94 /
     # 0.2E1 + t6258 / 0.2E1)) * t236 / 0.2E1 + (t507 * t6271 - t6275) *
     # t236 / 0.2E1 + (t6275 - t521 * t6284) * t236 / 0.2E1 + (t551 * t4
     #973 - t560 * t5067) * t236
        t6295 = t6294 * t27
        t6305 = t2063 / 0.2E1 + t142 / 0.2E1
        t6307 = t178 * t6305
        t6322 = ut(t64,t180,t233,n)
        t6325 = ut(t64,t180,t238,n)
        t6329 = (t6322 - t900) * t236 / 0.2E1 + (t900 - t6325) * t236 / 
     #0.2E1
        t6333 = t2572 * t943
        t6337 = ut(t64,t185,t233,n)
        t6340 = ut(t64,t185,t238,n)
        t6344 = (t6337 - t903) * t236 / 0.2E1 + (t903 - t6340) * t236 / 
     #0.2E1
        t6355 = t231 * t6305
        t6371 = (t6322 - t936) * t183 / 0.2E1 + (t936 - t6337) * t183 / 
     #0.2E1
        t6375 = t2572 * t907
        t6384 = (t6325 - t939) * t183 / 0.2E1 + (t939 - t6340) * t183 / 
     #0.2E1
        t6394 = t2307 + t2144 / 0.2E1 + t922 + t2325 / 0.2E1 + t958 + (t
     #2524 * (t2515 / 0.2E1 + t973 / 0.2E1) - t6307) * t183 / 0.2E1 + (t
     #6307 - t2547 * (t2532 / 0.2E1 + t988 / 0.2E1)) * t183 / 0.2E1 + (t
     #2719 * t902 - t2728 * t905) * t183 + (t2567 * t6329 - t6333) * t18
     #3 / 0.2E1 + (t6333 - t2583 * t6344) * t183 / 0.2E1 + (t2608 * (t21
     #61 / 0.2E1 + t1031 / 0.2E1) - t6355) * t236 / 0.2E1 + (t6355 - t26
     #30 * (t2186 / 0.2E1 + t1044 / 0.2E1)) * t236 / 0.2E1 + (t2647 * t6
     #371 - t6375) * t236 / 0.2E1 + (t6375 - t2661 * t6384) * t236 / 0.2
     #E1 + (t2888 * t938 - t2897 * t941) * t236
        t6398 = t1217 * t94
        t6401 = dx * ((t6394 * t86 - t1083) * t94 / 0.2E1 + t6398 / 0.2E
     #1)
        t6405 = dx * (t2904 - t2905)
        t6409 = (t2036 - t218) * t183
        t6411 = (t218 - t221) * t183
        t6412 = t6409 - t6411
        t6413 = t6412 * t183
        t6414 = t700 * t6413
        t6416 = (t221 - t2041) * t183
        t6417 = t6411 - t6416
        t6418 = t6417 * t183
        t6419 = t709 * t6418
        t6422 = t3978 - t712
        t6423 = t6422 * t183
        t6424 = t712 - t4242
        t6425 = t6424 * t183
        t6433 = (t4077 / 0.2E1 - t836 / 0.2E1) * t183
        t6436 = (t834 / 0.2E1 - t4341 / 0.2E1) * t183
        t6148 = (t6433 - t6436) * t183
        t6440 = t822 * t6148
        t6443 = t715 * t1795
        t6445 = (t6440 - t6443) * t236
        t6448 = (t4092 / 0.2E1 - t853 / 0.2E1) * t183
        t6451 = (t851 / 0.2E1 - t4356 / 0.2E1) * t183
        t6154 = (t6448 - t6451) * t183
        t6455 = t837 * t6154
        t6457 = (t6443 - t6455) * t236
        t6463 = (t5256 - t844) * t236
        t6465 = (t844 - t859) * t236
        t6467 = (t6463 - t6465) * t236
        t6469 = (t859 - t5454) * t236
        t6471 = (t6465 - t6469) * t236
        t6476 = t865 / 0.2E1
        t6477 = t870 / 0.2E1
        t6481 = (t870 - t879) * t236
        t6486 = t6476 + t6477 - dz * ((t5262 - t865) * t236 / 0.2E1 - t6
     #481 / 0.2E1) / 0.8E1
        t6487 = t4 * t6486
        t6488 = t6487 * t269
        t6489 = t879 / 0.2E1
        t6491 = (t865 - t870) * t236
        t6498 = t6477 + t6489 - dz * (t6491 / 0.2E1 - (t879 - t5460) * t
     #236 / 0.2E1) / 0.8E1
        t6499 = t4 * t6498
        t6500 = t6499 * t272
        t6504 = (t1712 - t269) * t236
        t6506 = (t269 - t272) * t236
        t6507 = t6504 - t6506
        t6508 = t6507 * t236
        t6509 = t873 * t6508
        t6511 = (t272 - t1718) * t236
        t6512 = t6506 - t6511
        t6513 = t6512 * t236
        t6514 = t882 * t6513
        t6517 = t5268 - t885
        t6518 = t6517 * t236
        t6519 = t885 - t5466
        t6520 = t6519 * t236
        t6528 = (t3906 / 0.2E1 - t585 / 0.2E1) * t183
        t6531 = (t582 / 0.2E1 - t4170 / 0.2E1) * t183
        t6197 = (t6528 - t6531) * t183
        t6535 = t573 * t6197
        t6537 = (t2048 - t6535) * t94
        t6542 = i - 2
        t6543 = u(t6542,j,k,n)
        t6545 = (t569 - t6543) * t94
        t6547 = (t571 - t6545) * t94
        t6548 = t1612 - t6547
        t6549 = t6548 * t94
        t6550 = t568 * t6549
        t6553 = rx(t6542,j,k,0,0)
        t6554 = rx(t6542,j,k,1,1)
        t6556 = rx(t6542,j,k,2,2)
        t6558 = rx(t6542,j,k,1,2)
        t6560 = rx(t6542,j,k,2,1)
        t6562 = rx(t6542,j,k,1,0)
        t6564 = rx(t6542,j,k,0,2)
        t6566 = rx(t6542,j,k,0,1)
        t6569 = rx(t6542,j,k,2,0)
        t6574 = t6553 * t6554 * t6556 - t6553 * t6558 * t6560 + t6562 * 
     #t6560 * t6564 - t6562 * t6566 * t6556 + t6569 * t6566 * t6558 - t6
     #569 * t6554 * t6564
        t6575 = 0.1E1 / t6574
        t6576 = t6553 ** 2
        t6577 = t6566 ** 2
        t6578 = t6564 ** 2
        t6580 = t6575 * (t6576 + t6577 + t6578)
        t6583 = t4 * (t124 / 0.2E1 + t6580 / 0.2E1)
        t6584 = t6583 * t6545
        t6586 = (t572 - t6584) * t94
        t6587 = t574 - t6586
        t6588 = t6587 * t94
        t6595 = (t5241 - t790) * t236
        t6597 = (t790 - t827) * t236
        t6599 = (t6595 - t6597) * t236
        t6601 = (t827 - t5439) * t236
        t6603 = (t6597 - t6601) * t236
        t6608 = t124 / 0.2E1
        t6615 = t63 + t6608 - dx * (t1643 / 0.2E1 - (t124 - t6580) * t94
     # / 0.2E1) / 0.8E1
        t6616 = t4 * t6615
        t6617 = t6616 * t571
        t6620 = t4 * t6575
        t6624 = t6553 * t6569 + t6560 * t6566 + t6564 * t6556
        t6625 = u(t6542,j,t233,n)
        t6627 = (t6625 - t6543) * t236
        t6628 = u(t6542,j,t238,n)
        t6630 = (t6543 - t6628) * t236
        t6632 = t6627 / 0.2E1 + t6630 / 0.2E1
        t6244 = t6620 * t6624
        t6634 = t6244 * t6632
        t6636 = (t606 - t6634) * t94
        t6638 = (t608 - t6636) * t94
        t6640 = (t1323 - t6638) * t94
        t6645 = t279 + t228 + t609 + t791 + t828 - t1330 * ((t6414 - t64
     #19) * t183 + (t6423 - t6425) * t183) / 0.24E2 - t1330 * (t6445 / 0
     #.2E1 + t6457 / 0.2E1) / 0.6E1 - t1429 * (t6467 / 0.2E1 + t6471 / 0
     #.2E1) / 0.6E1 + (t6488 - t6500) * t236 - t1429 * ((t6509 - t6514) 
     #* t236 + (t6518 - t6520) * t236) / 0.24E2 - t1330 * (t2050 / 0.2E1
     # + t6537 / 0.2E1) / 0.6E1 - t1227 * ((t1615 - t6550) * t94 + (t163
     #2 - t6588) * t94) / 0.24E2 - t1429 * (t6599 / 0.2E1 + t6603 / 0.2E
     #1) / 0.6E1 + (t1651 - t6617) * t94 - t1227 * (t1325 / 0.2E1 + t664
     #0 / 0.2E1) / 0.6E1
        t6646 = u(t6542,t180,k,n)
        t6648 = (t580 - t6646) * t94
        t6651 = (t311 / 0.2E1 - t6648 / 0.2E1) * t94
        t6304 = (t1237 - t6651) * t94
        t6655 = t629 * t6304
        t6658 = (t171 / 0.2E1 - t6545 / 0.2E1) * t94
        t6309 = (t1250 - t6658) * t94
        t6662 = t214 * t6309
        t6664 = (t6655 - t6662) * t183
        t6665 = u(t6542,t185,k,n)
        t6667 = (t583 - t6665) * t94
        t6670 = (t354 / 0.2E1 - t6667 / 0.2E1) * t94
        t6314 = (t1265 - t6670) * t94
        t6674 = t669 * t6314
        t6676 = (t6662 - t6674) * t183
        t6683 = (t5186 / 0.2E1 - t722 / 0.2E1) * t236
        t6686 = (t719 / 0.2E1 - t5384 / 0.2E1) * t236
        t6320 = (t6683 - t6686) * t236
        t6690 = t708 * t6320
        t6693 = t715 * t1609
        t6695 = (t6690 - t6693) * t183
        t6698 = (t5198 / 0.2E1 - t745 / 0.2E1) * t236
        t6701 = (t742 / 0.2E1 - t5396 / 0.2E1) * t236
        t6328 = (t6698 - t6701) * t236
        t6705 = t731 * t6328
        t6707 = (t6693 - t6705) * t183
        t6715 = t6553 * t6562 + t6566 * t6554 + t6564 * t6558
        t6717 = (t6646 - t6543) * t183
        t6719 = (t6543 - t6665) * t183
        t6721 = t6717 / 0.2E1 + t6719 / 0.2E1
        t6339 = t6620 * t6715
        t6723 = t6339 * t6721
        t6725 = (t589 - t6723) * t94
        t6727 = (t591 - t6725) * t94
        t6729 = (t1675 - t6727) * t94
        t6736 = (t5131 / 0.2E1 - t602 / 0.2E1) * t236
        t6739 = (t599 / 0.2E1 - t5329 / 0.2E1) * t236
        t6349 = (t6736 - t6739) * t236
        t6743 = t590 * t6349
        t6745 = (t1725 - t6743) * t94
        t6751 = (t3966 - t649) * t183
        t6753 = (t649 - t686) * t183
        t6755 = (t6751 - t6753) * t183
        t6757 = (t686 - t4230) * t183
        t6759 = (t6753 - t6757) * t183
        t6764 = t692 / 0.2E1
        t6765 = t697 / 0.2E1
        t6769 = (t697 - t706) * t183
        t6774 = t6764 + t6765 - dy * ((t3972 - t692) * t183 / 0.2E1 - t6
     #769 / 0.2E1) / 0.8E1
        t6775 = t4 * t6774
        t6776 = t6775 * t218
        t6777 = t706 / 0.2E1
        t6779 = (t692 - t697) * t183
        t6786 = t6765 + t6777 - dy * (t6779 / 0.2E1 - (t706 - t4236) * t
     #183 / 0.2E1) / 0.8E1
        t6787 = t4 * t6786
        t6788 = t6787 * t221
        t6792 = (t597 - t6625) * t94
        t6795 = (t458 / 0.2E1 - t6792 / 0.2E1) * t94
        t6376 = (t1903 - t6795) * t94
        t6799 = t771 * t6376
        t6802 = t265 * t6309
        t6804 = (t6799 - t6802) * t236
        t6806 = (t600 - t6628) * t94
        t6809 = (t499 / 0.2E1 - t6806 / 0.2E1) * t94
        t6381 = (t1920 - t6809) * t94
        t6813 = t809 * t6381
        t6815 = (t6802 - t6813) * t236
        t6821 = (t3994 - t734) * t183
        t6823 = (t734 - t751) * t183
        t6825 = (t6821 - t6823) * t183
        t6827 = (t751 - t4258) * t183
        t6829 = (t6823 - t6827) * t183
        t6834 = -t1227 * (t6664 / 0.2E1 + t6676 / 0.2E1) / 0.6E1 + t687 
     #- t1429 * (t6695 / 0.2E1 + t6707 / 0.2E1) / 0.6E1 - t1227 * (t1677
     # / 0.2E1 + t6729 / 0.2E1) / 0.6E1 + t592 - t1429 * (t1727 / 0.2E1 
     #+ t6745 / 0.2E1) / 0.6E1 + t650 - t1330 * (t6755 / 0.2E1 + t6759 /
     # 0.2E1) / 0.6E1 + (t6776 - t6788) * t183 - t1227 * (t6804 / 0.2E1 
     #+ t6815 / 0.2E1) / 0.6E1 + t845 + t860 - t1330 * (t6825 / 0.2E1 + 
     #t6829 / 0.2E1) / 0.6E1 + t735 + t752
        t6836 = (t6645 + t6834) * t56
        t6837 = t136 * t6836
        t6838 = t147 / 0.2E1
        t6839 = ut(t6542,j,k,n)
        t6841 = (t145 - t6839) * t94
        t6843 = (t147 - t6841) * t94
        t6844 = t149 - t6843
        t6845 = t6844 * t94
        t6848 = t1227 * (t2068 / 0.2E1 + t6845 / 0.2E1)
        t6849 = t6848 / 0.6E1
        t6852 = dx * (t2060 + t6838 - t6849) / 0.2E1
        t6853 = ut(t96,j,t1430,n)
        t6855 = (t6853 - t1100) * t236
        t6858 = (t6855 / 0.2E1 - t1105 / 0.2E1) * t236
        t6859 = ut(t96,j,t1441,n)
        t6861 = (t1103 - t6859) * t236
        t6864 = (t1102 / 0.2E1 - t6861 / 0.2E1) * t236
        t6460 = (t6858 - t6864) * t236
        t6868 = t590 * t6460
        t6870 = (t2256 - t6868) * t94
        t6875 = ut(t6542,j,t233,n)
        t6877 = (t6875 - t6839) * t236
        t6878 = ut(t6542,j,t238,n)
        t6880 = (t6839 - t6878) * t236
        t6882 = t6877 / 0.2E1 + t6880 / 0.2E1
        t6884 = t6244 * t6882
        t6886 = (t1109 - t6884) * t94
        t6888 = (t1111 - t6886) * t94
        t6890 = (t2333 - t6888) * t94
        t6896 = (t2243 - t961) * t236
        t6898 = (t961 - t964) * t236
        t6899 = t6896 - t6898
        t6900 = t6899 * t236
        t6901 = t873 * t6900
        t6903 = (t964 - t2249) * t236
        t6904 = t6898 - t6903
        t6905 = t6904 * t236
        t6906 = t882 * t6905
        t6909 = t5265 * t2243
        t6911 = (t6909 - t1211) * t236
        t6912 = t6911 - t1214
        t6913 = t6912 * t236
        t6914 = t5463 * t2249
        t6916 = (t1212 - t6914) * t236
        t6917 = t1214 - t6916
        t6918 = t6917 * t236
        t6924 = ut(t96,t1331,k,n)
        t6926 = (t2111 - t6924) * t94
        t6928 = t2591 / 0.2E1 + t6926 / 0.2E1
        t6930 = t3682 * t6928
        t6932 = (t6930 - t1118) * t183
        t6934 = (t6932 - t1124) * t183
        t6936 = (t1124 - t1133) * t183
        t6938 = (t6934 - t6936) * t183
        t6939 = ut(t96,t1379,k,n)
        t6941 = (t2117 - t6939) * t94
        t6943 = t2607 / 0.2E1 + t6941 / 0.2E1
        t6945 = t3935 * t6943
        t6947 = (t1131 - t6945) * t183
        t6949 = (t1133 - t6947) * t183
        t6951 = (t6936 - t6949) * t183
        t6956 = t6775 * t925
        t6957 = t6787 * t928
        t6960 = t935 + t1125 + t1134 + t1153 + t1099 + t1112 - t1429 * (
     #t2258 / 0.2E1 + t6870 / 0.2E1) / 0.6E1 - t1227 * (t2335 / 0.2E1 + 
     #t6890 / 0.2E1) / 0.6E1 - t1429 * ((t6901 - t6906) * t236 + (t6913 
     #- t6918) * t236) / 0.24E2 - t1330 * (t6938 / 0.2E1 + t6951 / 0.2E1
     #) / 0.6E1 + (t6956 - t6957) * t183 + t1166 + t1177 + t1186 + t1199
        t6962 = (t2113 - t925) * t183
        t6964 = (t925 - t928) * t183
        t6965 = t6962 - t6964
        t6966 = t6965 * t183
        t6967 = t700 * t6966
        t6969 = (t928 - t2119) * t183
        t6970 = t6964 - t6969
        t6971 = t6970 * t183
        t6972 = t709 * t6971
        t6975 = t3975 * t2113
        t6977 = (t6975 - t1135) * t183
        t6978 = t6977 - t1138
        t6979 = t6978 * t183
        t6980 = t4239 * t2119
        t6982 = (t1136 - t6980) * t183
        t6983 = t1138 - t6982
        t6984 = t6983 * t183
        t6990 = ut(i,t1331,t233,n)
        t6992 = (t6990 - t2111) * t236
        t6993 = ut(i,t1331,t238,n)
        t6995 = (t2111 - t6993) * t236
        t6997 = t6992 / 0.2E1 + t6995 / 0.2E1
        t6999 = t3713 * t6997
        t7001 = (t6999 - t1148) * t183
        t7003 = (t7001 - t1152) * t183
        t7005 = (t1152 - t1165) * t183
        t7007 = (t7003 - t7005) * t183
        t7008 = ut(i,t1379,t233,n)
        t7010 = (t7008 - t2117) * t236
        t7011 = ut(i,t1379,t238,n)
        t7013 = (t2117 - t7011) * t236
        t7015 = t7010 / 0.2E1 + t7013 / 0.2E1
        t7017 = t3956 * t7015
        t7019 = (t1163 - t7017) * t183
        t7021 = (t1165 - t7019) * t183
        t7023 = (t7005 - t7021) * t183
        t7028 = t6487 * t961
        t7029 = t6499 * t964
        t7032 = t6616 * t147
        t7035 = t568 * t6845
        t7038 = t6583 * t6841
        t7040 = (t1084 - t7038) * t94
        t7041 = t1086 - t7040
        t7042 = t7041 * t94
        t7048 = ut(t6542,t180,k,n)
        t7050 = (t1087 - t7048) * t94
        t7053 = (t975 / 0.2E1 - t7050 / 0.2E1) * t94
        t6585 = (t2521 - t7053) * t94
        t7057 = t629 * t6585
        t7060 = (t139 / 0.2E1 - t6841 / 0.2E1) * t94
        t6591 = (t2177 - t7060) * t94
        t7064 = t214 * t6591
        t7066 = (t7057 - t7064) * t183
        t7067 = ut(t6542,t185,k,n)
        t7069 = (t1090 - t7067) * t94
        t7072 = (t990 / 0.2E1 - t7069 / 0.2E1) * t94
        t6598 = (t2538 - t7072) * t94
        t7076 = t669 * t6598
        t7078 = (t7064 - t7076) * t183
        t7084 = (t1100 - t6875) * t94
        t7087 = (t1033 / 0.2E1 - t7084 / 0.2E1) * t94
        t6606 = (t2167 - t7087) * t94
        t7091 = t771 * t6606
        t7094 = t265 * t6591
        t7096 = (t7091 - t7094) * t236
        t7098 = (t1103 - t6878) * t94
        t7101 = (t1046 / 0.2E1 - t7098 / 0.2E1) * t94
        t6612 = (t2192 - t7101) * t94
        t7105 = t809 * t6612
        t7107 = (t7094 - t7105) * t236
        t7112 = ut(i,t180,t1430,n)
        t7114 = (t7112 - t2241) * t183
        t7115 = ut(i,t185,t1430,n)
        t7117 = (t2241 - t7115) * t183
        t7119 = t7114 / 0.2E1 + t7117 / 0.2E1
        t7121 = t4876 * t7119
        t7123 = (t7121 - t1194) * t236
        t7125 = (t7123 - t1198) * t236
        t7127 = (t1198 - t1209) * t236
        t7129 = (t7125 - t7127) * t236
        t7130 = ut(i,t180,t1441,n)
        t7132 = (t7130 - t2247) * t183
        t7133 = ut(i,t185,t1441,n)
        t7135 = (t2247 - t7133) * t183
        t7137 = t7132 / 0.2E1 + t7135 / 0.2E1
        t7139 = t5053 * t7137
        t7141 = (t1207 - t7139) * t236
        t7143 = (t1209 - t7141) * t236
        t7145 = (t7127 - t7143) * t236
        t7151 = (t6924 - t1087) * t183
        t7154 = (t7151 / 0.2E1 - t1092 / 0.2E1) * t183
        t7156 = (t1090 - t6939) * t183
        t7159 = (t1089 / 0.2E1 - t7156 / 0.2E1) * t183
        t6650 = (t7154 - t7159) * t183
        t7163 = t573 * t6650
        t7165 = (t2126 - t7163) * t94
        t7171 = (t2241 - t6853) * t94
        t7173 = t2454 / 0.2E1 + t7171 / 0.2E1
        t7175 = t4866 * t7173
        t7177 = (t7175 - t1172) * t236
        t7179 = (t7177 - t1176) * t236
        t7181 = (t1176 - t1185) * t236
        t7183 = (t7179 - t7181) * t236
        t7185 = (t2247 - t6859) * t94
        t7187 = t2470 / 0.2E1 + t7185 / 0.2E1
        t7189 = t5040 * t7187
        t7191 = (t1183 - t7189) * t236
        t7193 = (t1185 - t7191) * t236
        t7195 = (t7181 - t7193) * t236
        t7201 = (t6990 - t1139) * t183
        t7204 = (t7201 / 0.2E1 - t1190 / 0.2E1) * t183
        t7206 = (t1154 - t7008) * t183
        t7209 = (t1188 / 0.2E1 - t7206 / 0.2E1) * t183
        t6682 = (t7204 - t7209) * t183
        t7213 = t822 * t6682
        t7216 = t715 * t2000
        t7218 = (t7213 - t7216) * t236
        t7220 = (t6993 - t1142) * t183
        t7223 = (t7220 / 0.2E1 - t1203 / 0.2E1) * t183
        t7225 = (t1157 - t7011) * t183
        t7228 = (t1201 / 0.2E1 - t7225 / 0.2E1) * t183
        t6696 = (t7223 - t7228) * t183
        t7232 = t837 * t6696
        t7234 = (t7216 - t7232) * t236
        t7240 = (t7048 - t6839) * t183
        t7242 = (t6839 - t7067) * t183
        t7244 = t7240 / 0.2E1 + t7242 / 0.2E1
        t7246 = t6339 * t7244
        t7248 = (t1096 - t7246) * t94
        t7250 = (t1098 - t7248) * t94
        t7252 = (t2152 - t7250) * t94
        t7258 = (t7112 - t1139) * t236
        t7261 = (t7258 / 0.2E1 - t1144 / 0.2E1) * t236
        t7263 = (t1142 - t7130) * t236
        t7266 = (t1141 / 0.2E1 - t7263 / 0.2E1) * t236
        t6716 = (t7261 - t7266) * t236
        t7270 = t708 * t6716
        t7273 = t715 * t2052
        t7275 = (t7270 - t7273) * t183
        t7277 = (t7115 - t1154) * t236
        t7280 = (t7277 / 0.2E1 - t1159 / 0.2E1) * t236
        t7282 = (t1157 - t7133) * t236
        t7285 = (t1156 / 0.2E1 - t7282 / 0.2E1) * t236
        t6731 = (t7280 - t7285) * t236
        t7289 = t731 * t6731
        t7291 = (t7273 - t7289) * t183
        t7296 = -t1330 * ((t6967 - t6972) * t183 + (t6979 - t6984) * t18
     #3) / 0.24E2 - t1330 * (t7007 / 0.2E1 + t7023 / 0.2E1) / 0.6E1 + (t
     #7028 - t7029) * t236 + (t2204 - t7032) * t94 + t1210 - t1227 * ((t
     #2302 - t7035) * t94 + (t2310 - t7042) * t94) / 0.24E2 - t1227 * (t
     #7066 / 0.2E1 + t7078 / 0.2E1) / 0.6E1 - t1227 * (t7096 / 0.2E1 + t
     #7107 / 0.2E1) / 0.6E1 - t1429 * (t7129 / 0.2E1 + t7145 / 0.2E1) / 
     #0.6E1 - t1330 * (t2128 / 0.2E1 + t7165 / 0.2E1) / 0.6E1 - t1429 * 
     #(t7183 / 0.2E1 + t7195 / 0.2E1) / 0.6E1 + t971 - t1330 * (t7218 / 
     #0.2E1 + t7234 / 0.2E1) / 0.6E1 - t1227 * (t2154 / 0.2E1 + t7252 / 
     #0.2E1) / 0.6E1 - t1429 * (t7275 / 0.2E1 + t7291 / 0.2E1) / 0.6E1
        t7298 = (t6960 + t7296) * t56
        t7300 = t2076 * t7298 / 0.2E1
        t7301 = t6725 / 0.2E1
        t7302 = t6636 / 0.2E1
        t7304 = t639 / 0.2E1 + t6648 / 0.2E1
        t7306 = t3630 * t7304
        t7308 = t571 / 0.2E1 + t6545 / 0.2E1
        t7310 = t573 * t7308
        t7312 = (t7306 - t7310) * t183
        t7313 = t7312 / 0.2E1
        t7315 = t680 / 0.2E1 + t6667 / 0.2E1
        t7317 = t3877 * t7315
        t7319 = (t7310 - t7317) * t183
        t7320 = t7319 / 0.2E1
        t7321 = t3874 ** 2
        t7322 = t3866 ** 2
        t7323 = t3870 ** 2
        t7325 = t3887 * (t7321 + t7322 + t7323)
        t7326 = t106 ** 2
        t7327 = t98 ** 2
        t7328 = t102 ** 2
        t7330 = t119 * (t7326 + t7327 + t7328)
        t7333 = t4 * (t7325 / 0.2E1 + t7330 / 0.2E1)
        t7334 = t7333 * t582
        t7335 = t4138 ** 2
        t7336 = t4130 ** 2
        t7337 = t4134 ** 2
        t7339 = t4151 * (t7335 + t7336 + t7337)
        t7342 = t4 * (t7330 / 0.2E1 + t7339 / 0.2E1)
        t7343 = t7342 * t585
        t7345 = (t7334 - t7343) * t183
        t6856 = t3899 * (t3874 * t3881 + t3866 * t3872 + t3870 * t3868)
        t7351 = t6856 * t3925
        t6863 = t575 * (t106 * t113 + t98 * t104 + t102 * t100)
        t7357 = t6863 * t604
        t7359 = (t7351 - t7357) * t183
        t7360 = t7359 / 0.2E1
        t6872 = t4163 * (t4138 * t4145 + t4130 * t4136 + t4134 * t4132)
        t7366 = t6872 * t4189
        t7368 = (t7357 - t7366) * t183
        t7369 = t7368 / 0.2E1
        t7371 = t782 / 0.2E1 + t6792 / 0.2E1
        t7373 = t4807 * t7371
        t7375 = t590 * t7308
        t7377 = (t7373 - t7375) * t236
        t7378 = t7377 / 0.2E1
        t7380 = t821 / 0.2E1 + t6806 / 0.2E1
        t7382 = t4947 * t7380
        t7384 = (t7375 - t7382) * t236
        t7385 = t7384 / 0.2E1
        t6889 = t5109 * (t5091 * t5084 + t5076 * t5082 + t5080 * t5078)
        t7391 = t6889 * t5119
        t7393 = t6863 * t587
        t7395 = (t7391 - t7393) * t236
        t7396 = t7395 / 0.2E1
        t6895 = t5307 * (t5282 * t5289 + t5274 * t5280 + t5278 * t5276)
        t7402 = t6895 * t5317
        t7404 = (t7393 - t7402) * t236
        t7405 = t7404 / 0.2E1
        t7406 = t5091 ** 2
        t7407 = t5082 ** 2
        t7408 = t5078 ** 2
        t7410 = t5097 * (t7406 + t7407 + t7408)
        t7411 = t113 ** 2
        t7412 = t104 ** 2
        t7413 = t100 ** 2
        t7415 = t119 * (t7411 + t7412 + t7413)
        t7418 = t4 * (t7410 / 0.2E1 + t7415 / 0.2E1)
        t7419 = t7418 * t599
        t7420 = t5289 ** 2
        t7421 = t5280 ** 2
        t7422 = t5276 ** 2
        t7424 = t5295 * (t7420 + t7421 + t7422)
        t7427 = t4 * (t7415 / 0.2E1 + t7424 / 0.2E1)
        t7428 = t7427 * t602
        t7430 = (t7419 - t7428) * t236
        t7431 = t6586 + t592 + t7301 + t609 + t7302 + t7313 + t7320 + t7
     #345 + t7360 + t7369 + t7378 + t7385 + t7396 + t7405 + t7430
        t7432 = t7431 * t118
        t7433 = t887 - t7432
        t7434 = t7433 * t94
        t7436 = t2905 / 0.2E1 + t7434 / 0.2E1
        t7437 = dx * t7436
        t7439 = t136 * t7437 / 0.2E1
        t7445 = t1227 * (t149 - dx * (t2068 - t6845) / 0.12E2) / 0.12E2
        t7446 = t568 * t7434
        t7449 = rx(t6542,t180,k,0,0)
        t7450 = rx(t6542,t180,k,1,1)
        t7452 = rx(t6542,t180,k,2,2)
        t7454 = rx(t6542,t180,k,1,2)
        t7456 = rx(t6542,t180,k,2,1)
        t7458 = rx(t6542,t180,k,1,0)
        t7460 = rx(t6542,t180,k,0,2)
        t7462 = rx(t6542,t180,k,0,1)
        t7465 = rx(t6542,t180,k,2,0)
        t7470 = t7449 * t7450 * t7452 - t7449 * t7454 * t7456 + t7458 * 
     #t7456 * t7460 - t7458 * t7462 * t7452 + t7465 * t7462 * t7454 - t7
     #465 * t7450 * t7460
        t7471 = 0.1E1 / t7470
        t7472 = t7449 ** 2
        t7473 = t7462 ** 2
        t7474 = t7460 ** 2
        t7476 = t7471 * (t7472 + t7473 + t7474)
        t7479 = t4 * (t3892 / 0.2E1 + t7476 / 0.2E1)
        t7480 = t7479 * t6648
        t7482 = (t3896 - t7480) * t94
        t7483 = t4 * t7471
        t7488 = u(t6542,t1331,k,n)
        t7490 = (t7488 - t6646) * t183
        t7492 = t7490 / 0.2E1 + t6717 / 0.2E1
        t6985 = t7483 * (t7449 * t7458 + t7462 * t7450 + t7460 * t7454)
        t7494 = t6985 * t7492
        t7496 = (t3910 - t7494) * t94
        t7497 = t7496 / 0.2E1
        t7502 = u(t6542,t180,t233,n)
        t7504 = (t7502 - t6646) * t236
        t7505 = u(t6542,t180,t238,n)
        t7507 = (t6646 - t7505) * t236
        t7509 = t7504 / 0.2E1 + t7507 / 0.2E1
        t6998 = t7483 * (t7449 * t7465 + t7462 * t7456 + t7460 * t7452)
        t7511 = t6998 * t7509
        t7513 = (t3927 - t7511) * t94
        t7514 = t7513 / 0.2E1
        t7515 = rx(t96,t1331,k,0,0)
        t7516 = rx(t96,t1331,k,1,1)
        t7518 = rx(t96,t1331,k,2,2)
        t7520 = rx(t96,t1331,k,1,2)
        t7522 = rx(t96,t1331,k,2,1)
        t7524 = rx(t96,t1331,k,1,0)
        t7526 = rx(t96,t1331,k,0,2)
        t7528 = rx(t96,t1331,k,0,1)
        t7531 = rx(t96,t1331,k,2,0)
        t7536 = t7515 * t7516 * t7518 - t7515 * t7520 * t7522 + t7522 * 
     #t7524 * t7526 - t7524 * t7528 * t7518 + t7531 * t7528 * t7520 - t7
     #531 * t7516 * t7526
        t7537 = 0.1E1 / t7536
        t7538 = t4 * t7537
        t7544 = (t3904 - t7488) * t94
        t7546 = t3960 / 0.2E1 + t7544 / 0.2E1
        t7034 = t7538 * (t7515 * t7524 + t7528 * t7516 + t7526 * t7520)
        t7548 = t7034 * t7546
        t7550 = (t7548 - t7306) * t183
        t7551 = t7550 / 0.2E1
        t7552 = t7524 ** 2
        t7553 = t7516 ** 2
        t7554 = t7520 ** 2
        t7556 = t7537 * (t7552 + t7553 + t7554)
        t7559 = t4 * (t7556 / 0.2E1 + t7325 / 0.2E1)
        t7560 = t7559 * t3906
        t7562 = (t7560 - t7334) * t183
        t7567 = u(t96,t1331,t233,n)
        t7569 = (t7567 - t3904) * t236
        t7570 = u(t96,t1331,t238,n)
        t7572 = (t3904 - t7570) * t236
        t7574 = t7569 / 0.2E1 + t7572 / 0.2E1
        t7055 = t7538 * (t7524 * t7531 + t7516 * t7522 + t7520 * t7518)
        t7576 = t7055 * t7574
        t7578 = (t7576 - t7351) * t183
        t7579 = t7578 / 0.2E1
        t7580 = rx(t96,t180,t233,0,0)
        t7581 = rx(t96,t180,t233,1,1)
        t7583 = rx(t96,t180,t233,2,2)
        t7585 = rx(t96,t180,t233,1,2)
        t7587 = rx(t96,t180,t233,2,1)
        t7589 = rx(t96,t180,t233,1,0)
        t7591 = rx(t96,t180,t233,0,2)
        t7593 = rx(t96,t180,t233,0,1)
        t7596 = rx(t96,t180,t233,2,0)
        t7601 = t7580 * t7581 * t7583 - t7580 * t7585 * t7587 + t7589 * 
     #t7587 * t7591 - t7589 * t7593 * t7583 + t7596 * t7593 * t7585 - t7
     #596 * t7581 * t7591
        t7602 = 0.1E1 / t7601
        t7603 = t4 * t7602
        t7609 = (t3918 - t7502) * t94
        t7611 = t4025 / 0.2E1 + t7609 / 0.2E1
        t7086 = t7603 * (t7580 * t7596 + t7593 * t7587 + t7591 * t7583)
        t7613 = t7086 * t7611
        t7615 = t3645 * t7304
        t7618 = (t7613 - t7615) * t236 / 0.2E1
        t7619 = rx(t96,t180,t238,0,0)
        t7620 = rx(t96,t180,t238,1,1)
        t7622 = rx(t96,t180,t238,2,2)
        t7624 = rx(t96,t180,t238,1,2)
        t7626 = rx(t96,t180,t238,2,1)
        t7628 = rx(t96,t180,t238,1,0)
        t7630 = rx(t96,t180,t238,0,2)
        t7632 = rx(t96,t180,t238,0,1)
        t7635 = rx(t96,t180,t238,2,0)
        t7640 = t7619 * t7620 * t7622 - t7619 * t7624 * t7626 + t7628 * 
     #t7626 * t7630 - t7628 * t7632 * t7622 + t7635 * t7632 * t7624 - t7
     #635 * t7620 * t7630
        t7641 = 0.1E1 / t7640
        t7642 = t4 * t7641
        t7648 = (t3921 - t7505) * t94
        t7650 = t4064 / 0.2E1 + t7648 / 0.2E1
        t7122 = t7642 * (t7619 * t7635 + t7632 * t7626 + t7630 * t7622)
        t7652 = t7122 * t7650
        t7655 = (t7615 - t7652) * t236 / 0.2E1
        t7661 = (t7567 - t3918) * t183
        t7663 = t7661 / 0.2E1 + t5115 / 0.2E1
        t7140 = t7603 * (t7589 * t7596 + t7581 * t7587 + t7585 * t7583)
        t7665 = t7140 * t7663
        t7667 = t6856 * t3908
        t7670 = (t7665 - t7667) * t236 / 0.2E1
        t7676 = (t7570 - t3921) * t183
        t7678 = t7676 / 0.2E1 + t5313 / 0.2E1
        t7152 = t7642 * (t7628 * t7635 + t7620 * t7626 + t7624 * t7622)
        t7680 = t7152 * t7678
        t7683 = (t7667 - t7680) * t236 / 0.2E1
        t7684 = t7596 ** 2
        t7685 = t7587 ** 2
        t7686 = t7583 ** 2
        t7688 = t7602 * (t7684 + t7685 + t7686)
        t7689 = t3881 ** 2
        t7690 = t3872 ** 2
        t7691 = t3868 ** 2
        t7693 = t3887 * (t7689 + t7690 + t7691)
        t7696 = t4 * (t7688 / 0.2E1 + t7693 / 0.2E1)
        t7697 = t7696 * t3920
        t7698 = t7635 ** 2
        t7699 = t7626 ** 2
        t7700 = t7622 ** 2
        t7702 = t7641 * (t7698 + t7699 + t7700)
        t7705 = t4 * (t7693 / 0.2E1 + t7702 / 0.2E1)
        t7706 = t7705 * t3923
        t7709 = t7482 + t3913 + t7497 + t3930 + t7514 + t7551 + t7313 + 
     #t7562 + t7579 + t7360 + t7618 + t7655 + t7670 + t7683 + (t7697 - t
     #7706) * t236
        t7710 = t7709 * t3886
        t7712 = (t7710 - t7432) * t183
        t7713 = rx(t6542,t185,k,0,0)
        t7714 = rx(t6542,t185,k,1,1)
        t7716 = rx(t6542,t185,k,2,2)
        t7718 = rx(t6542,t185,k,1,2)
        t7720 = rx(t6542,t185,k,2,1)
        t7722 = rx(t6542,t185,k,1,0)
        t7724 = rx(t6542,t185,k,0,2)
        t7726 = rx(t6542,t185,k,0,1)
        t7729 = rx(t6542,t185,k,2,0)
        t7734 = t7713 * t7714 * t7716 - t7713 * t7718 * t7720 + t7722 * 
     #t7720 * t7724 - t7722 * t7726 * t7716 + t7729 * t7726 * t7718 - t7
     #729 * t7714 * t7724
        t7735 = 0.1E1 / t7734
        t7736 = t7713 ** 2
        t7737 = t7726 ** 2
        t7738 = t7724 ** 2
        t7740 = t7735 * (t7736 + t7737 + t7738)
        t7743 = t4 * (t4156 / 0.2E1 + t7740 / 0.2E1)
        t7744 = t7743 * t6667
        t7746 = (t4160 - t7744) * t94
        t7747 = t4 * t7735
        t7752 = u(t6542,t1379,k,n)
        t7754 = (t6665 - t7752) * t183
        t7756 = t6719 / 0.2E1 + t7754 / 0.2E1
        t7208 = t7747 * (t7713 * t7722 + t7726 * t7714 + t7724 * t7718)
        t7758 = t7208 * t7756
        t7760 = (t4174 - t7758) * t94
        t7761 = t7760 / 0.2E1
        t7766 = u(t6542,t185,t233,n)
        t7768 = (t7766 - t6665) * t236
        t7769 = u(t6542,t185,t238,n)
        t7771 = (t6665 - t7769) * t236
        t7773 = t7768 / 0.2E1 + t7771 / 0.2E1
        t7222 = t7747 * (t7713 * t7729 + t7720 * t7726 + t7724 * t7716)
        t7775 = t7222 * t7773
        t7777 = (t4191 - t7775) * t94
        t7778 = t7777 / 0.2E1
        t7779 = rx(t96,t1379,k,0,0)
        t7780 = rx(t96,t1379,k,1,1)
        t7782 = rx(t96,t1379,k,2,2)
        t7784 = rx(t96,t1379,k,1,2)
        t7786 = rx(t96,t1379,k,2,1)
        t7788 = rx(t96,t1379,k,1,0)
        t7790 = rx(t96,t1379,k,0,2)
        t7792 = rx(t96,t1379,k,0,1)
        t7795 = rx(t96,t1379,k,2,0)
        t7800 = t7779 * t7780 * t7782 - t7779 * t7784 * t7786 + t7788 * 
     #t7786 * t7790 - t7788 * t7792 * t7782 + t7795 * t7792 * t7784 - t7
     #795 * t7780 * t7790
        t7801 = 0.1E1 / t7800
        t7802 = t4 * t7801
        t7808 = (t4168 - t7752) * t94
        t7810 = t4224 / 0.2E1 + t7808 / 0.2E1
        t7254 = t7802 * (t7779 * t7788 + t7792 * t7780 + t7790 * t7784)
        t7812 = t7254 * t7810
        t7814 = (t7317 - t7812) * t183
        t7815 = t7814 / 0.2E1
        t7816 = t7788 ** 2
        t7817 = t7780 ** 2
        t7818 = t7784 ** 2
        t7820 = t7801 * (t7816 + t7817 + t7818)
        t7823 = t4 * (t7339 / 0.2E1 + t7820 / 0.2E1)
        t7824 = t7823 * t4170
        t7826 = (t7343 - t7824) * t183
        t7831 = u(t96,t1379,t233,n)
        t7833 = (t7831 - t4168) * t236
        t7834 = u(t96,t1379,t238,n)
        t7836 = (t4168 - t7834) * t236
        t7838 = t7833 / 0.2E1 + t7836 / 0.2E1
        t7272 = t7802 * (t7788 * t7795 + t7780 * t7786 + t7784 * t7782)
        t7840 = t7272 * t7838
        t7842 = (t7366 - t7840) * t183
        t7843 = t7842 / 0.2E1
        t7844 = rx(t96,t185,t233,0,0)
        t7845 = rx(t96,t185,t233,1,1)
        t7847 = rx(t96,t185,t233,2,2)
        t7849 = rx(t96,t185,t233,1,2)
        t7851 = rx(t96,t185,t233,2,1)
        t7853 = rx(t96,t185,t233,1,0)
        t7855 = rx(t96,t185,t233,0,2)
        t7857 = rx(t96,t185,t233,0,1)
        t7860 = rx(t96,t185,t233,2,0)
        t7865 = t7844 * t7845 * t7847 - t7844 * t7849 * t7851 + t7853 * 
     #t7851 * t7855 - t7853 * t7857 * t7847 + t7860 * t7857 * t7849 - t7
     #860 * t7845 * t7855
        t7866 = 0.1E1 / t7865
        t7867 = t4 * t7866
        t7873 = (t4182 - t7766) * t94
        t7875 = t4289 / 0.2E1 + t7873 / 0.2E1
        t7307 = t7867 * (t7844 * t7860 + t7857 * t7851 + t7855 * t7847)
        t7877 = t7307 * t7875
        t7879 = t3893 * t7315
        t7882 = (t7877 - t7879) * t236 / 0.2E1
        t7883 = rx(t96,t185,t238,0,0)
        t7884 = rx(t96,t185,t238,1,1)
        t7886 = rx(t96,t185,t238,2,2)
        t7888 = rx(t96,t185,t238,1,2)
        t7890 = rx(t96,t185,t238,2,1)
        t7892 = rx(t96,t185,t238,1,0)
        t7894 = rx(t96,t185,t238,0,2)
        t7896 = rx(t96,t185,t238,0,1)
        t7899 = rx(t96,t185,t238,2,0)
        t7904 = t7883 * t7884 * t7886 - t7883 * t7888 * t7890 + t7892 * 
     #t7890 * t7894 - t7892 * t7896 * t7886 + t7899 * t7896 * t7888 - t7
     #899 * t7884 * t7894
        t7905 = 0.1E1 / t7904
        t7906 = t4 * t7905
        t7912 = (t4185 - t7769) * t94
        t7914 = t4328 / 0.2E1 + t7912 / 0.2E1
        t7354 = t7906 * (t7883 * t7899 + t7896 * t7890 + t7894 * t7886)
        t7916 = t7354 * t7914
        t7919 = (t7879 - t7916) * t236 / 0.2E1
        t7925 = (t4182 - t7831) * t183
        t7927 = t5117 / 0.2E1 + t7925 / 0.2E1
        t7370 = t7867 * (t7853 * t7860 + t7845 * t7851 + t7849 * t7847)
        t7929 = t7370 * t7927
        t7931 = t6872 * t4172
        t7934 = (t7929 - t7931) * t236 / 0.2E1
        t7940 = (t4185 - t7834) * t183
        t7942 = t5315 / 0.2E1 + t7940 / 0.2E1
        t7387 = t7906 * (t7892 * t7899 + t7884 * t7890 + t7888 * t7886)
        t7944 = t7387 * t7942
        t7947 = (t7931 - t7944) * t236 / 0.2E1
        t7948 = t7860 ** 2
        t7949 = t7851 ** 2
        t7950 = t7847 ** 2
        t7952 = t7866 * (t7948 + t7949 + t7950)
        t7953 = t4145 ** 2
        t7954 = t4136 ** 2
        t7955 = t4132 ** 2
        t7957 = t4151 * (t7953 + t7954 + t7955)
        t7960 = t4 * (t7952 / 0.2E1 + t7957 / 0.2E1)
        t7961 = t7960 * t4184
        t7962 = t7899 ** 2
        t7963 = t7890 ** 2
        t7964 = t7886 ** 2
        t7966 = t7905 * (t7962 + t7963 + t7964)
        t7969 = t4 * (t7957 / 0.2E1 + t7966 / 0.2E1)
        t7970 = t7969 * t4187
        t7973 = t7746 + t4177 + t7761 + t4194 + t7778 + t7320 + t7815 + 
     #t7826 + t7369 + t7843 + t7882 + t7919 + t7934 + t7947 + (t7961 - t
     #7970) * t236
        t7974 = t7973 * t4150
        t7976 = (t7432 - t7974) * t183
        t7978 = t7712 / 0.2E1 + t7976 / 0.2E1
        t7980 = t573 * t7978
        t7983 = (t4396 - t7980) * t94 / 0.2E1
        t7984 = rx(t6542,j,t233,0,0)
        t7985 = rx(t6542,j,t233,1,1)
        t7987 = rx(t6542,j,t233,2,2)
        t7989 = rx(t6542,j,t233,1,2)
        t7991 = rx(t6542,j,t233,2,1)
        t7993 = rx(t6542,j,t233,1,0)
        t7995 = rx(t6542,j,t233,0,2)
        t7997 = rx(t6542,j,t233,0,1)
        t8000 = rx(t6542,j,t233,2,0)
        t8005 = t7984 * t7985 * t7987 - t7984 * t7989 * t7991 + t7993 * 
     #t7991 * t7995 - t7993 * t7997 * t7987 + t8000 * t7997 * t7989 - t8
     #000 * t7985 * t7995
        t8006 = 0.1E1 / t8005
        t8007 = t7984 ** 2
        t8008 = t7997 ** 2
        t8009 = t7995 ** 2
        t8011 = t8006 * (t8007 + t8008 + t8009)
        t8014 = t4 * (t5102 / 0.2E1 + t8011 / 0.2E1)
        t8015 = t8014 * t6792
        t8017 = (t5106 - t8015) * t94
        t8018 = t4 * t8006
        t8024 = (t7502 - t6625) * t183
        t8026 = (t6625 - t7766) * t183
        t8028 = t8024 / 0.2E1 + t8026 / 0.2E1
        t7469 = t8018 * (t7984 * t7993 + t7997 * t7985 + t7995 * t7989)
        t8030 = t7469 * t8028
        t8032 = (t5121 - t8030) * t94
        t8033 = t8032 / 0.2E1
        t8038 = u(t6542,j,t1430,n)
        t8040 = (t8038 - t6625) * t236
        t8042 = t8040 / 0.2E1 + t6627 / 0.2E1
        t7486 = t8018 * (t7984 * t8000 + t7997 * t7991 + t7995 * t7987)
        t8044 = t7486 * t8042
        t8046 = (t5135 - t8044) * t94
        t8047 = t8046 / 0.2E1
        t7498 = t7603 * (t7580 * t7589 + t7593 * t7581 + t7591 * t7585)
        t8053 = t7498 * t7611
        t8055 = t4798 * t7371
        t8058 = (t8053 - t8055) * t183 / 0.2E1
        t7510 = t7867 * (t7844 * t7853 + t7857 * t7845 + t7855 * t7849)
        t8064 = t7510 * t7875
        t8067 = (t8055 - t8064) * t183 / 0.2E1
        t8068 = t7589 ** 2
        t8069 = t7581 ** 2
        t8070 = t7585 ** 2
        t8072 = t7602 * (t8068 + t8069 + t8070)
        t8073 = t5084 ** 2
        t8074 = t5076 ** 2
        t8075 = t5080 ** 2
        t8077 = t5097 * (t8073 + t8074 + t8075)
        t8080 = t4 * (t8072 / 0.2E1 + t8077 / 0.2E1)
        t8081 = t8080 * t5115
        t8082 = t7853 ** 2
        t8083 = t7845 ** 2
        t8084 = t7849 ** 2
        t8086 = t7866 * (t8082 + t8083 + t8084)
        t8089 = t4 * (t8077 / 0.2E1 + t8086 / 0.2E1)
        t8090 = t8089 * t5117
        t8093 = u(t96,t180,t1430,n)
        t8095 = (t8093 - t3918) * t236
        t8097 = t8095 / 0.2E1 + t3920 / 0.2E1
        t8099 = t7140 * t8097
        t8101 = t6889 * t5133
        t8104 = (t8099 - t8101) * t183 / 0.2E1
        t8105 = u(t96,t185,t1430,n)
        t8107 = (t8105 - t4182) * t236
        t8109 = t8107 / 0.2E1 + t4184 / 0.2E1
        t8111 = t7370 * t8109
        t8114 = (t8101 - t8111) * t183 / 0.2E1
        t8115 = rx(t96,j,t1430,0,0)
        t8116 = rx(t96,j,t1430,1,1)
        t8118 = rx(t96,j,t1430,2,2)
        t8120 = rx(t96,j,t1430,1,2)
        t8122 = rx(t96,j,t1430,2,1)
        t8124 = rx(t96,j,t1430,1,0)
        t8126 = rx(t96,j,t1430,0,2)
        t8128 = rx(t96,j,t1430,0,1)
        t8131 = rx(t96,j,t1430,2,0)
        t8136 = t8115 * t8116 * t8118 - t8115 * t8120 * t8122 + t8122 * 
     #t8124 * t8126 - t8124 * t8128 * t8118 + t8131 * t8128 * t8120 - t8
     #131 * t8116 * t8126
        t8137 = 0.1E1 / t8136
        t8138 = t4 * t8137
        t8144 = (t5129 - t8038) * t94
        t8146 = t5235 / 0.2E1 + t8144 / 0.2E1
        t7586 = t8138 * (t8115 * t8131 + t8128 * t8122 + t8126 * t8118)
        t8148 = t7586 * t8146
        t8150 = (t8148 - t7373) * t236
        t8151 = t8150 / 0.2E1
        t8157 = (t8093 - t5129) * t183
        t8159 = (t5129 - t8105) * t183
        t8161 = t8157 / 0.2E1 + t8159 / 0.2E1
        t7599 = t8138 * (t8124 * t8131 + t8116 * t8122 + t8120 * t8118)
        t8163 = t7599 * t8161
        t8165 = (t8163 - t7391) * t236
        t8166 = t8165 / 0.2E1
        t8167 = t8131 ** 2
        t8168 = t8122 ** 2
        t8169 = t8118 ** 2
        t8171 = t8137 * (t8167 + t8168 + t8169)
        t8174 = t4 * (t8171 / 0.2E1 + t7410 / 0.2E1)
        t8175 = t8174 * t5131
        t8177 = (t8175 - t7419) * t236
        t8178 = t8017 + t5124 + t8033 + t5138 + t8047 + t8058 + t8067 + 
     #(t8081 - t8090) * t183 + t8104 + t8114 + t8151 + t7378 + t8166 + t
     #7396 + t8177
        t8179 = t8178 * t5096
        t8181 = (t8179 - t7432) * t236
        t8182 = rx(t6542,j,t238,0,0)
        t8183 = rx(t6542,j,t238,1,1)
        t8185 = rx(t6542,j,t238,2,2)
        t8187 = rx(t6542,j,t238,1,2)
        t8189 = rx(t6542,j,t238,2,1)
        t8191 = rx(t6542,j,t238,1,0)
        t8193 = rx(t6542,j,t238,0,2)
        t8195 = rx(t6542,j,t238,0,1)
        t8198 = rx(t6542,j,t238,2,0)
        t8203 = t8182 * t8183 * t8185 - t8182 * t8187 * t8189 + t8191 * 
     #t8189 * t8193 - t8191 * t8195 * t8185 + t8198 * t8195 * t8187 - t8
     #198 * t8183 * t8193
        t8204 = 0.1E1 / t8203
        t8205 = t8182 ** 2
        t8206 = t8195 ** 2
        t8207 = t8193 ** 2
        t8209 = t8204 * (t8205 + t8206 + t8207)
        t8212 = t4 * (t5300 / 0.2E1 + t8209 / 0.2E1)
        t8213 = t8212 * t6806
        t8215 = (t5304 - t8213) * t94
        t8216 = t4 * t8204
        t8222 = (t7505 - t6628) * t183
        t8224 = (t6628 - t7769) * t183
        t8226 = t8222 / 0.2E1 + t8224 / 0.2E1
        t7656 = t8216 * (t8182 * t8191 + t8195 * t8183 + t8193 * t8187)
        t8228 = t7656 * t8226
        t8230 = (t5319 - t8228) * t94
        t8231 = t8230 / 0.2E1
        t8236 = u(t6542,j,t1441,n)
        t8238 = (t6628 - t8236) * t236
        t8240 = t6630 / 0.2E1 + t8238 / 0.2E1
        t7668 = t8216 * (t8182 * t8198 + t8195 * t8189 + t8193 * t8185)
        t8242 = t7668 * t8240
        t8244 = (t5333 - t8242) * t94
        t8245 = t8244 / 0.2E1
        t7675 = t7642 * (t7619 * t7628 + t7632 * t7620 + t7630 * t7624)
        t8251 = t7675 * t7650
        t8253 = t4925 * t7380
        t8256 = (t8251 - t8253) * t183 / 0.2E1
        t7692 = t7906 * (t7883 * t7892 + t7896 * t7884 + t7894 * t7888)
        t8262 = t7692 * t7914
        t8265 = (t8253 - t8262) * t183 / 0.2E1
        t8266 = t7628 ** 2
        t8267 = t7620 ** 2
        t8268 = t7624 ** 2
        t8270 = t7641 * (t8266 + t8267 + t8268)
        t8271 = t5282 ** 2
        t8272 = t5274 ** 2
        t8273 = t5278 ** 2
        t8275 = t5295 * (t8271 + t8272 + t8273)
        t8278 = t4 * (t8270 / 0.2E1 + t8275 / 0.2E1)
        t8279 = t8278 * t5313
        t8280 = t7892 ** 2
        t8281 = t7884 ** 2
        t8282 = t7888 ** 2
        t8284 = t7905 * (t8280 + t8281 + t8282)
        t8287 = t4 * (t8275 / 0.2E1 + t8284 / 0.2E1)
        t8288 = t8287 * t5315
        t8291 = u(t96,t180,t1441,n)
        t8293 = (t3921 - t8291) * t236
        t8295 = t3923 / 0.2E1 + t8293 / 0.2E1
        t8297 = t7152 * t8295
        t8299 = t6895 * t5331
        t8302 = (t8297 - t8299) * t183 / 0.2E1
        t8303 = u(t96,t185,t1441,n)
        t8305 = (t4185 - t8303) * t236
        t8307 = t4187 / 0.2E1 + t8305 / 0.2E1
        t8309 = t7387 * t8307
        t8312 = (t8299 - t8309) * t183 / 0.2E1
        t8313 = rx(t96,j,t1441,0,0)
        t8314 = rx(t96,j,t1441,1,1)
        t8316 = rx(t96,j,t1441,2,2)
        t8318 = rx(t96,j,t1441,1,2)
        t8320 = rx(t96,j,t1441,2,1)
        t8322 = rx(t96,j,t1441,1,0)
        t8324 = rx(t96,j,t1441,0,2)
        t8326 = rx(t96,j,t1441,0,1)
        t8329 = rx(t96,j,t1441,2,0)
        t8334 = t8313 * t8314 * t8316 - t8313 * t8318 * t8320 + t8322 * 
     #t8320 * t8324 - t8322 * t8326 * t8316 + t8329 * t8326 * t8318 - t8
     #329 * t8314 * t8324
        t8335 = 0.1E1 / t8334
        t8336 = t4 * t8335
        t8342 = (t5327 - t8236) * t94
        t8344 = t5433 / 0.2E1 + t8342 / 0.2E1
        t7767 = t8336 * (t8313 * t8329 + t8326 * t8320 + t8324 * t8316)
        t8346 = t7767 * t8344
        t8348 = (t7382 - t8346) * t236
        t8349 = t8348 / 0.2E1
        t8355 = (t8291 - t5327) * t183
        t8357 = (t5327 - t8303) * t183
        t8359 = t8355 / 0.2E1 + t8357 / 0.2E1
        t7787 = t8336 * (t8322 * t8329 + t8314 * t8320 + t8318 * t8316)
        t8361 = t7787 * t8359
        t8363 = (t7402 - t8361) * t236
        t8364 = t8363 / 0.2E1
        t8365 = t8329 ** 2
        t8366 = t8320 ** 2
        t8367 = t8316 ** 2
        t8369 = t8335 * (t8365 + t8366 + t8367)
        t8372 = t4 * (t7424 / 0.2E1 + t8369 / 0.2E1)
        t8373 = t8372 * t5329
        t8375 = (t7428 - t8373) * t236
        t8376 = t8215 + t5322 + t8231 + t5336 + t8245 + t8256 + t8265 + 
     #(t8279 - t8288) * t183 + t8302 + t8312 + t7385 + t8349 + t7405 + t
     #8364 + t8375
        t8377 = t8376 * t5294
        t8379 = (t7432 - t8377) * t236
        t8381 = t8181 / 0.2E1 + t8379 / 0.2E1
        t8383 = t590 * t8381
        t8386 = (t5474 - t8383) * t94 / 0.2E1
        t8388 = (t4126 - t7710) * t94
        t8390 = t5481 / 0.2E1 + t8388 / 0.2E1
        t8392 = t629 * t8390
        t8394 = t214 * t7436
        t8397 = (t8392 - t8394) * t183 / 0.2E1
        t8399 = (t4390 - t7974) * t94
        t8401 = t5494 / 0.2E1 + t8399 / 0.2E1
        t8403 = t669 * t8401
        t8406 = (t8394 - t8403) * t183 / 0.2E1
        t8407 = t700 * t4128
        t8408 = t709 * t4392
        t8411 = t7580 ** 2
        t8412 = t7593 ** 2
        t8413 = t7591 ** 2
        t8415 = t7602 * (t8411 + t8412 + t8413)
        t8418 = t4 * (t5524 / 0.2E1 + t8415 / 0.2E1)
        t8419 = t8418 * t4025
        t8423 = t7498 * t7663
        t8426 = (t5539 - t8423) * t94 / 0.2E1
        t8428 = t7086 * t8097
        t8431 = (t5551 - t8428) * t94 / 0.2E1
        t8432 = rx(i,t1331,t233,0,0)
        t8433 = rx(i,t1331,t233,1,1)
        t8435 = rx(i,t1331,t233,2,2)
        t8437 = rx(i,t1331,t233,1,2)
        t8439 = rx(i,t1331,t233,2,1)
        t8441 = rx(i,t1331,t233,1,0)
        t8443 = rx(i,t1331,t233,0,2)
        t8445 = rx(i,t1331,t233,0,1)
        t8448 = rx(i,t1331,t233,2,0)
        t8453 = t8432 * t8433 * t8435 - t8432 * t8437 * t8439 + t8441 * 
     #t8439 * t8443 - t8445 * t8441 * t8435 + t8448 * t8445 * t8437 - t8
     #448 * t8433 * t8443
        t8454 = 0.1E1 / t8453
        t8455 = t4 * t8454
        t8461 = (t3983 - t7567) * t94
        t8463 = t5586 / 0.2E1 + t8461 / 0.2E1
        t7874 = t8455 * (t8441 * t8432 + t8445 * t8433 + t8443 * t8437)
        t8465 = t7874 * t8463
        t8467 = (t8465 - t5144) * t183
        t8468 = t8467 / 0.2E1
        t8469 = t8441 ** 2
        t8470 = t8433 ** 2
        t8471 = t8437 ** 2
        t8473 = t8454 * (t8469 + t8470 + t8471)
        t8476 = t4 * (t8473 / 0.2E1 + t5163 / 0.2E1)
        t8477 = t8476 * t4077
        t8479 = (t8477 - t5172) * t183
        t8484 = u(i,t1331,t1430,n)
        t8486 = (t8484 - t3983) * t236
        t8488 = t8486 / 0.2E1 + t3985 / 0.2E1
        t7897 = t8455 * (t8441 * t8448 + t8433 * t8439 + t8437 * t8435)
        t8490 = t7897 * t8488
        t8492 = (t8490 - t5190) * t183
        t8493 = t8492 / 0.2E1
        t8494 = rx(i,t180,t1430,0,0)
        t8495 = rx(i,t180,t1430,1,1)
        t8497 = rx(i,t180,t1430,2,2)
        t8499 = rx(i,t180,t1430,1,2)
        t8501 = rx(i,t180,t1430,2,1)
        t8503 = rx(i,t180,t1430,1,0)
        t8505 = rx(i,t180,t1430,0,2)
        t8507 = rx(i,t180,t1430,0,1)
        t8510 = rx(i,t180,t1430,2,0)
        t8515 = t8494 * t8495 * t8497 - t8494 * t8499 * t8501 + t8503 * 
     #t8501 * t8505 - t8503 * t8507 * t8497 + t8510 * t8507 * t8499 - t8
     #510 * t8495 * t8505
        t8516 = 0.1E1 / t8515
        t8517 = t4 * t8516
        t8523 = (t5184 - t8093) * t94
        t8525 = t5650 / 0.2E1 + t8523 / 0.2E1
        t7928 = t8517 * (t8494 * t8510 + t8507 * t8501 + t8505 * t8497)
        t8527 = t7928 * t8525
        t8529 = (t8527 - t4029) * t236
        t8530 = t8529 / 0.2E1
        t8536 = (t8484 - t5184) * t183
        t8538 = t8536 / 0.2E1 + t5248 / 0.2E1
        t7939 = t8517 * (t8503 * t8510 + t8495 * t8501 + t8499 * t8497)
        t8540 = t7939 * t8538
        t8542 = (t8540 - t4081) * t236
        t8543 = t8542 / 0.2E1
        t8544 = t8510 ** 2
        t8545 = t8501 ** 2
        t8546 = t8497 ** 2
        t8548 = t8516 * (t8544 + t8545 + t8546)
        t8551 = t4 * (t8548 / 0.2E1 + t4104 / 0.2E1)
        t8552 = t8551 * t5186
        t8554 = (t8552 - t4113) * t236
        t8555 = (t5528 - t8419) * t94 + t5542 + t8426 + t5554 + t8431 + 
     #t8468 + t5149 + t8479 + t8493 + t5195 + t8530 + t4034 + t8543 + t4
     #086 + t8554
        t8556 = t8555 * t4017
        t8558 = (t8556 - t4126) * t236
        t8559 = t7619 ** 2
        t8560 = t7632 ** 2
        t8561 = t7630 ** 2
        t8563 = t7641 * (t8559 + t8560 + t8561)
        t8566 = t4 * (t5704 / 0.2E1 + t8563 / 0.2E1)
        t8567 = t8566 * t4064
        t8571 = t7675 * t7678
        t8574 = (t5719 - t8571) * t94 / 0.2E1
        t8576 = t7122 * t8295
        t8579 = (t5731 - t8576) * t94 / 0.2E1
        t8580 = rx(i,t1331,t238,0,0)
        t8581 = rx(i,t1331,t238,1,1)
        t8583 = rx(i,t1331,t238,2,2)
        t8585 = rx(i,t1331,t238,1,2)
        t8587 = rx(i,t1331,t238,2,1)
        t8589 = rx(i,t1331,t238,1,0)
        t8591 = rx(i,t1331,t238,0,2)
        t8593 = rx(i,t1331,t238,0,1)
        t8596 = rx(i,t1331,t238,2,0)
        t8601 = t8580 * t8581 * t8583 - t8580 * t8585 * t8587 + t8589 * 
     #t8587 * t8591 - t8589 * t8593 * t8583 + t8596 * t8593 * t8585 - t8
     #596 * t8581 * t8591
        t8602 = 0.1E1 / t8601
        t8603 = t4 * t8602
        t8609 = (t3986 - t7570) * t94
        t8611 = t5766 / 0.2E1 + t8609 / 0.2E1
        t8013 = t8603 * (t8580 * t8589 + t8593 * t8581 + t8591 * t8585)
        t8613 = t8013 * t8611
        t8615 = (t8613 - t5342) * t183
        t8616 = t8615 / 0.2E1
        t8617 = t8589 ** 2
        t8618 = t8581 ** 2
        t8619 = t8585 ** 2
        t8621 = t8602 * (t8617 + t8618 + t8619)
        t8624 = t4 * (t8621 / 0.2E1 + t5361 / 0.2E1)
        t8625 = t8624 * t4092
        t8627 = (t8625 - t5370) * t183
        t8632 = u(i,t1331,t1441,n)
        t8634 = (t3986 - t8632) * t236
        t8636 = t3988 / 0.2E1 + t8634 / 0.2E1
        t8035 = t8603 * (t8589 * t8596 + t8581 * t8587 + t8585 * t8583)
        t8638 = t8035 * t8636
        t8640 = (t8638 - t5388) * t183
        t8641 = t8640 / 0.2E1
        t8642 = rx(i,t180,t1441,0,0)
        t8643 = rx(i,t180,t1441,1,1)
        t8645 = rx(i,t180,t1441,2,2)
        t8647 = rx(i,t180,t1441,1,2)
        t8649 = rx(i,t180,t1441,2,1)
        t8651 = rx(i,t180,t1441,1,0)
        t8653 = rx(i,t180,t1441,0,2)
        t8655 = rx(i,t180,t1441,0,1)
        t8658 = rx(i,t180,t1441,2,0)
        t8663 = t8642 * t8643 * t8645 - t8642 * t8647 * t8649 + t8651 * 
     #t8649 * t8653 - t8651 * t8655 * t8645 + t8658 * t8655 * t8647 - t8
     #658 * t8643 * t8653
        t8664 = 0.1E1 / t8663
        t8665 = t4 * t8664
        t8671 = (t5382 - t8291) * t94
        t8673 = t5830 / 0.2E1 + t8671 / 0.2E1
        t8071 = t8665 * (t8642 * t8658 + t8655 * t8649 + t8653 * t8645)
        t8675 = t8071 * t8673
        t8677 = (t4068 - t8675) * t236
        t8678 = t8677 / 0.2E1
        t8684 = (t8632 - t5382) * t183
        t8686 = t8684 / 0.2E1 + t5446 / 0.2E1
        t8091 = t8665 * (t8651 * t8658 + t8643 * t8649 + t8647 * t8645)
        t8688 = t8091 * t8686
        t8690 = (t4096 - t8688) * t236
        t8691 = t8690 / 0.2E1
        t8692 = t8658 ** 2
        t8693 = t8649 ** 2
        t8694 = t8645 ** 2
        t8696 = t8664 * (t8692 + t8693 + t8694)
        t8699 = t4 * (t4118 / 0.2E1 + t8696 / 0.2E1)
        t8700 = t8699 * t5384
        t8702 = (t4122 - t8700) * t236
        t8703 = (t5708 - t8567) * t94 + t5722 + t8574 + t5734 + t8579 + 
     #t8616 + t5347 + t8627 + t8641 + t5393 + t4071 + t8678 + t4099 + t8
     #691 + t8702
        t8704 = t8703 * t4056
        t8706 = (t4126 - t8704) * t236
        t8708 = t8558 / 0.2E1 + t8706 / 0.2E1
        t8710 = t708 * t8708
        t8712 = t715 * t5472
        t8715 = (t8710 - t8712) * t183 / 0.2E1
        t8716 = t7844 ** 2
        t8717 = t7857 ** 2
        t8718 = t7855 ** 2
        t8720 = t7866 * (t8716 + t8717 + t8718)
        t8723 = t4 * (t5893 / 0.2E1 + t8720 / 0.2E1)
        t8724 = t8723 * t4289
        t8728 = t7510 * t7927
        t8731 = (t5908 - t8728) * t94 / 0.2E1
        t8733 = t7307 * t8109
        t8736 = (t5920 - t8733) * t94 / 0.2E1
        t8737 = rx(i,t1379,t233,0,0)
        t8738 = rx(i,t1379,t233,1,1)
        t8740 = rx(i,t1379,t233,2,2)
        t8742 = rx(i,t1379,t233,1,2)
        t8744 = rx(i,t1379,t233,2,1)
        t8746 = rx(i,t1379,t233,1,0)
        t8748 = rx(i,t1379,t233,0,2)
        t8750 = rx(i,t1379,t233,0,1)
        t8753 = rx(i,t1379,t233,2,0)
        t8758 = t8737 * t8738 * t8740 - t8737 * t8742 * t8744 + t8746 * 
     #t8744 * t8748 - t8746 * t8750 * t8740 + t8753 * t8750 * t8742 - t8
     #753 * t8738 * t8748
        t8759 = 0.1E1 / t8758
        t8760 = t4 * t8759
        t8766 = (t4247 - t7831) * t94
        t8768 = t5955 / 0.2E1 + t8766 / 0.2E1
        t8158 = t8760 * (t8737 * t8746 + t8750 * t8738 + t8748 * t8742)
        t8770 = t8158 * t8768
        t8772 = (t5155 - t8770) * t183
        t8773 = t8772 / 0.2E1
        t8774 = t8746 ** 2
        t8775 = t8738 ** 2
        t8776 = t8742 ** 2
        t8778 = t8759 * (t8774 + t8775 + t8776)
        t8781 = t4 * (t5177 / 0.2E1 + t8778 / 0.2E1)
        t8782 = t8781 * t4341
        t8784 = (t5181 - t8782) * t183
        t8789 = u(i,t1379,t1430,n)
        t8791 = (t8789 - t4247) * t236
        t8793 = t8791 / 0.2E1 + t4249 / 0.2E1
        t8188 = t8760 * (t8746 * t8753 + t8738 * t8744 + t8742 * t8740)
        t8795 = t8188 * t8793
        t8797 = (t5202 - t8795) * t183
        t8798 = t8797 / 0.2E1
        t8799 = rx(i,t185,t1430,0,0)
        t8800 = rx(i,t185,t1430,1,1)
        t8802 = rx(i,t185,t1430,2,2)
        t8804 = rx(i,t185,t1430,1,2)
        t8806 = rx(i,t185,t1430,2,1)
        t8808 = rx(i,t185,t1430,1,0)
        t8810 = rx(i,t185,t1430,0,2)
        t8812 = rx(i,t185,t1430,0,1)
        t8815 = rx(i,t185,t1430,2,0)
        t8820 = t8799 * t8800 * t8802 - t8799 * t8804 * t8806 + t8808 * 
     #t8806 * t8810 - t8808 * t8812 * t8802 + t8815 * t8812 * t8804 - t8
     #815 * t8800 * t8810
        t8821 = 0.1E1 / t8820
        t8822 = t4 * t8821
        t8828 = (t5196 - t8105) * t94
        t8830 = t6019 / 0.2E1 + t8828 / 0.2E1
        t8225 = t8822 * (t8799 * t8815 + t8812 * t8806 + t8810 * t8802)
        t8832 = t8225 * t8830
        t8834 = (t8832 - t4293) * t236
        t8835 = t8834 / 0.2E1
        t8841 = (t5196 - t8789) * t183
        t8843 = t5250 / 0.2E1 + t8841 / 0.2E1
        t8239 = t8822 * (t8808 * t8815 + t8800 * t8806 + t8804 * t8802)
        t8845 = t8239 * t8843
        t8847 = (t8845 - t4345) * t236
        t8848 = t8847 / 0.2E1
        t8849 = t8815 ** 2
        t8850 = t8806 ** 2
        t8851 = t8802 ** 2
        t8853 = t8821 * (t8849 + t8850 + t8851)
        t8856 = t4 * (t8853 / 0.2E1 + t4368 / 0.2E1)
        t8857 = t8856 * t5198
        t8859 = (t8857 - t4377) * t236
        t8860 = (t5897 - t8724) * t94 + t5911 + t8731 + t5923 + t8736 + 
     #t5158 + t8773 + t8784 + t5205 + t8798 + t8835 + t4298 + t8848 + t4
     #350 + t8859
        t8861 = t8860 * t4281
        t8863 = (t8861 - t4390) * t236
        t8864 = t7883 ** 2
        t8865 = t7896 ** 2
        t8866 = t7894 ** 2
        t8868 = t7905 * (t8864 + t8865 + t8866)
        t8871 = t4 * (t6073 / 0.2E1 + t8868 / 0.2E1)
        t8872 = t8871 * t4328
        t8876 = t7692 * t7942
        t8879 = (t6088 - t8876) * t94 / 0.2E1
        t8881 = t7354 * t8307
        t8884 = (t6100 - t8881) * t94 / 0.2E1
        t8885 = rx(i,t1379,t238,0,0)
        t8886 = rx(i,t1379,t238,1,1)
        t8888 = rx(i,t1379,t238,2,2)
        t8890 = rx(i,t1379,t238,1,2)
        t8892 = rx(i,t1379,t238,2,1)
        t8894 = rx(i,t1379,t238,1,0)
        t8896 = rx(i,t1379,t238,0,2)
        t8898 = rx(i,t1379,t238,0,1)
        t8901 = rx(i,t1379,t238,2,0)
        t8906 = t8885 * t8886 * t8888 - t8885 * t8890 * t8892 + t8894 * 
     #t8892 * t8896 - t8894 * t8898 * t8888 + t8901 * t8898 * t8890 - t8
     #901 * t8886 * t8896
        t8907 = 0.1E1 / t8906
        t8908 = t4 * t8907
        t8914 = (t4250 - t7834) * t94
        t8916 = t6135 / 0.2E1 + t8914 / 0.2E1
        t8308 = t8908 * (t8885 * t8894 + t8898 * t8886 + t8896 * t8890)
        t8918 = t8308 * t8916
        t8920 = (t5353 - t8918) * t183
        t8921 = t8920 / 0.2E1
        t8922 = t8894 ** 2
        t8923 = t8886 ** 2
        t8924 = t8890 ** 2
        t8926 = t8907 * (t8922 + t8923 + t8924)
        t8929 = t4 * (t5375 / 0.2E1 + t8926 / 0.2E1)
        t8930 = t8929 * t4356
        t8932 = (t5379 - t8930) * t183
        t8937 = u(i,t1379,t1441,n)
        t8939 = (t4250 - t8937) * t236
        t8941 = t4252 / 0.2E1 + t8939 / 0.2E1
        t8330 = t8908 * (t8894 * t8901 + t8886 * t8892 + t8890 * t8888)
        t8943 = t8330 * t8941
        t8945 = (t5400 - t8943) * t183
        t8946 = t8945 / 0.2E1
        t8947 = rx(i,t185,t1441,0,0)
        t8948 = rx(i,t185,t1441,1,1)
        t8950 = rx(i,t185,t1441,2,2)
        t8952 = rx(i,t185,t1441,1,2)
        t8954 = rx(i,t185,t1441,2,1)
        t8956 = rx(i,t185,t1441,1,0)
        t8958 = rx(i,t185,t1441,0,2)
        t8960 = rx(i,t185,t1441,0,1)
        t8963 = rx(i,t185,t1441,2,0)
        t8968 = t8947 * t8948 * t8950 - t8947 * t8952 * t8954 + t8956 * 
     #t8954 * t8958 - t8956 * t8960 * t8950 + t8963 * t8960 * t8952 - t8
     #963 * t8948 * t8958
        t8969 = 0.1E1 / t8968
        t8970 = t4 * t8969
        t8976 = (t5394 - t8303) * t94
        t8978 = t6199 / 0.2E1 + t8976 / 0.2E1
        t8368 = t8970 * (t8947 * t8963 + t8960 * t8954 + t8958 * t8950)
        t8980 = t8368 * t8978
        t8982 = (t4332 - t8980) * t236
        t8983 = t8982 / 0.2E1
        t8989 = (t5394 - t8937) * t183
        t8991 = t5448 / 0.2E1 + t8989 / 0.2E1
        t8384 = t8970 * (t8956 * t8963 + t8948 * t8954 + t8952 * t8950)
        t8993 = t8384 * t8991
        t8995 = (t4360 - t8993) * t236
        t8996 = t8995 / 0.2E1
        t8997 = t8963 ** 2
        t8998 = t8954 ** 2
        t8999 = t8950 ** 2
        t9001 = t8969 * (t8997 + t8998 + t8999)
        t9004 = t4 * (t4382 / 0.2E1 + t9001 / 0.2E1)
        t9005 = t9004 * t5396
        t9007 = (t4386 - t9005) * t236
        t9008 = (t6077 - t8872) * t94 + t6091 + t8879 + t6103 + t8884 + 
     #t5356 + t8921 + t8932 + t5403 + t8946 + t4335 + t8983 + t4363 + t8
     #996 + t9007
        t9009 = t9008 * t4320
        t9011 = (t4390 - t9009) * t236
        t9013 = t8863 / 0.2E1 + t9011 / 0.2E1
        t9015 = t731 * t9013
        t9018 = (t8712 - t9015) * t183 / 0.2E1
        t9020 = (t5270 - t8179) * t94
        t9022 = t6245 / 0.2E1 + t9020 / 0.2E1
        t9024 = t771 * t9022
        t9026 = t265 * t7436
        t9029 = (t9024 - t9026) * t236 / 0.2E1
        t9031 = (t5468 - t8377) * t94
        t9033 = t6258 / 0.2E1 + t9031 / 0.2E1
        t9035 = t809 * t9033
        t9038 = (t9026 - t9035) * t236 / 0.2E1
        t9040 = (t8556 - t5270) * t183
        t9042 = (t5270 - t8861) * t183
        t9044 = t9040 / 0.2E1 + t9042 / 0.2E1
        t9046 = t822 * t9044
        t9048 = t715 * t4394
        t9051 = (t9046 - t9048) * t236 / 0.2E1
        t9053 = (t8704 - t5468) * t183
        t9055 = (t5468 - t9009) * t183
        t9057 = t9053 / 0.2E1 + t9055 / 0.2E1
        t9059 = t837 * t9057
        t9062 = (t9048 - t9059) * t236 / 0.2E1
        t9063 = t873 * t5272
        t9064 = t882 * t5470
        t9067 = (t2919 - t7446) * t94 + t4399 + t7983 + t5477 + t8386 + 
     #t8397 + t8406 + (t8407 - t8408) * t183 + t8715 + t9018 + t9029 + t
     #9038 + t9051 + t9062 + (t9063 - t9064) * t236
        t9068 = t9067 * t56
        t9070 = t2917 * t9068 / 0.6E1
        t9071 = t7248 / 0.2E1
        t9072 = t6886 / 0.2E1
        t9074 = t1114 / 0.2E1 + t7050 / 0.2E1
        t9076 = t3630 * t9074
        t9078 = t147 / 0.2E1 + t6841 / 0.2E1
        t9080 = t573 * t9078
        t9082 = (t9076 - t9080) * t183
        t9083 = t9082 / 0.2E1
        t9085 = t1127 / 0.2E1 + t7069 / 0.2E1
        t9087 = t3877 * t9085
        t9089 = (t9080 - t9087) * t183
        t9090 = t9089 / 0.2E1
        t9091 = t7333 * t1089
        t9092 = t7342 * t1092
        t9094 = (t9091 - t9092) * t183
        t9095 = ut(t96,t180,t233,n)
        t9097 = (t9095 - t1087) * t236
        t9098 = ut(t96,t180,t238,n)
        t9100 = (t1087 - t9098) * t236
        t9102 = t9097 / 0.2E1 + t9100 / 0.2E1
        t9104 = t6856 * t9102
        t9106 = t6863 * t1107
        t9108 = (t9104 - t9106) * t183
        t9109 = t9108 / 0.2E1
        t9110 = ut(t96,t185,t233,n)
        t9112 = (t9110 - t1090) * t236
        t9113 = ut(t96,t185,t238,n)
        t9115 = (t1090 - t9113) * t236
        t9117 = t9112 / 0.2E1 + t9115 / 0.2E1
        t9119 = t6872 * t9117
        t9121 = (t9106 - t9119) * t183
        t9122 = t9121 / 0.2E1
        t9124 = t1168 / 0.2E1 + t7084 / 0.2E1
        t9126 = t4807 * t9124
        t9128 = t590 * t9078
        t9130 = (t9126 - t9128) * t236
        t9131 = t9130 / 0.2E1
        t9133 = t1179 / 0.2E1 + t7098 / 0.2E1
        t9135 = t4947 * t9133
        t9137 = (t9128 - t9135) * t236
        t9138 = t9137 / 0.2E1
        t9140 = (t9095 - t1100) * t183
        t9142 = (t1100 - t9110) * t183
        t9144 = t9140 / 0.2E1 + t9142 / 0.2E1
        t9146 = t6889 * t9144
        t9148 = t6863 * t1094
        t9150 = (t9146 - t9148) * t236
        t9151 = t9150 / 0.2E1
        t9153 = (t9098 - t1103) * t183
        t9155 = (t1103 - t9113) * t183
        t9157 = t9153 / 0.2E1 + t9155 / 0.2E1
        t9159 = t6895 * t9157
        t9161 = (t9148 - t9159) * t236
        t9162 = t9161 / 0.2E1
        t9163 = t7418 * t1102
        t9164 = t7427 * t1105
        t9166 = (t9163 - t9164) * t236
        t9167 = t7040 + t1099 + t9071 + t1112 + t9072 + t9083 + t9090 + 
     #t9094 + t9109 + t9122 + t9131 + t9138 + t9151 + t9162 + t9166
        t9168 = t9167 * t118
        t9169 = t1216 - t9168
        t9170 = t9169 * t94
        t9173 = dx * (t6398 / 0.2E1 + t9170 / 0.2E1)
        t9175 = t2076 * t9173 / 0.4E1
        t9177 = dx * (t2905 - t7434)
        t9179 = t136 * t9177 / 0.12E2
        t9180 = t137 + t136 * t2057 - t2075 + t2076 * t2628 / 0.2E1 - t1
     #36 * t2908 / 0.2E1 + t2916 + t2917 * t6295 / 0.6E1 - t2076 * t6401
     # / 0.4E1 + t136 * t6405 / 0.12E2 - t2 - t6837 - t6852 - t7300 - t7
     #439 - t7445 - t9070 - t9175 - t9179
        t9184 = 0.8E1 * t58
        t9185 = 0.8E1 * t59
        t9186 = 0.8E1 * t60
        t9196 = sqrt(0.8E1 * t29 + 0.8E1 * t30 + 0.8E1 * t31 + t9184 + t
     #9185 + t9186 - 0.2E1 * dx * ((t88 + t89 + t90 - t29 - t30 - t31) *
     # t94 / 0.2E1 - (t58 + t59 + t60 - t120 - t121 - t122) * t94 / 0.2E
     #1))
        t9197 = 0.1E1 / t9196
        t9201 = 0.1E1 / 0.2E1 - t134
        t9202 = t9201 * dt
        t9204 = t132 * t9202 * t153
        t9205 = t9201 ** 2
        t9208 = t158 * t9205 * t890 / 0.2E1
        t9209 = t9205 * t9201
        t9212 = t158 * t9209 * t1219 / 0.6E1
        t9214 = t9202 * t1223 / 0.24E2
        t9216 = t9205 * t161
        t9221 = t9209 * t895
        t9228 = t9202 * t6836
        t9230 = t9216 * t7298 / 0.2E1
        t9232 = t9202 * t7437 / 0.2E1
        t9234 = t9221 * t9068 / 0.6E1
        t9236 = t9216 * t9173 / 0.4E1
        t9238 = t9202 * t9177 / 0.12E2
        t9239 = t137 + t9202 * t2057 - t2075 + t9216 * t2628 / 0.2E1 - t
     #9202 * t2908 / 0.2E1 + t2916 + t9221 * t6295 / 0.6E1 - t9216 * t64
     #01 / 0.4E1 + t9202 * t6405 / 0.12E2 - t2 - t9228 - t6852 - t9230 -
     # t9232 - t7445 - t9234 - t9236 - t9238
        t9242 = 0.2E1 * t1226 * t9239 * t9197
        t9244 = (t132 * t136 * t153 + t158 * t159 * t890 / 0.2E1 + t158 
     #* t893 * t1219 / 0.6E1 - t136 * t1223 / 0.24E2 + 0.2E1 * t1226 * t
     #9180 * t9197 - t9204 - t9208 - t9212 + t9214 - t9242) * t133
        t9250 = t132 * (t171 - dx * t1613 / 0.24E2)
        t9252 = dx * t1631 / 0.24E2
        t9257 = t28 * t197
        t9259 = t57 * t215
        t9260 = t9259 / 0.2E1
        t9264 = t119 * t579
        t9272 = t4 * (t9257 / 0.2E1 + t9260 - dx * ((t87 * t179 - t9257)
     # * t94 / 0.2E1 - (t9259 - t9264) * t94 / 0.2E1) / 0.8E1)
        t9277 = t1330 * (t2427 / 0.2E1 + t2432 / 0.2E1)
        t9279 = t925 / 0.4E1
        t9280 = t928 / 0.4E1
        t9283 = t1330 * (t6966 / 0.2E1 + t6971 / 0.2E1)
        t9284 = t9283 / 0.12E2
        t9290 = (t902 - t905) * t183
        t9301 = t912 / 0.2E1
        t9302 = t915 / 0.2E1
        t9303 = t9277 / 0.6E1
        t9306 = t925 / 0.2E1
        t9307 = t928 / 0.2E1
        t9308 = t9283 / 0.6E1
        t9309 = t1089 / 0.2E1
        t9310 = t1092 / 0.2E1
        t9314 = (t1089 - t1092) * t183
        t9316 = ((t7151 - t1089) * t183 - t9314) * t183
        t9320 = (t9314 - (t1092 - t7156) * t183) * t183
        t9323 = t1330 * (t9316 / 0.2E1 + t9320 / 0.2E1)
        t9324 = t9323 / 0.6E1
        t9331 = t912 / 0.4E1 + t915 / 0.4E1 - t9277 / 0.12E2 + t9279 + t
     #9280 - t9284 - dx * ((t902 / 0.2E1 + t905 / 0.2E1 - t1330 * (((t20
     #79 - t902) * t183 - t9290) * t183 / 0.2E1 + (t9290 - (t905 - t2085
     #) * t183) * t183 / 0.2E1) / 0.6E1 - t9301 - t9302 + t9303) * t94 /
     # 0.2E1 - (t9306 + t9307 - t9308 - t9309 - t9310 + t9324) * t94 / 0
     #.2E1) / 0.8E1
        t9336 = t4 * (t9257 / 0.2E1 + t9259 / 0.2E1)
        t9338 = t3699 / 0.4E1 + t3857 / 0.4E1 + t4128 / 0.4E1 + t4392 / 
     #0.4E1
        t9343 = t3549 * t975
        t9345 = (t2966 * t973 - t9343) * t94
        t9351 = t2095 / 0.2E1 + t912 / 0.2E1
        t9353 = t306 * t9351
        t9355 = (t2524 * (t2079 / 0.2E1 + t902 / 0.2E1) - t9353) * t94
        t9358 = t2113 / 0.2E1 + t925 / 0.2E1
        t9360 = t629 * t9358
        t9362 = (t9353 - t9360) * t94
        t9363 = t9362 / 0.2E1
        t9367 = t2760 * t1009
        t9369 = (t2753 * t6329 - t9367) * t94
        t9372 = t3371 * t1146
        t9374 = (t9367 - t9372) * t94
        t9375 = t9374 / 0.2E1
        t9379 = (t6322 - t1002) * t94
        t9381 = (t1002 - t1139) * t94
        t9383 = t9379 / 0.2E1 + t9381 / 0.2E1
        t9387 = t2760 * t977
        t9392 = (t6325 - t1005) * t94
        t9394 = (t1005 - t1142) * t94
        t9396 = t9392 / 0.2E1 + t9394 / 0.2E1
        t9403 = t2550 / 0.2E1 + t1055 / 0.2E1
        t9407 = t388 * t9351
        t9412 = t2569 / 0.2E1 + t1068 / 0.2E1
        t9422 = t9345 + t9355 / 0.2E1 + t9363 + t9369 / 0.2E1 + t9375 + 
     #t2597 / 0.2E1 + t986 + t2438 + t2356 / 0.2E1 + t1016 + (t3396 * t9
     #383 - t9387) * t236 / 0.2E1 + (t9387 - t3427 * t9396) * t236 / 0.2
     #E1 + (t3437 * t9403 - t9407) * t236 / 0.2E1 + (t9407 - t3450 * t94
     #12) * t236 / 0.2E1 + (t3683 * t1004 - t3692 * t1007) * t236
        t9423 = t9422 * t301
        t9427 = t3707 * t990
        t9429 = (t3274 * t988 - t9427) * t94
        t9435 = t915 / 0.2E1 + t2101 / 0.2E1
        t9437 = t348 * t9435
        t9439 = (t2547 * (t905 / 0.2E1 + t2085 / 0.2E1) - t9437) * t94
        t9442 = t928 / 0.2E1 + t2119 / 0.2E1
        t9444 = t669 * t9442
        t9446 = (t9437 - t9444) * t94
        t9447 = t9446 / 0.2E1
        t9451 = t3095 * t1024
        t9453 = (t3089 * t6344 - t9451) * t94
        t9456 = t3481 * t1161
        t9458 = (t9451 - t9456) * t94
        t9459 = t9458 / 0.2E1
        t9463 = (t6337 - t1017) * t94
        t9465 = (t1017 - t1154) * t94
        t9467 = t9463 / 0.2E1 + t9465 / 0.2E1
        t9471 = t3095 * t992
        t9476 = (t6340 - t1020) * t94
        t9478 = (t1020 - t1157) * t94
        t9480 = t9476 / 0.2E1 + t9478 / 0.2E1
        t9487 = t1057 / 0.2E1 + t2555 / 0.2E1
        t9491 = t411 * t9435
        t9496 = t1070 / 0.2E1 + t2574 / 0.2E1
        t9506 = t9429 + t9439 / 0.2E1 + t9447 + t9453 / 0.2E1 + t9459 + 
     #t997 + t2613 / 0.2E1 + t2443 + t1029 + t2374 / 0.2E1 + (t3505 * t9
     #467 - t9471) * t236 / 0.2E1 + (t9471 - t3537 * t9480) * t236 / 0.2
     #E1 + (t3548 * t9487 - t9491) * t236 / 0.2E1 + (t9491 - t3562 * t94
     #96) * t236 / 0.2E1 + (t3841 * t1019 - t3850 * t1022) * t236
        t9507 = t9506 * t344
        t9510 = t3895 * t1114
        t9512 = (t9343 - t9510) * t94
        t9514 = t7151 / 0.2E1 + t1089 / 0.2E1
        t9516 = t3630 * t9514
        t9518 = (t9360 - t9516) * t94
        t9519 = t9518 / 0.2E1
        t9521 = t3645 * t9102
        t9523 = (t9372 - t9521) * t94
        t9524 = t9523 / 0.2E1
        t9525 = t6932 / 0.2E1
        t9526 = t7001 / 0.2E1
        t9528 = (t1139 - t9095) * t94
        t9530 = t9381 / 0.2E1 + t9528 / 0.2E1
        t9532 = t3753 * t9530
        t9534 = t3371 * t1116
        t9536 = (t9532 - t9534) * t236
        t9537 = t9536 / 0.2E1
        t9539 = (t1142 - t9098) * t94
        t9541 = t9394 / 0.2E1 + t9539 / 0.2E1
        t9543 = t3787 * t9541
        t9545 = (t9534 - t9543) * t236
        t9546 = t9545 / 0.2E1
        t9548 = t7201 / 0.2E1 + t1188 / 0.2E1
        t9550 = t3800 * t9548
        t9552 = t708 * t9358
        t9554 = (t9550 - t9552) * t236
        t9555 = t9554 / 0.2E1
        t9557 = t7220 / 0.2E1 + t1201 / 0.2E1
        t9559 = t3811 * t9557
        t9561 = (t9552 - t9559) * t236
        t9562 = t9561 / 0.2E1
        t9563 = t4112 * t1141
        t9564 = t4121 * t1144
        t9566 = (t9563 - t9564) * t236
        t9567 = t9512 + t9363 + t9519 + t9375 + t9524 + t9525 + t1125 + 
     #t6977 + t9526 + t1153 + t9537 + t9546 + t9555 + t9562 + t9566
        t9568 = t9567 * t631
        t9569 = t9568 - t1216
        t9570 = t9569 * t183
        t9571 = t4159 * t1127
        t9573 = (t9427 - t9571) * t94
        t9575 = t1092 / 0.2E1 + t7156 / 0.2E1
        t9577 = t3877 * t9575
        t9579 = (t9444 - t9577) * t94
        t9580 = t9579 / 0.2E1
        t9582 = t3893 * t9117
        t9584 = (t9456 - t9582) * t94
        t9585 = t9584 / 0.2E1
        t9586 = t6947 / 0.2E1
        t9587 = t7019 / 0.2E1
        t9589 = (t1154 - t9110) * t94
        t9591 = t9465 / 0.2E1 + t9589 / 0.2E1
        t9593 = t4000 * t9591
        t9595 = t3481 * t1129
        t9597 = (t9593 - t9595) * t236
        t9598 = t9597 / 0.2E1
        t9600 = (t1157 - t9113) * t94
        t9602 = t9478 / 0.2E1 + t9600 / 0.2E1
        t9604 = t4037 * t9602
        t9606 = (t9595 - t9604) * t236
        t9607 = t9606 / 0.2E1
        t9609 = t1190 / 0.2E1 + t7206 / 0.2E1
        t9611 = t4050 * t9609
        t9613 = t731 * t9442
        t9615 = (t9611 - t9613) * t236
        t9616 = t9615 / 0.2E1
        t9618 = t1203 / 0.2E1 + t7225 / 0.2E1
        t9620 = t4061 * t9618
        t9622 = (t9613 - t9620) * t236
        t9623 = t9622 / 0.2E1
        t9624 = t4376 * t1156
        t9625 = t4385 * t1159
        t9627 = (t9624 - t9625) * t236
        t9628 = t9573 + t9447 + t9580 + t9459 + t9585 + t1134 + t9586 + 
     #t6982 + t1166 + t9587 + t9598 + t9607 + t9616 + t9623 + t9627
        t9629 = t9628 * t672
        t9630 = t1216 - t9629
        t9631 = t9630 * t183
        t9633 = (t9423 - t1083) * t183 / 0.4E1 + (t1083 - t9507) * t183 
     #/ 0.4E1 + t9570 / 0.4E1 + t9631 / 0.4E1
        t9639 = dx * (t921 / 0.2E1 - t1098 / 0.2E1)
        t9643 = t9272 * t9202 * t9331
        t9646 = t9336 * t9216 * t9338 / 0.2E1
        t9649 = t9336 * t9221 * t9633 / 0.6E1
        t9651 = t9202 * t9639 / 0.24E2
        t9653 = (t9272 * t136 * t9331 + t9336 * t2076 * t9338 / 0.2E1 + 
     #t9336 * t2917 * t9633 / 0.6E1 - t136 * t9639 / 0.24E2 - t9643 - t9
     #646 - t9649 + t9651) * t133
        t9660 = t1330 * (t1864 / 0.2E1 + t1871 / 0.2E1)
        t9662 = t218 / 0.4E1
        t9663 = t221 / 0.4E1
        t9666 = t1330 * (t6413 / 0.2E1 + t6418 / 0.2E1)
        t9667 = t9666 / 0.12E2
        t9673 = (t184 - t188) * t183
        t9684 = t200 / 0.2E1
        t9685 = t203 / 0.2E1
        t9686 = t9660 / 0.6E1
        t9689 = t218 / 0.2E1
        t9690 = t221 / 0.2E1
        t9691 = t9666 / 0.6E1
        t9692 = t582 / 0.2E1
        t9693 = t585 / 0.2E1
        t9697 = (t582 - t585) * t183
        t9699 = ((t3906 - t582) * t183 - t9697) * t183
        t9703 = (t9697 - (t585 - t4170) * t183) * t183
        t9706 = t1330 * (t9699 / 0.2E1 + t9703 / 0.2E1)
        t9707 = t9706 / 0.6E1
        t9715 = t9272 * (t200 / 0.4E1 + t203 / 0.4E1 - t9660 / 0.12E2 + 
     #t9662 + t9663 - t9667 - dx * ((t184 / 0.2E1 + t188 / 0.2E1 - t1330
     # * (((t2017 - t184) * t183 - t9673) * t183 / 0.2E1 + (t9673 - (t18
     #8 - t2022) * t183) * t183 / 0.2E1) / 0.6E1 - t9684 - t9685 + t9686
     #) * t94 / 0.2E1 - (t9689 + t9690 - t9691 - t9692 - t9693 + t9707) 
     #* t94 / 0.2E1) / 0.8E1)
        t9719 = dx * (t209 / 0.2E1 - t591 / 0.2E1) / 0.24E2
        t9724 = t28 * t249
        t9726 = t57 * t266
        t9727 = t9726 / 0.2E1
        t9731 = t119 * t596
        t9739 = t4 * (t9724 / 0.2E1 + t9727 - dx * ((t87 * t232 - t9724)
     # * t94 / 0.2E1 - (t9726 - t9731) * t94 / 0.2E1) / 0.8E1)
        t9744 = t1429 * (t2490 / 0.2E1 + t2495 / 0.2E1)
        t9746 = t961 / 0.4E1
        t9747 = t964 / 0.4E1
        t9750 = t1429 * (t6900 / 0.2E1 + t6905 / 0.2E1)
        t9751 = t9750 / 0.12E2
        t9757 = (t938 - t941) * t236
        t9768 = t948 / 0.2E1
        t9769 = t951 / 0.2E1
        t9770 = t9744 / 0.6E1
        t9773 = t961 / 0.2E1
        t9774 = t964 / 0.2E1
        t9775 = t9750 / 0.6E1
        t9776 = t1102 / 0.2E1
        t9777 = t1105 / 0.2E1
        t9781 = (t1102 - t1105) * t236
        t9783 = ((t6855 - t1102) * t236 - t9781) * t236
        t9787 = (t9781 - (t1105 - t6861) * t236) * t236
        t9790 = t1429 * (t9783 / 0.2E1 + t9787 / 0.2E1)
        t9791 = t9790 / 0.6E1
        t9798 = t948 / 0.4E1 + t951 / 0.4E1 - t9744 / 0.12E2 + t9746 + t
     #9747 - t9751 - dx * ((t938 / 0.2E1 + t941 / 0.2E1 - t1429 * (((t22
     #09 - t938) * t236 - t9757) * t236 / 0.2E1 + (t9757 - (t941 - t2215
     #) * t236) * t236 / 0.2E1) / 0.6E1 - t9768 - t9769 + t9770) * t94 /
     # 0.2E1 - (t9773 + t9774 - t9775 - t9776 - t9777 + t9791) * t94 / 0
     #.2E1) / 0.8E1
        t9803 = t4 * (t9724 / 0.2E1 + t9726 / 0.2E1)
        t9805 = t4973 / 0.4E1 + t5067 / 0.4E1 + t5272 / 0.4E1 + t5470 / 
     #0.4E1
        t9810 = t4887 * t1033
        t9812 = (t4444 * t1031 - t9810) * t94
        t9816 = t4147 * t1059
        t9818 = (t4141 * t6371 - t9816) * t94
        t9821 = t4664 * t1192
        t9823 = (t9816 - t9821) * t94
        t9824 = t9823 / 0.2E1
        t9830 = t2225 / 0.2E1 + t948 / 0.2E1
        t9832 = t452 * t9830
        t9834 = (t2608 * (t2209 / 0.2E1 + t938 / 0.2E1) - t9832) * t94
        t9837 = t2243 / 0.2E1 + t961 / 0.2E1
        t9839 = t771 * t9837
        t9841 = (t9832 - t9839) * t94
        t9842 = t9841 / 0.2E1
        t9846 = t4147 * t1035
        t9860 = t2384 / 0.2E1 + t1004 / 0.2E1
        t9864 = t507 * t9830
        t9869 = t2403 / 0.2E1 + t1019 / 0.2E1
        t9877 = t9812 + t9818 / 0.2E1 + t9824 + t9834 / 0.2E1 + t9842 + 
     #(t4678 * t9383 - t9846) * t183 / 0.2E1 + (t9846 - t4688 * t9467) *
     # t183 / 0.2E1 + (t4939 * t1055 - t4948 * t1057) * t183 + (t3437 * 
     #t9860 - t9864) * t183 / 0.2E1 + (t9864 - t3548 * t9869) * t183 / 0
     #.2E1 + t2460 / 0.2E1 + t1042 + t2274 / 0.2E1 + t1066 + t2501
        t9878 = t9877 * t448
        t9882 = t4981 * t1046
        t9884 = (t4682 * t1044 - t9882) * t94
        t9888 = t4462 * t1072
        t9890 = (t4458 * t6384 - t9888) * t94
        t9893 = t4720 * t1205
        t9895 = (t9888 - t9893) * t94
        t9896 = t9895 / 0.2E1
        t9902 = t951 / 0.2E1 + t2231 / 0.2E1
        t9904 = t492 * t9902
        t9906 = (t2630 * (t941 / 0.2E1 + t2215 / 0.2E1) - t9904) * t94
        t9909 = t964 / 0.2E1 + t2249 / 0.2E1
        t9911 = t809 * t9909
        t9913 = (t9904 - t9911) * t94
        t9914 = t9913 / 0.2E1
        t9918 = t4462 * t1048
        t9932 = t1007 / 0.2E1 + t2389 / 0.2E1
        t9936 = t521 * t9902
        t9941 = t1022 / 0.2E1 + t2408 / 0.2E1
        t9949 = t9884 + t9890 / 0.2E1 + t9896 + t9906 / 0.2E1 + t9914 + 
     #(t4728 * t9396 - t9918) * t183 / 0.2E1 + (t9918 - t4737 * t9480) *
     # t183 / 0.2E1 + (t5033 * t1068 - t5042 * t1070) * t183 + (t3450 * 
     #t9932 - t9936) * t183 / 0.2E1 + (t9936 - t3562 * t9941) * t183 / 0
     #.2E1 + t1053 + t2476 / 0.2E1 + t1077 + t2292 / 0.2E1 + t2506
        t9950 = t9949 * t489
        t9953 = t5105 * t1168
        t9955 = (t9810 - t9953) * t94
        t9957 = t4798 * t9144
        t9959 = (t9821 - t9957) * t94
        t9960 = t9959 / 0.2E1
        t9962 = t6855 / 0.2E1 + t1102 / 0.2E1
        t9964 = t4807 * t9962
        t9966 = (t9839 - t9964) * t94
        t9967 = t9966 / 0.2E1
        t9969 = t4817 * t9530
        t9971 = t4664 * t1170
        t9973 = (t9969 - t9971) * t183
        t9974 = t9973 / 0.2E1
        t9976 = t4826 * t9591
        t9978 = (t9971 - t9976) * t183
        t9979 = t9978 / 0.2E1
        t9980 = t5171 * t1188
        t9981 = t5180 * t1190
        t9983 = (t9980 - t9981) * t183
        t9985 = t7258 / 0.2E1 + t1141 / 0.2E1
        t9987 = t3800 * t9985
        t9989 = t822 * t9837
        t9991 = (t9987 - t9989) * t183
        t9992 = t9991 / 0.2E1
        t9994 = t7277 / 0.2E1 + t1156 / 0.2E1
        t9996 = t4050 * t9994
        t9998 = (t9989 - t9996) * t183
        t9999 = t9998 / 0.2E1
        t10000 = t7177 / 0.2E1
        t10001 = t7123 / 0.2E1
        t10002 = t9955 + t9824 + t9960 + t9842 + t9967 + t9974 + t9979 +
     # t9983 + t9992 + t9999 + t10000 + t1177 + t10001 + t1199 + t6911
        t10003 = t10002 * t774
        t10004 = t10003 - t1216
        t10005 = t10004 * t236
        t10006 = t5303 * t1179
        t10008 = (t9882 - t10006) * t94
        t10010 = t4925 * t9157
        t10012 = (t9893 - t10010) * t94
        t10013 = t10012 / 0.2E1
        t10015 = t1105 / 0.2E1 + t6861 / 0.2E1
        t10017 = t4947 * t10015
        t10019 = (t9911 - t10017) * t94
        t10020 = t10019 / 0.2E1
        t10022 = t4956 * t9541
        t10024 = t4720 * t1181
        t10026 = (t10022 - t10024) * t183
        t10027 = t10026 / 0.2E1
        t10029 = t4965 * t9602
        t10031 = (t10024 - t10029) * t183
        t10032 = t10031 / 0.2E1
        t10033 = t5369 * t1201
        t10034 = t5378 * t1203
        t10036 = (t10033 - t10034) * t183
        t10038 = t1144 / 0.2E1 + t7263 / 0.2E1
        t10040 = t3811 * t10038
        t10042 = t837 * t9909
        t10044 = (t10040 - t10042) * t183
        t10045 = t10044 / 0.2E1
        t10047 = t1159 / 0.2E1 + t7282 / 0.2E1
        t10049 = t4061 * t10047
        t10051 = (t10042 - t10049) * t183
        t10052 = t10051 / 0.2E1
        t10053 = t7191 / 0.2E1
        t10054 = t7141 / 0.2E1
        t10055 = t10008 + t9896 + t10013 + t9914 + t10020 + t10027 + t10
     #032 + t10036 + t10045 + t10052 + t1186 + t10053 + t1210 + t10054 +
     # t6916
        t10056 = t10055 * t813
        t10057 = t1216 - t10056
        t10058 = t10057 * t236
        t10060 = (t9878 - t1083) * t236 / 0.4E1 + (t1083 - t9950) * t236
     # / 0.4E1 + t10005 / 0.4E1 + t10058 / 0.4E1
        t10066 = dx * (t957 / 0.2E1 - t1111 / 0.2E1)
        t10070 = t9739 * t9202 * t9798
        t10073 = t9803 * t9216 * t9805 / 0.2E1
        t10076 = t9803 * t9221 * t10060 / 0.6E1
        t10078 = t9202 * t10066 / 0.24E2
        t10080 = (t9739 * t136 * t9798 + t9803 * t2076 * t9805 / 0.2E1 +
     # t9803 * t2917 * t10060 / 0.6E1 - t136 * t10066 / 0.24E2 - t10070 
     #- t10073 - t10076 + t10078) * t133
        t10087 = t1429 * (t1439 / 0.2E1 + t1448 / 0.2E1)
        t10089 = t269 / 0.4E1
        t10090 = t272 / 0.4E1
        t10093 = t1429 * (t6508 / 0.2E1 + t6513 / 0.2E1)
        t10094 = t10093 / 0.12E2
        t10100 = (t237 - t241) * t236
        t10111 = t252 / 0.2E1
        t10112 = t255 / 0.2E1
        t10113 = t10087 / 0.6E1
        t10116 = t269 / 0.2E1
        t10117 = t272 / 0.2E1
        t10118 = t10093 / 0.6E1
        t10119 = t599 / 0.2E1
        t10120 = t602 / 0.2E1
        t10124 = (t599 - t602) * t236
        t10126 = ((t5131 - t599) * t236 - t10124) * t236
        t10130 = (t10124 - (t602 - t5329) * t236) * t236
        t10133 = t1429 * (t10126 / 0.2E1 + t10130 / 0.2E1)
        t10134 = t10133 / 0.6E1
        t10142 = t9739 * (t252 / 0.4E1 + t255 / 0.4E1 - t10087 / 0.12E2 
     #+ t10089 + t10090 - t10094 - dx * ((t237 / 0.2E1 + t241 / 0.2E1 - 
     #t1429 * (((t1684 - t237) * t236 - t10100) * t236 / 0.2E1 + (t10100
     # - (t241 - t1690) * t236) * t236 / 0.2E1) / 0.6E1 - t10111 - t1011
     #2 + t10113) * t94 / 0.2E1 - (t10116 + t10117 - t10118 - t10119 - t
     #10120 + t10134) * t94 / 0.2E1) / 0.8E1)
        t10146 = dx * (t261 / 0.2E1 - t608 / 0.2E1) / 0.24E2
        t10153 = t147 - dx * t6844 / 0.24E2
        t10158 = t161 * t7433 * t94
        t10163 = t895 * t9169 * t94
        t10166 = dx * t7041
        t10169 = cc * t6615
        t10189 = (t7312 - t7319) * t183
        t10212 = t6863 * t6349
        t10234 = (t7359 - t7368) * t183
        t10261 = i - 3
        t10262 = u(t10261,j,k,n)
        t10264 = (t6543 - t10262) * t94
        t10272 = rx(t10261,j,k,0,0)
        t10273 = rx(t10261,j,k,1,1)
        t10275 = rx(t10261,j,k,2,2)
        t10277 = rx(t10261,j,k,1,2)
        t10279 = rx(t10261,j,k,2,1)
        t10281 = rx(t10261,j,k,1,0)
        t10283 = rx(t10261,j,k,0,2)
        t10285 = rx(t10261,j,k,0,1)
        t10288 = rx(t10261,j,k,2,0)
        t10294 = 0.1E1 / (t10272 * t10273 * t10275 - t10272 * t10277 * t
     #10279 + t10281 * t10279 * t10283 - t10281 * t10285 * t10275 + t102
     #88 * t10285 * t10277 - t10288 * t10273 * t10283)
        t10295 = t10272 ** 2
        t10296 = t10285 ** 2
        t10297 = t10283 ** 2
        t10299 = t10294 * (t10295 + t10296 + t10297)
        t10302 = t4 * (t6580 / 0.2E1 + t10299 / 0.2E1)
        t10305 = (t6584 - t10302 * t10264) * t94
        t10314 = t7330 / 0.2E1
        t10324 = t4 * (t7325 / 0.2E1 + t10314 - dy * ((t7556 - t7325) * 
     #t183 / 0.2E1 - (t7330 - t7339) * t183 / 0.2E1) / 0.8E1)
        t10336 = t4 * (t10314 + t7339 / 0.2E1 - dy * ((t7325 - t7330) * 
     #t183 / 0.2E1 - (t7339 - t7820) * t183 / 0.2E1) / 0.8E1)
        t10340 = u(t10261,t180,k,n)
        t10342 = (t6646 - t10340) * t94
        t9621 = (t6658 - (t571 / 0.2E1 - t10264 / 0.2E1) * t94) * t94
        t10356 = t573 * t9621
        t10359 = u(t10261,t185,k,n)
        t10361 = (t6665 - t10359) * t94
        t10409 = t4 * (t6608 + t6580 / 0.2E1 - dx * (t126 / 0.2E1 - (t65
     #80 - t10299) * t94 / 0.2E1) / 0.8E1)
        t10413 = u(t10261,j,t233,n)
        t10415 = (t6625 - t10413) * t94
        t10425 = t590 * t9621
        t10428 = u(t10261,j,t238,n)
        t10430 = (t6628 - t10428) * t94
        t9682 = ((t8095 / 0.2E1 - t3923 / 0.2E1) * t236 - (t3920 / 0.2E1
     # - t8293 / 0.2E1) * t236) * t236
        t9694 = ((t8107 / 0.2E1 - t4187 / 0.2E1) * t236 - (t4184 / 0.2E1
     # - t8305 / 0.2E1) * t236) * t236
        t10444 = -t1330 * (t6537 / 0.2E1 + (t6535 - t6339 * ((t7490 / 0.
     #2E1 - t6719 / 0.2E1) * t183 - (t6717 / 0.2E1 - t7754 / 0.2E1) * t1
     #83) * t183) * t94 / 0.2E1) / 0.6E1 - t1330 * (((t7550 - t7312) * t
     #183 - t10189) * t183 / 0.2E1 + (t10189 - (t7319 - t7814) * t183) *
     # t183 / 0.2E1) / 0.6E1 - t1429 * ((t6856 * t9682 - t10212) * t183 
     #/ 0.2E1 + (t10212 - t6872 * t9694) * t183 / 0.2E1) / 0.6E1 - t1330
     # * (((t7578 - t7359) * t183 - t10234) * t183 / 0.2E1 + (t10234 - (
     #t7368 - t7842) * t183) * t183 / 0.2E1) / 0.6E1 - t1429 * (t6745 / 
     #0.2E1 + (t6743 - t6244 * ((t8040 / 0.2E1 - t6630 / 0.2E1) * t236 -
     # (t6627 / 0.2E1 - t8238 / 0.2E1) * t236) * t236) * t94 / 0.2E1) / 
     #0.6E1 - t1227 * ((t6550 - t6583 * (t6547 - (t6545 - t10264) * t94)
     # * t94) * t94 + (t6588 - (t6586 - t10305) * t94) * t94) / 0.24E2 +
     # (t10324 * t582 - t10336 * t585) * t183 - t1227 * ((t3630 * (t6651
     # - (t639 / 0.2E1 - t10342 / 0.2E1) * t94) * t94 - t10356) * t183 /
     # 0.2E1 + (t10356 - t3877 * (t6670 - (t680 / 0.2E1 - t10361 / 0.2E1
     #) * t94) * t94) * t183 / 0.2E1) / 0.6E1 - t1429 * ((t7418 * t10126
     # - t7427 * t10130) * t236 + ((t8177 - t7430) * t236 - (t7430 - t83
     #75) * t236) * t236) / 0.24E2 + t609 - t1330 * ((t7333 * t9699 - t7
     #342 * t9703) * t183 + ((t7562 - t7345) * t183 - (t7345 - t7826) * 
     #t183) * t183) / 0.24E2 + (t6617 - t10409 * t6545) * t94 - t1227 * 
     #((t4807 * (t6795 - (t782 / 0.2E1 - t10415 / 0.2E1) * t94) * t94 - 
     #t10425) * t236 / 0.2E1 + (t10425 - t4947 * (t6809 - (t821 / 0.2E1 
     #- t10430 / 0.2E1) * t94) * t94) * t236 / 0.2E1) / 0.6E1 + t7396 + 
     #t7405
        t10448 = (t7377 - t7384) * t236
        t10471 = t6863 * t6197
        t10490 = t4 * t10294
        t10496 = (t10413 - t10262) * t236
        t10498 = (t10262 - t10428) * t236
        t9892 = t10490 * (t10272 * t10288 + t10285 * t10279 + t10283 * t
     #10275)
        t10504 = (t6634 - t9892 * (t10496 / 0.2E1 + t10498 / 0.2E1)) * t
     #94
        t10518 = (t10340 - t10262) * t183
        t10520 = (t10262 - t10359) * t183
        t9908 = t10490 * (t10272 * t10281 + t10285 * t10273 + t10283 * t
     #10277)
        t10526 = (t6723 - t9908 * (t10518 / 0.2E1 + t10520 / 0.2E1)) * t
     #94
        t10536 = t7415 / 0.2E1
        t10546 = t4 * (t7410 / 0.2E1 + t10536 - dz * ((t8171 - t7410) * 
     #t236 / 0.2E1 - (t7415 - t7424) * t236 / 0.2E1) / 0.8E1)
        t10558 = t4 * (t10536 + t7424 / 0.2E1 - dz * ((t7410 - t7415) * 
     #t236 / 0.2E1 - (t7424 - t8369) * t236 / 0.2E1) / 0.8E1)
        t10565 = (t7395 - t7404) * t236
        t9961 = ((t7661 / 0.2E1 - t5117 / 0.2E1) * t183 - (t5115 / 0.2E1
     # - t7925 / 0.2E1) * t183) * t183
        t9970 = ((t7676 / 0.2E1 - t5315 / 0.2E1) * t183 - (t5313 / 0.2E1
     # - t7940 / 0.2E1) * t183) * t183
        t10576 = -t1429 * (((t8150 - t7377) * t236 - t10448) * t236 / 0.
     #2E1 + (t10448 - (t7384 - t8348) * t236) * t236 / 0.2E1) / 0.6E1 - 
     #t1330 * ((t6889 * t9961 - t10471) * t236 / 0.2E1 + (t10471 - t6895
     # * t9970) * t236 / 0.2E1) / 0.6E1 + t7360 + t7369 + t7378 + t7385 
     #+ t7302 + t7301 + t592 - t1227 * (t6640 / 0.2E1 + (t6638 - (t6636 
     #- t10504) * t94) * t94 / 0.2E1) / 0.6E1 + t7313 + t7320 - t1227 * 
     #(t6729 / 0.2E1 + (t6727 - (t6725 - t10526) * t94) * t94 / 0.2E1) /
     # 0.6E1 + (t10546 * t599 - t10558 * t602) * t236 - t1429 * (((t8165
     # - t7395) * t236 - t10565) * t236 / 0.2E1 + (t10565 - (t7404 - t83
     #63) * t236) * t236 / 0.2E1) / 0.6E1
        t10578 = (t10444 + t10576) * t118
        t10581 = ut(t10261,j,k,n)
        t10583 = (t6839 - t10581) * t94
        t10587 = (t6843 - (t6841 - t10583) * t94) * t94
        t10594 = dx * (t6838 + t6841 / 0.2E1 - t1227 * (t6845 / 0.2E1 + 
     #t10587 / 0.2E1) / 0.6E1) / 0.2E1
        t10595 = ut(t10261,t180,k,n)
        t10598 = ut(t10261,t185,k,n)
        t10606 = (t7246 - t9908 * ((t10595 - t10581) * t183 / 0.2E1 + (t
     #10581 - t10598) * t183 / 0.2E1)) * t94
        t10621 = (t8174 * t6855 - t9163) * t236
        t10626 = (t9164 - t8372 * t6861) * t236
        t10637 = ut(t96,t180,t1430,n)
        t10640 = ut(t96,t185,t1430,n)
        t10644 = (t10637 - t6853) * t183 / 0.2E1 + (t6853 - t10640) * t1
     #83 / 0.2E1
        t10648 = (t7599 * t10644 - t9146) * t236
        t10652 = (t9150 - t9161) * t236
        t10655 = ut(t96,t180,t1441,n)
        t10658 = ut(t96,t185,t1441,n)
        t10662 = (t10655 - t6859) * t183 / 0.2E1 + (t6859 - t10658) * t1
     #83 / 0.2E1
        t10666 = (t9159 - t7787 * t10662) * t236
        t10675 = ut(t6542,t1331,k,n)
        t10677 = (t6924 - t10675) * t94
        t10683 = (t7034 * (t6926 / 0.2E1 + t10677 / 0.2E1) - t9076) * t1
     #83
        t10687 = (t9082 - t9089) * t183
        t10690 = ut(t6542,t1379,k,n)
        t10692 = (t6939 - t10690) * t94
        t10698 = (t9087 - t7254 * (t6941 / 0.2E1 + t10692 / 0.2E1)) * t1
     #83
        t10707 = ut(t6542,j,t1430,n)
        t10709 = (t6853 - t10707) * t94
        t10715 = (t7586 * (t7171 / 0.2E1 + t10709 / 0.2E1) - t9126) * t2
     #36
        t10719 = (t9130 - t9137) * t236
        t10722 = ut(t6542,j,t1441,n)
        t10724 = (t6859 - t10722) * t94
        t10730 = (t9135 - t7767 * (t7185 / 0.2E1 + t10724 / 0.2E1)) * t2
     #36
        t10739 = -t1227 * (t7252 / 0.2E1 + (t7250 - (t7248 - t10606) * t
     #94) * t94 / 0.2E1) / 0.6E1 + t9131 + t1099 + t1112 - t1429 * ((t74
     #18 * t9783 - t7427 * t9787) * t236 + ((t10621 - t9166) * t236 - (t
     #9166 - t10626) * t236) * t236) / 0.24E2 + (t7032 - t10409 * t6841)
     # * t94 + t9072 + t9090 + t9109 - t1429 * (((t10648 - t9150) * t236
     # - t10652) * t236 / 0.2E1 + (t10652 - (t9161 - t10666) * t236) * t
     #236 / 0.2E1) / 0.6E1 - t1330 * (((t10683 - t9082) * t183 - t10687)
     # * t183 / 0.2E1 + (t10687 - (t9089 - t10698) * t183) * t183 / 0.2E
     #1) / 0.6E1 + t9122 + t9162 - t1429 * (((t10715 - t9130) * t236 - t
     #10719) * t236 / 0.2E1 + (t10719 - (t9137 - t10730) * t236) * t236 
     #/ 0.2E1) / 0.6E1 + t9138
        t10740 = ut(t96,t1331,t233,n)
        t10742 = (t10740 - t9095) * t183
        t10746 = ut(t96,t1379,t233,n)
        t10748 = (t9110 - t10746) * t183
        t10758 = t6863 * t6650
        t10761 = ut(t96,t1331,t238,n)
        t10763 = (t10761 - t9098) * t183
        t10767 = ut(t96,t1379,t238,n)
        t10769 = (t9113 - t10767) * t183
        t10787 = ut(t10261,j,t233,n)
        t10789 = (t6875 - t10787) * t94
        t10247 = (t7060 - (t147 / 0.2E1 - t10583 / 0.2E1) * t94) * t94
        t10803 = t590 * t10247
        t10806 = ut(t10261,j,t238,n)
        t10808 = (t6878 - t10806) * t94
        t10823 = (t10675 - t7048) * t183
        t10828 = (t7067 - t10690) * t183
        t10851 = (t7038 - t10302 * t10583) * t94
        t10860 = (t10707 - t6875) * t236
        t10865 = (t6878 - t10722) * t236
        t10888 = (t6884 - t9892 * ((t10787 - t10581) * t236 / 0.2E1 + (t
     #10581 - t10806) * t236 / 0.2E1)) * t94
        t10898 = (t7048 - t10595) * t94
        t10908 = t573 * t10247
        t10912 = (t7067 - t10598) * t94
        t10932 = (t7559 * t7151 - t9091) * t183
        t10937 = (t9092 - t7823 * t7156) * t183
        t10946 = (t10637 - t9095) * t236
        t10951 = (t9098 - t10655) * t236
        t10961 = t6863 * t6460
        t10965 = (t10640 - t9110) * t236
        t10970 = (t9113 - t10658) * t236
        t10989 = (t10740 - t6924) * t236 / 0.2E1 + (t6924 - t10761) * t2
     #36 / 0.2E1
        t10993 = (t7055 * t10989 - t9104) * t183
        t10997 = (t9108 - t9121) * t183
        t11005 = (t10746 - t6939) * t236 / 0.2E1 + (t6939 - t10767) * t2
     #36 / 0.2E1
        t11009 = (t9119 - t7272 * t11005) * t183
        t10320 = ((t10742 / 0.2E1 - t9142 / 0.2E1) * t183 - (t9140 / 0.2
     #E1 - t10748 / 0.2E1) * t183) * t183
        t10325 = ((t10763 / 0.2E1 - t9155 / 0.2E1) * t183 - (t9153 / 0.2
     #E1 - t10769 / 0.2E1) * t183) * t183
        t10457 = ((t10946 / 0.2E1 - t9100 / 0.2E1) * t236 - (t9097 / 0.2
     #E1 - t10951 / 0.2E1) * t236) * t236
        t10461 = ((t10965 / 0.2E1 - t9115 / 0.2E1) * t236 - (t9112 / 0.2
     #E1 - t10970 / 0.2E1) * t236) * t236
        t11018 = t9151 - t1330 * ((t6889 * t10320 - t10758) * t236 / 0.2
     #E1 + (t10758 - t6895 * t10325) * t236 / 0.2E1) / 0.6E1 + (t10546 *
     # t1102 - t10558 * t1105) * t236 - t1227 * ((t4807 * (t7087 - (t116
     #8 / 0.2E1 - t10789 / 0.2E1) * t94) * t94 - t10803) * t236 / 0.2E1 
     #+ (t10803 - t4947 * (t7101 - (t1179 / 0.2E1 - t10808 / 0.2E1) * t9
     #4) * t94) * t236 / 0.2E1) / 0.6E1 - t1330 * (t7165 / 0.2E1 + (t716
     #3 - t6339 * ((t10823 / 0.2E1 - t7242 / 0.2E1) * t183 - (t7240 / 0.
     #2E1 - t10828 / 0.2E1) * t183) * t183) * t94 / 0.2E1) / 0.6E1 + (t1
     #0324 * t1089 - t10336 * t1092) * t183 - t1227 * ((t7035 - t6583 * 
     #t10587) * t94 + (t7042 - (t7040 - t10851) * t94) * t94) / 0.24E2 -
     # t1429 * (t6870 / 0.2E1 + (t6868 - t6244 * ((t10860 / 0.2E1 - t688
     #0 / 0.2E1) * t236 - (t6877 / 0.2E1 - t10865 / 0.2E1) * t236) * t23
     #6) * t94 / 0.2E1) / 0.6E1 - t1227 * (t6890 / 0.2E1 + (t6888 - (t68
     #86 - t10888) * t94) * t94 / 0.2E1) / 0.6E1 - t1227 * ((t3630 * (t7
     #053 - (t1114 / 0.2E1 - t10898 / 0.2E1) * t94) * t94 - t10908) * t1
     #83 / 0.2E1 + (t10908 - t3877 * (t7072 - (t1127 / 0.2E1 - t10912 / 
     #0.2E1) * t94) * t94) * t183 / 0.2E1) / 0.6E1 - t1330 * ((t7333 * t
     #9316 - t7342 * t9320) * t183 + ((t10932 - t9094) * t183 - (t9094 -
     # t10937) * t183) * t183) / 0.24E2 - t1429 * ((t6856 * t10457 - t10
     #961) * t183 / 0.2E1 + (t10961 - t6872 * t10461) * t183 / 0.2E1) / 
     #0.6E1 - t1330 * (((t10993 - t9108) * t183 - t10997) * t183 / 0.2E1
     # + (t10997 - (t9121 - t11009) * t183) * t183 / 0.2E1) / 0.6E1 + t9
     #083 + t9071
        t11020 = (t10739 + t11018) * t118
        t11026 = t6648 / 0.2E1 + t10342 / 0.2E1
        t11028 = t6985 * t11026
        t11030 = t6545 / 0.2E1 + t10264 / 0.2E1
        t11032 = t6339 * t11030
        t11035 = (t11028 - t11032) * t183 / 0.2E1
        t11037 = t6667 / 0.2E1 + t10361 / 0.2E1
        t11039 = t7208 * t11037
        t11042 = (t11032 - t11039) * t183 / 0.2E1
        t11043 = t7458 ** 2
        t11044 = t7450 ** 2
        t11045 = t7454 ** 2
        t11047 = t7471 * (t11043 + t11044 + t11045)
        t11048 = t6562 ** 2
        t11049 = t6554 ** 2
        t11050 = t6558 ** 2
        t11052 = t6575 * (t11048 + t11049 + t11050)
        t11055 = t4 * (t11047 / 0.2E1 + t11052 / 0.2E1)
        t11056 = t11055 * t6717
        t11057 = t7722 ** 2
        t11058 = t7714 ** 2
        t11059 = t7718 ** 2
        t11061 = t7735 * (t11057 + t11058 + t11059)
        t11064 = t4 * (t11052 / 0.2E1 + t11061 / 0.2E1)
        t11065 = t11064 * t6719
        t10507 = t7483 * (t7458 * t7465 + t7450 * t7456 + t7454 * t7452)
        t11073 = t10507 * t7509
        t10511 = t6620 * (t6562 * t6569 + t6554 * t6560 + t6558 * t6556)
        t11079 = t10511 * t6632
        t11082 = (t11073 - t11079) * t183 / 0.2E1
        t10519 = t7747 * (t7722 * t7729 + t7714 * t7720 + t7718 * t7716)
        t11088 = t10519 * t7773
        t11091 = (t11079 - t11088) * t183 / 0.2E1
        t11093 = t6792 / 0.2E1 + t10415 / 0.2E1
        t11095 = t7486 * t11093
        t11097 = t6244 * t11030
        t11100 = (t11095 - t11097) * t236 / 0.2E1
        t11102 = t6806 / 0.2E1 + t10430 / 0.2E1
        t11104 = t7668 * t11102
        t11107 = (t11097 - t11104) * t236 / 0.2E1
        t10533 = t8018 * (t7993 * t8000 + t7985 * t7991 + t7989 * t7987)
        t11113 = t10533 * t8028
        t11115 = t10511 * t6721
        t11118 = (t11113 - t11115) * t236 / 0.2E1
        t10540 = t8216 * (t8191 * t8198 + t8183 * t8189 + t8187 * t8185)
        t11124 = t10540 * t8226
        t11127 = (t11115 - t11124) * t236 / 0.2E1
        t11128 = t8000 ** 2
        t11129 = t7991 ** 2
        t11130 = t7987 ** 2
        t11132 = t8006 * (t11128 + t11129 + t11130)
        t11133 = t6569 ** 2
        t11134 = t6560 ** 2
        t11135 = t6556 ** 2
        t11137 = t6575 * (t11133 + t11134 + t11135)
        t11140 = t4 * (t11132 / 0.2E1 + t11137 / 0.2E1)
        t11141 = t11140 * t6627
        t11142 = t8198 ** 2
        t11143 = t8189 ** 2
        t11144 = t8185 ** 2
        t11146 = t8204 * (t11142 + t11143 + t11144)
        t11149 = t4 * (t11137 / 0.2E1 + t11146 / 0.2E1)
        t11150 = t11149 * t6630
        t11153 = t10305 + t7301 + t10526 / 0.2E1 + t7302 + t10504 / 0.2E
     #1 + t11035 + t11042 + (t11056 - t11065) * t183 + t11082 + t11091 +
     # t11100 + t11107 + t11118 + t11127 + (t11141 - t11150) * t236
        t11154 = t11153 * t6574
        t11156 = (t7432 - t11154) * t94
        t11158 = t7434 / 0.2E1 + t11156 / 0.2E1
        t11159 = dx * t11158
        t11167 = t1227 * (t6843 - dx * (t6845 - t10587) / 0.12E2) / 0.12
     #E2
        t11171 = rx(t10261,t180,k,0,0)
        t11172 = rx(t10261,t180,k,1,1)
        t11174 = rx(t10261,t180,k,2,2)
        t11176 = rx(t10261,t180,k,1,2)
        t11178 = rx(t10261,t180,k,2,1)
        t11180 = rx(t10261,t180,k,1,0)
        t11182 = rx(t10261,t180,k,0,2)
        t11184 = rx(t10261,t180,k,0,1)
        t11187 = rx(t10261,t180,k,2,0)
        t11193 = 0.1E1 / (t11171 * t11172 * t11174 - t11171 * t11176 * t
     #11178 + t11180 * t11178 * t11182 - t11180 * t11184 * t11174 + t111
     #87 * t11184 * t11176 - t11187 * t11172 * t11182)
        t11194 = t11171 ** 2
        t11195 = t11184 ** 2
        t11196 = t11182 ** 2
        t11205 = t4 * t11193
        t11210 = u(t10261,t1331,k,n)
        t11224 = u(t10261,t180,t233,n)
        t11227 = u(t10261,t180,t238,n)
        t11237 = rx(t6542,t1331,k,0,0)
        t11238 = rx(t6542,t1331,k,1,1)
        t11240 = rx(t6542,t1331,k,2,2)
        t11242 = rx(t6542,t1331,k,1,2)
        t11244 = rx(t6542,t1331,k,2,1)
        t11246 = rx(t6542,t1331,k,1,0)
        t11248 = rx(t6542,t1331,k,0,2)
        t11250 = rx(t6542,t1331,k,0,1)
        t11253 = rx(t6542,t1331,k,2,0)
        t11259 = 0.1E1 / (t11237 * t11238 * t11240 - t11237 * t11242 * t
     #11244 + t11246 * t11244 * t11248 - t11246 * t11250 * t11240 + t112
     #53 * t11250 * t11242 - t11253 * t11238 * t11248)
        t11260 = t4 * t11259
        t11274 = t11246 ** 2
        t11275 = t11238 ** 2
        t11276 = t11242 ** 2
        t11289 = u(t6542,t1331,t233,n)
        t11292 = u(t6542,t1331,t238,n)
        t11296 = (t11289 - t7488) * t236 / 0.2E1 + (t7488 - t11292) * t2
     #36 / 0.2E1
        t11302 = rx(t6542,t180,t233,0,0)
        t11303 = rx(t6542,t180,t233,1,1)
        t11305 = rx(t6542,t180,t233,2,2)
        t11307 = rx(t6542,t180,t233,1,2)
        t11309 = rx(t6542,t180,t233,2,1)
        t11311 = rx(t6542,t180,t233,1,0)
        t11313 = rx(t6542,t180,t233,0,2)
        t11315 = rx(t6542,t180,t233,0,1)
        t11318 = rx(t6542,t180,t233,2,0)
        t11324 = 0.1E1 / (t11302 * t11303 * t11305 - t11302 * t11307 * t
     #11309 + t11311 * t11309 * t11313 - t11311 * t11315 * t11305 + t113
     #18 * t11315 * t11307 - t11318 * t11303 * t11313)
        t11325 = t4 * t11324
        t11333 = t7609 / 0.2E1 + (t7502 - t11224) * t94 / 0.2E1
        t11337 = t6998 * t11026
        t11341 = rx(t6542,t180,t238,0,0)
        t11342 = rx(t6542,t180,t238,1,1)
        t11344 = rx(t6542,t180,t238,2,2)
        t11346 = rx(t6542,t180,t238,1,2)
        t11348 = rx(t6542,t180,t238,2,1)
        t11350 = rx(t6542,t180,t238,1,0)
        t11352 = rx(t6542,t180,t238,0,2)
        t11354 = rx(t6542,t180,t238,0,1)
        t11357 = rx(t6542,t180,t238,2,0)
        t11363 = 0.1E1 / (t11341 * t11342 * t11344 - t11341 * t11346 * t
     #11348 + t11350 * t11348 * t11352 - t11350 * t11354 * t11344 + t113
     #57 * t11354 * t11346 - t11357 * t11342 * t11352)
        t11364 = t4 * t11363
        t11372 = t7648 / 0.2E1 + (t7505 - t11227) * t94 / 0.2E1
        t11385 = (t11289 - t7502) * t183 / 0.2E1 + t8024 / 0.2E1
        t11389 = t10507 * t7492
        t11400 = (t11292 - t7505) * t183 / 0.2E1 + t8222 / 0.2E1
        t11406 = t11318 ** 2
        t11407 = t11309 ** 2
        t11408 = t11305 ** 2
        t11411 = t7465 ** 2
        t11412 = t7456 ** 2
        t11413 = t7452 ** 2
        t11415 = t7471 * (t11411 + t11412 + t11413)
        t11420 = t11357 ** 2
        t11421 = t11348 ** 2
        t11422 = t11344 ** 2
        t10717 = t11260 * (t11237 * t11246 + t11250 * t11238 + t11248 * 
     #t11242)
        t10753 = t11325 * (t11302 * t11318 + t11315 * t11309 + t11313 * 
     #t11305)
        t10759 = t11364 * (t11341 * t11357 + t11354 * t11348 + t11352 * 
     #t11344)
        t10766 = t11325 * (t11311 * t11318 + t11303 * t11309 + t11307 * 
     #t11305)
        t10774 = t11364 * (t11350 * t11357 + t11342 * t11348 + t11346 * 
     #t11344)
        t11431 = (t7480 - t4 * (t7476 / 0.2E1 + t11193 * (t11194 + t1119
     #5 + t11196) / 0.2E1) * t10342) * t94 + t7497 + (t7494 - t11205 * (
     #t11171 * t11180 + t11184 * t11172 + t11182 * t11176) * ((t11210 - 
     #t10340) * t183 / 0.2E1 + t10518 / 0.2E1)) * t94 / 0.2E1 + t7514 + 
     #(t7511 - t11205 * (t11171 * t11187 + t11184 * t11178 + t11182 * t1
     #1174) * ((t11224 - t10340) * t236 / 0.2E1 + (t10340 - t11227) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + (t10717 * (t7544 / 0.2E1 + (t7488 - t
     #11210) * t94 / 0.2E1) - t11028) * t183 / 0.2E1 + t11035 + (t4 * (t
     #11259 * (t11274 + t11275 + t11276) / 0.2E1 + t11047 / 0.2E1) * t74
     #90 - t11056) * t183 + (t11260 * (t11246 * t11253 + t11238 * t11244
     # + t11242 * t11240) * t11296 - t11073) * t183 / 0.2E1 + t11082 + (
     #t10753 * t11333 - t11337) * t236 / 0.2E1 + (t11337 - t10759 * t113
     #72) * t236 / 0.2E1 + (t10766 * t11385 - t11389) * t236 / 0.2E1 + (
     #t11389 - t10774 * t11400) * t236 / 0.2E1 + (t4 * (t11324 * (t11406
     # + t11407 + t11408) / 0.2E1 + t11415 / 0.2E1) * t7504 - t4 * (t114
     #15 / 0.2E1 + t11363 * (t11420 + t11421 + t11422) / 0.2E1) * t7507)
     # * t236
        t11432 = t11431 * t7470
        t11435 = rx(t10261,t185,k,0,0)
        t11436 = rx(t10261,t185,k,1,1)
        t11438 = rx(t10261,t185,k,2,2)
        t11440 = rx(t10261,t185,k,1,2)
        t11442 = rx(t10261,t185,k,2,1)
        t11444 = rx(t10261,t185,k,1,0)
        t11446 = rx(t10261,t185,k,0,2)
        t11448 = rx(t10261,t185,k,0,1)
        t11451 = rx(t10261,t185,k,2,0)
        t11457 = 0.1E1 / (t11436 * t11435 * t11438 - t11435 * t11440 * t
     #11442 + t11444 * t11442 * t11446 - t11444 * t11448 * t11438 + t114
     #51 * t11448 * t11440 - t11451 * t11436 * t11446)
        t11458 = t11435 ** 2
        t11459 = t11448 ** 2
        t11460 = t11446 ** 2
        t11469 = t4 * t11457
        t11474 = u(t10261,t1379,k,n)
        t11488 = u(t10261,t185,t233,n)
        t11491 = u(t10261,t185,t238,n)
        t11501 = rx(t6542,t1379,k,0,0)
        t11502 = rx(t6542,t1379,k,1,1)
        t11504 = rx(t6542,t1379,k,2,2)
        t11506 = rx(t6542,t1379,k,1,2)
        t11508 = rx(t6542,t1379,k,2,1)
        t11510 = rx(t6542,t1379,k,1,0)
        t11512 = rx(t6542,t1379,k,0,2)
        t11514 = rx(t6542,t1379,k,0,1)
        t11517 = rx(t6542,t1379,k,2,0)
        t11523 = 0.1E1 / (t11501 * t11502 * t11504 - t11501 * t11506 * t
     #11508 + t11510 * t11508 * t11512 - t11510 * t11514 * t11504 + t115
     #17 * t11514 * t11506 - t11517 * t11502 * t11512)
        t11524 = t4 * t11523
        t11538 = t11510 ** 2
        t11539 = t11502 ** 2
        t11540 = t11506 ** 2
        t11553 = u(t6542,t1379,t233,n)
        t11556 = u(t6542,t1379,t238,n)
        t11560 = (t11553 - t7752) * t236 / 0.2E1 + (t7752 - t11556) * t2
     #36 / 0.2E1
        t11566 = rx(t6542,t185,t233,0,0)
        t11567 = rx(t6542,t185,t233,1,1)
        t11569 = rx(t6542,t185,t233,2,2)
        t11571 = rx(t6542,t185,t233,1,2)
        t11573 = rx(t6542,t185,t233,2,1)
        t11575 = rx(t6542,t185,t233,1,0)
        t11577 = rx(t6542,t185,t233,0,2)
        t11579 = rx(t6542,t185,t233,0,1)
        t11582 = rx(t6542,t185,t233,2,0)
        t11588 = 0.1E1 / (t11566 * t11567 * t11569 - t11566 * t11571 * t
     #11573 + t11575 * t11573 * t11577 - t11575 * t11579 * t11569 + t115
     #82 * t11579 * t11571 - t11582 * t11567 * t11577)
        t11589 = t4 * t11588
        t11597 = t7873 / 0.2E1 + (t7766 - t11488) * t94 / 0.2E1
        t11601 = t7222 * t11037
        t11605 = rx(t6542,t185,t238,0,0)
        t11606 = rx(t6542,t185,t238,1,1)
        t11608 = rx(t6542,t185,t238,2,2)
        t11610 = rx(t6542,t185,t238,1,2)
        t11612 = rx(t6542,t185,t238,2,1)
        t11614 = rx(t6542,t185,t238,1,0)
        t11616 = rx(t6542,t185,t238,0,2)
        t11618 = rx(t6542,t185,t238,0,1)
        t11621 = rx(t6542,t185,t238,2,0)
        t11627 = 0.1E1 / (t11605 * t11606 * t11608 - t11605 * t11610 * t
     #11612 + t11614 * t11612 * t11616 - t11614 * t11618 * t11608 + t116
     #21 * t11618 * t11610 - t11621 * t11606 * t11616)
        t11628 = t4 * t11627
        t11636 = t7912 / 0.2E1 + (t7769 - t11491) * t94 / 0.2E1
        t11649 = t8026 / 0.2E1 + (t7766 - t11553) * t183 / 0.2E1
        t11653 = t10519 * t7756
        t11664 = t8224 / 0.2E1 + (t7769 - t11556) * t183 / 0.2E1
        t11670 = t11582 ** 2
        t11671 = t11573 ** 2
        t11672 = t11569 ** 2
        t11675 = t7729 ** 2
        t11676 = t7720 ** 2
        t11677 = t7716 ** 2
        t11679 = t7735 * (t11675 + t11676 + t11677)
        t11684 = t11621 ** 2
        t11685 = t11612 ** 2
        t11686 = t11608 ** 2
        t10929 = t11524 * (t11501 * t11510 + t11514 * t11502 + t11512 * 
     #t11506)
        t10960 = t11589 * (t11566 * t11582 + t11579 * t11573 + t11577 * 
     #t11569)
        t10967 = t11628 * (t11605 * t11621 + t11618 * t11612 + t11616 * 
     #t11608)
        t10973 = t11589 * (t11575 * t11582 + t11567 * t11573 + t11571 * 
     #t11569)
        t10979 = t11628 * (t11614 * t11621 + t11606 * t11612 + t11610 * 
     #t11608)
        t11695 = (t7744 - t4 * (t7740 / 0.2E1 + t11457 * (t11458 + t1145
     #9 + t11460) / 0.2E1) * t10361) * t94 + t7761 + (t7758 - t11469 * (
     #t11435 * t11444 + t11448 * t11436 + t11446 * t11440) * (t10520 / 0
     #.2E1 + (t10359 - t11474) * t183 / 0.2E1)) * t94 / 0.2E1 + t7778 + 
     #(t7775 - t11469 * (t11435 * t11451 + t11448 * t11442 + t11446 * t1
     #1438) * ((t11488 - t10359) * t236 / 0.2E1 + (t10359 - t11491) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + t11042 + (t11039 - t10929 * (t7808 / 
     #0.2E1 + (t7752 - t11474) * t94 / 0.2E1)) * t183 / 0.2E1 + (t11065 
     #- t4 * (t11061 / 0.2E1 + t11523 * (t11538 + t11539 + t11540) / 0.2
     #E1) * t7754) * t183 + t11091 + (t11088 - t11524 * (t11510 * t11517
     # + t11502 * t11508 + t11506 * t11504) * t11560) * t183 / 0.2E1 + (
     #t10960 * t11597 - t11601) * t236 / 0.2E1 + (t11601 - t10967 * t116
     #36) * t236 / 0.2E1 + (t10973 * t11649 - t11653) * t236 / 0.2E1 + (
     #t11653 - t10979 * t11664) * t236 / 0.2E1 + (t4 * (t11588 * (t11670
     # + t11671 + t11672) / 0.2E1 + t11679 / 0.2E1) * t7768 - t4 * (t116
     #79 / 0.2E1 + t11627 * (t11684 + t11685 + t11686) / 0.2E1) * t7771)
     # * t236
        t11696 = t11695 * t7734
        t11706 = rx(t10261,j,t233,0,0)
        t11707 = rx(t10261,j,t233,1,1)
        t11709 = rx(t10261,j,t233,2,2)
        t11711 = rx(t10261,j,t233,1,2)
        t11713 = rx(t10261,j,t233,2,1)
        t11715 = rx(t10261,j,t233,1,0)
        t11717 = rx(t10261,j,t233,0,2)
        t11719 = rx(t10261,j,t233,0,1)
        t11722 = rx(t10261,j,t233,2,0)
        t11728 = 0.1E1 / (t11706 * t11707 * t11709 - t11706 * t11711 * t
     #11713 + t11715 * t11713 * t11717 - t11715 * t11719 * t11709 + t117
     #22 * t11719 * t11711 - t11722 * t11707 * t11717)
        t11729 = t11706 ** 2
        t11730 = t11719 ** 2
        t11731 = t11717 ** 2
        t11740 = t4 * t11728
        t11760 = u(t10261,j,t1430,n)
        t11777 = t7469 * t11093
        t11790 = t11311 ** 2
        t11791 = t11303 ** 2
        t11792 = t11307 ** 2
        t11795 = t7993 ** 2
        t11796 = t7985 ** 2
        t11797 = t7989 ** 2
        t11799 = t8006 * (t11795 + t11796 + t11797)
        t11804 = t11575 ** 2
        t11805 = t11567 ** 2
        t11806 = t11571 ** 2
        t11815 = u(t6542,t180,t1430,n)
        t11819 = (t11815 - t7502) * t236 / 0.2E1 + t7504 / 0.2E1
        t11823 = t10533 * t8042
        t11827 = u(t6542,t185,t1430,n)
        t11831 = (t11827 - t7766) * t236 / 0.2E1 + t7768 / 0.2E1
        t11837 = rx(t6542,j,t1430,0,0)
        t11838 = rx(t6542,j,t1430,1,1)
        t11840 = rx(t6542,j,t1430,2,2)
        t11842 = rx(t6542,j,t1430,1,2)
        t11844 = rx(t6542,j,t1430,2,1)
        t11846 = rx(t6542,j,t1430,1,0)
        t11848 = rx(t6542,j,t1430,0,2)
        t11850 = rx(t6542,j,t1430,0,1)
        t11853 = rx(t6542,j,t1430,2,0)
        t11859 = 0.1E1 / (t11838 * t11837 * t11840 - t11837 * t11842 * t
     #11844 + t11846 * t11844 * t11848 - t11846 * t11850 * t11840 + t118
     #53 * t11850 * t11842 - t11853 * t11838 * t11848)
        t11860 = t4 * t11859
        t11883 = (t11815 - t8038) * t183 / 0.2E1 + (t8038 - t11827) * t1
     #83 / 0.2E1
        t11889 = t11853 ** 2
        t11890 = t11844 ** 2
        t11891 = t11840 ** 2
        t11148 = t11325 * (t11302 * t11311 + t11315 * t11303 + t11313 * 
     #t11307)
        t11160 = t11589 * (t11566 * t11575 + t11579 * t11567 + t11577 * 
     #t11571)
        t11201 = t11860 * (t11837 * t11853 + t11850 * t11844 + t11848 * 
     #t11840)
        t11900 = (t8015 - t4 * (t8011 / 0.2E1 + t11728 * (t11729 + t1173
     #0 + t11731) / 0.2E1) * t10415) * t94 + t8033 + (t8030 - t11740 * (
     #t11706 * t11715 + t11719 * t11707 + t11717 * t11711) * ((t11224 - 
     #t10413) * t183 / 0.2E1 + (t10413 - t11488) * t183 / 0.2E1)) * t94 
     #/ 0.2E1 + t8047 + (t8044 - t11740 * (t11706 * t11722 + t11719 * t1
     #1713 + t11717 * t11709) * ((t11760 - t10413) * t236 / 0.2E1 + t104
     #96 / 0.2E1)) * t94 / 0.2E1 + (t11148 * t11333 - t11777) * t183 / 0
     #.2E1 + (t11777 - t11160 * t11597) * t183 / 0.2E1 + (t4 * (t11324 *
     # (t11790 + t11791 + t11792) / 0.2E1 + t11799 / 0.2E1) * t8024 - t4
     # * (t11799 / 0.2E1 + t11588 * (t11804 + t11805 + t11806) / 0.2E1) 
     #* t8026) * t183 + (t10766 * t11819 - t11823) * t183 / 0.2E1 + (t11
     #823 - t10973 * t11831) * t183 / 0.2E1 + (t11201 * (t8144 / 0.2E1 +
     # (t8038 - t11760) * t94 / 0.2E1) - t11095) * t236 / 0.2E1 + t11100
     # + (t11860 * (t11846 * t11853 + t11838 * t11844 + t11842 * t11840)
     # * t11883 - t11113) * t236 / 0.2E1 + t11118 + (t4 * (t11859 * (t11
     #889 + t11890 + t11891) / 0.2E1 + t11132 / 0.2E1) * t8040 - t11141)
     # * t236
        t11901 = t11900 * t8005
        t11904 = rx(t10261,j,t238,0,0)
        t11905 = rx(t10261,j,t238,1,1)
        t11907 = rx(t10261,j,t238,2,2)
        t11909 = rx(t10261,j,t238,1,2)
        t11911 = rx(t10261,j,t238,2,1)
        t11913 = rx(t10261,j,t238,1,0)
        t11915 = rx(t10261,j,t238,0,2)
        t11917 = rx(t10261,j,t238,0,1)
        t11920 = rx(t10261,j,t238,2,0)
        t11926 = 0.1E1 / (t11904 * t11905 * t11907 - t11904 * t11909 * t
     #11911 + t11913 * t11911 * t11915 - t11913 * t11917 * t11907 + t119
     #20 * t11917 * t11909 - t11920 * t11905 * t11915)
        t11927 = t11904 ** 2
        t11928 = t11917 ** 2
        t11929 = t11915 ** 2
        t11938 = t4 * t11926
        t11958 = u(t10261,j,t1441,n)
        t11975 = t7656 * t11102
        t11988 = t11350 ** 2
        t11989 = t11342 ** 2
        t11990 = t11346 ** 2
        t11993 = t8191 ** 2
        t11994 = t8183 ** 2
        t11995 = t8187 ** 2
        t11997 = t8204 * (t11993 + t11994 + t11995)
        t12002 = t11614 ** 2
        t12003 = t11606 ** 2
        t12004 = t11610 ** 2
        t12013 = u(t6542,t180,t1441,n)
        t12017 = t7507 / 0.2E1 + (t7505 - t12013) * t236 / 0.2E1
        t12021 = t10540 * t8240
        t12025 = u(t6542,t185,t1441,n)
        t12029 = t7771 / 0.2E1 + (t7769 - t12025) * t236 / 0.2E1
        t12035 = rx(t6542,j,t1441,0,0)
        t12036 = rx(t6542,j,t1441,1,1)
        t12038 = rx(t6542,j,t1441,2,2)
        t12040 = rx(t6542,j,t1441,1,2)
        t12042 = rx(t6542,j,t1441,2,1)
        t12044 = rx(t6542,j,t1441,1,0)
        t12046 = rx(t6542,j,t1441,0,2)
        t12048 = rx(t6542,j,t1441,0,1)
        t12051 = rx(t6542,j,t1441,2,0)
        t12057 = 0.1E1 / (t12035 * t12036 * t12038 - t12035 * t12040 * t
     #12042 + t12044 * t12042 * t12046 - t12044 * t12048 * t12038 + t120
     #51 * t12048 * t12040 - t12051 * t12036 * t12046)
        t12058 = t4 * t12057
        t12081 = (t12013 - t8236) * t183 / 0.2E1 + (t8236 - t12025) * t1
     #83 / 0.2E1
        t12087 = t12051 ** 2
        t12088 = t12042 ** 2
        t12089 = t12038 ** 2
        t11353 = t11364 * (t11341 * t11350 + t11354 * t11342 + t11352 * 
     #t11346)
        t11360 = t11628 * (t11605 * t11614 + t11618 * t11606 + t11616 * 
     #t11610)
        t11394 = t12058 * (t12035 * t12051 + t12048 * t12042 + t12046 * 
     #t12038)
        t12098 = (t8213 - t4 * (t8209 / 0.2E1 + t11926 * (t11927 + t1192
     #8 + t11929) / 0.2E1) * t10430) * t94 + t8231 + (t8228 - t11938 * (
     #t11904 * t11913 + t11917 * t11905 + t11915 * t11909) * ((t11227 - 
     #t10428) * t183 / 0.2E1 + (t10428 - t11491) * t183 / 0.2E1)) * t94 
     #/ 0.2E1 + t8245 + (t8242 - t11938 * (t11904 * t11920 + t11917 * t1
     #1911 + t11915 * t11907) * (t10498 / 0.2E1 + (t10428 - t11958) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + (t11353 * t11372 - t11975) * t183 / 0
     #.2E1 + (t11975 - t11360 * t11636) * t183 / 0.2E1 + (t4 * (t11363 *
     # (t11988 + t11989 + t11990) / 0.2E1 + t11997 / 0.2E1) * t8222 - t4
     # * (t11997 / 0.2E1 + t11627 * (t12002 + t12003 + t12004) / 0.2E1) 
     #* t8224) * t183 + (t10774 * t12017 - t12021) * t183 / 0.2E1 + (t12
     #021 - t10979 * t12029) * t183 / 0.2E1 + t11107 + (t11104 - t11394 
     #* (t8342 / 0.2E1 + (t8236 - t11958) * t94 / 0.2E1)) * t236 / 0.2E1
     # + t11127 + (t11124 - t12058 * (t12044 * t12051 + t12036 * t12042 
     #+ t12040 * t12038) * t12081) * t236 / 0.2E1 + (t11150 - t4 * (t111
     #46 / 0.2E1 + t12057 * (t12087 + t12088 + t12089) / 0.2E1) * t8238)
     # * t236
        t12099 = t12098 * t8203
        t12116 = t573 * t11158
        t12133 = t11302 ** 2
        t12134 = t11315 ** 2
        t12135 = t11313 ** 2
        t12154 = rx(t96,t1331,t233,0,0)
        t12155 = rx(t96,t1331,t233,1,1)
        t12157 = rx(t96,t1331,t233,2,2)
        t12159 = rx(t96,t1331,t233,1,2)
        t12161 = rx(t96,t1331,t233,2,1)
        t12163 = rx(t96,t1331,t233,1,0)
        t12165 = rx(t96,t1331,t233,0,2)
        t12167 = rx(t96,t1331,t233,0,1)
        t12170 = rx(t96,t1331,t233,2,0)
        t12176 = 0.1E1 / (t12154 * t12155 * t12157 - t12154 * t12159 * t
     #12161 + t12163 * t12161 * t12165 - t12163 * t12167 * t12157 + t121
     #70 * t12167 * t12159 - t12170 * t12155 * t12165)
        t12177 = t4 * t12176
        t12185 = t8461 / 0.2E1 + (t7567 - t11289) * t94 / 0.2E1
        t12191 = t12163 ** 2
        t12192 = t12155 ** 2
        t12193 = t12159 ** 2
        t12206 = u(t96,t1331,t1430,n)
        t12210 = (t12206 - t7567) * t236 / 0.2E1 + t7569 / 0.2E1
        t12216 = rx(t96,t180,t1430,0,0)
        t12217 = rx(t96,t180,t1430,1,1)
        t12219 = rx(t96,t180,t1430,2,2)
        t12221 = rx(t96,t180,t1430,1,2)
        t12223 = rx(t96,t180,t1430,2,1)
        t12225 = rx(t96,t180,t1430,1,0)
        t12227 = rx(t96,t180,t1430,0,2)
        t12229 = rx(t96,t180,t1430,0,1)
        t12232 = rx(t96,t180,t1430,2,0)
        t12238 = 0.1E1 / (t12217 * t12216 * t12219 - t12216 * t12221 * t
     #12223 + t12225 * t12223 * t12227 - t12225 * t12229 * t12219 + t122
     #32 * t12229 * t12221 - t12232 * t12217 * t12227)
        t12239 = t4 * t12238
        t12247 = t8523 / 0.2E1 + (t8093 - t11815) * t94 / 0.2E1
        t12260 = (t12206 - t8093) * t183 / 0.2E1 + t8157 / 0.2E1
        t12266 = t12232 ** 2
        t12267 = t12223 ** 2
        t12268 = t12219 ** 2
        t11529 = t12177 * (t12154 * t12163 + t12167 * t12155 + t12165 * 
     #t12159)
        t11545 = t12177 * (t12163 * t12170 + t12155 * t12161 + t12159 * 
     #t12157)
        t11550 = t12239 * (t12216 * t12232 + t12229 * t12223 + t12227 * 
     #t12219)
        t11557 = t12239 * (t12225 * t12232 + t12217 * t12223 + t12221 * 
     #t12219)
        t12277 = (t8419 - t4 * (t8415 / 0.2E1 + t11324 * (t12133 + t1213
     #4 + t12135) / 0.2E1) * t7609) * t94 + t8426 + (t8423 - t11148 * t1
     #1385) * t94 / 0.2E1 + t8431 + (t8428 - t10753 * t11819) * t94 / 0.
     #2E1 + (t11529 * t12185 - t8053) * t183 / 0.2E1 + t8058 + (t4 * (t1
     #2176 * (t12191 + t12192 + t12193) / 0.2E1 + t8072 / 0.2E1) * t7661
     # - t8081) * t183 + (t11545 * t12210 - t8099) * t183 / 0.2E1 + t810
     #4 + (t11550 * t12247 - t7613) * t236 / 0.2E1 + t7618 + (t11557 * t
     #12260 - t7665) * t236 / 0.2E1 + t7670 + (t4 * (t12238 * (t12266 + 
     #t12267 + t12268) / 0.2E1 + t7688 / 0.2E1) * t8095 - t7697) * t236
        t12278 = t12277 * t7601
        t12281 = t11341 ** 2
        t12282 = t11354 ** 2
        t12283 = t11352 ** 2
        t12302 = rx(t96,t1331,t238,0,0)
        t12303 = rx(t96,t1331,t238,1,1)
        t12305 = rx(t96,t1331,t238,2,2)
        t12307 = rx(t96,t1331,t238,1,2)
        t12309 = rx(t96,t1331,t238,2,1)
        t12311 = rx(t96,t1331,t238,1,0)
        t12313 = rx(t96,t1331,t238,0,2)
        t12315 = rx(t96,t1331,t238,0,1)
        t12318 = rx(t96,t1331,t238,2,0)
        t12324 = 0.1E1 / (t12302 * t12303 * t12305 - t12302 * t12307 * t
     #12309 + t12311 * t12309 * t12313 - t12311 * t12315 * t12305 + t123
     #18 * t12315 * t12307 - t12318 * t12303 * t12313)
        t12325 = t4 * t12324
        t12333 = t8609 / 0.2E1 + (t7570 - t11292) * t94 / 0.2E1
        t12339 = t12311 ** 2
        t12340 = t12303 ** 2
        t12341 = t12307 ** 2
        t12354 = u(t96,t1331,t1441,n)
        t12358 = t7572 / 0.2E1 + (t7570 - t12354) * t236 / 0.2E1
        t12364 = rx(t96,t180,t1441,0,0)
        t12365 = rx(t96,t180,t1441,1,1)
        t12367 = rx(t96,t180,t1441,2,2)
        t12369 = rx(t96,t180,t1441,1,2)
        t12371 = rx(t96,t180,t1441,2,1)
        t12373 = rx(t96,t180,t1441,1,0)
        t12375 = rx(t96,t180,t1441,0,2)
        t12377 = rx(t96,t180,t1441,0,1)
        t12380 = rx(t96,t180,t1441,2,0)
        t12386 = 0.1E1 / (t12365 * t12364 * t12367 - t12364 * t12369 * t
     #12371 + t12373 * t12371 * t12375 - t12373 * t12377 * t12367 + t123
     #80 * t12377 * t12369 - t12380 * t12365 * t12375)
        t12387 = t4 * t12386
        t12395 = t8671 / 0.2E1 + (t8291 - t12013) * t94 / 0.2E1
        t12408 = (t12354 - t8291) * t183 / 0.2E1 + t8355 / 0.2E1
        t12414 = t12380 ** 2
        t12415 = t12371 ** 2
        t12416 = t12367 ** 2
        t11669 = t12325 * (t12302 * t12311 + t12315 * t12303 + t12313 * 
     #t12307)
        t11692 = t12325 * (t12311 * t12318 + t12303 * t12309 + t12307 * 
     #t12305)
        t11699 = t12387 * (t12364 * t12380 + t12377 * t12371 + t12375 * 
     #t12367)
        t11704 = t12387 * (t12373 * t12380 + t12365 * t12371 + t12369 * 
     #t12367)
        t12425 = (t8567 - t4 * (t8563 / 0.2E1 + t11363 * (t12281 + t1228
     #2 + t12283) / 0.2E1) * t7648) * t94 + t8574 + (t8571 - t11353 * t1
     #1400) * t94 / 0.2E1 + t8579 + (t8576 - t10759 * t12017) * t94 / 0.
     #2E1 + (t11669 * t12333 - t8251) * t183 / 0.2E1 + t8256 + (t4 * (t1
     #2324 * (t12339 + t12340 + t12341) / 0.2E1 + t8270 / 0.2E1) * t7676
     # - t8279) * t183 + (t11692 * t12358 - t8297) * t183 / 0.2E1 + t830
     #2 + t7655 + (t7652 - t11699 * t12395) * t236 / 0.2E1 + t7683 + (t7
     #680 - t11704 * t12408) * t236 / 0.2E1 + (t7706 - t4 * (t7702 / 0.2
     #E1 + t12386 * (t12414 + t12415 + t12416) / 0.2E1) * t8293) * t236
        t12426 = t12425 * t7640
        t12430 = (t12278 - t7710) * t236 / 0.2E1 + (t7710 - t12426) * t2
     #36 / 0.2E1
        t12434 = t6863 * t8381
        t12438 = t11566 ** 2
        t12439 = t11579 ** 2
        t12440 = t11577 ** 2
        t12459 = rx(t96,t1379,t233,0,0)
        t12460 = rx(t96,t1379,t233,1,1)
        t12462 = rx(t96,t1379,t233,2,2)
        t12464 = rx(t96,t1379,t233,1,2)
        t12466 = rx(t96,t1379,t233,2,1)
        t12468 = rx(t96,t1379,t233,1,0)
        t12470 = rx(t96,t1379,t233,0,2)
        t12472 = rx(t96,t1379,t233,0,1)
        t12475 = rx(t96,t1379,t233,2,0)
        t12481 = 0.1E1 / (t12460 * t12459 * t12462 - t12459 * t12464 * t
     #12466 + t12468 * t12466 * t12470 - t12468 * t12472 * t12462 + t124
     #75 * t12472 * t12464 - t12475 * t12460 * t12470)
        t12482 = t4 * t12481
        t12490 = t8766 / 0.2E1 + (t7831 - t11553) * t94 / 0.2E1
        t12496 = t12468 ** 2
        t12497 = t12460 ** 2
        t12498 = t12464 ** 2
        t12511 = u(t96,t1379,t1430,n)
        t12515 = (t12511 - t7831) * t236 / 0.2E1 + t7833 / 0.2E1
        t12521 = rx(t96,t185,t1430,0,0)
        t12522 = rx(t96,t185,t1430,1,1)
        t12524 = rx(t96,t185,t1430,2,2)
        t12526 = rx(t96,t185,t1430,1,2)
        t12528 = rx(t96,t185,t1430,2,1)
        t12530 = rx(t96,t185,t1430,1,0)
        t12532 = rx(t96,t185,t1430,0,2)
        t12534 = rx(t96,t185,t1430,0,1)
        t12537 = rx(t96,t185,t1430,2,0)
        t12543 = 0.1E1 / (t12521 * t12522 * t12524 - t12521 * t12526 * t
     #12528 + t12530 * t12528 * t12532 - t12530 * t12534 * t12524 + t125
     #37 * t12534 * t12526 - t12537 * t12522 * t12532)
        t12544 = t4 * t12543
        t12552 = t8828 / 0.2E1 + (t8105 - t11827) * t94 / 0.2E1
        t12565 = t8159 / 0.2E1 + (t8105 - t12511) * t183 / 0.2E1
        t12571 = t12537 ** 2
        t12572 = t12528 ** 2
        t12573 = t12524 ** 2
        t11817 = t12482 * (t12459 * t12468 + t12472 * t12460 + t12470 * 
     #t12464)
        t11834 = t12482 * (t12468 * t12475 + t12460 * t12466 + t12464 * 
     #t12462)
        t11843 = t12544 * (t12521 * t12537 + t12528 * t12534 + t12532 * 
     #t12524)
        t11852 = t12544 * (t12530 * t12537 + t12522 * t12528 + t12526 * 
     #t12524)
        t12582 = (t8724 - t4 * (t8720 / 0.2E1 + t11588 * (t12438 + t1243
     #9 + t12440) / 0.2E1) * t7873) * t94 + t8731 + (t8728 - t11160 * t1
     #1649) * t94 / 0.2E1 + t8736 + (t8733 - t10960 * t11831) * t94 / 0.
     #2E1 + t8067 + (t8064 - t11817 * t12490) * t183 / 0.2E1 + (t8090 - 
     #t4 * (t8086 / 0.2E1 + t12481 * (t12496 + t12497 + t12498) / 0.2E1)
     # * t7925) * t183 + t8114 + (t8111 - t11834 * t12515) * t183 / 0.2E
     #1 + (t11843 * t12552 - t7877) * t236 / 0.2E1 + t7882 + (t11852 * t
     #12565 - t7929) * t236 / 0.2E1 + t7934 + (t4 * (t12543 * (t12571 + 
     #t12572 + t12573) / 0.2E1 + t7952 / 0.2E1) * t8107 - t7961) * t236
        t12583 = t12582 * t7865
        t12586 = t11605 ** 2
        t12587 = t11618 ** 2
        t12588 = t11616 ** 2
        t12607 = rx(t96,t1379,t238,0,0)
        t12608 = rx(t96,t1379,t238,1,1)
        t12610 = rx(t96,t1379,t238,2,2)
        t12612 = rx(t96,t1379,t238,1,2)
        t12614 = rx(t96,t1379,t238,2,1)
        t12616 = rx(t96,t1379,t238,1,0)
        t12618 = rx(t96,t1379,t238,0,2)
        t12620 = rx(t96,t1379,t238,0,1)
        t12623 = rx(t96,t1379,t238,2,0)
        t12629 = 0.1E1 / (t12607 * t12608 * t12610 - t12607 * t12612 * t
     #12614 + t12616 * t12614 * t12618 - t12616 * t12620 * t12610 + t126
     #23 * t12620 * t12612 - t12623 * t12608 * t12618)
        t12630 = t4 * t12629
        t12638 = t8914 / 0.2E1 + (t7834 - t11556) * t94 / 0.2E1
        t12644 = t12616 ** 2
        t12645 = t12608 ** 2
        t12646 = t12612 ** 2
        t12659 = u(t96,t1379,t1441,n)
        t12663 = t7836 / 0.2E1 + (t7834 - t12659) * t236 / 0.2E1
        t12669 = rx(t96,t185,t1441,0,0)
        t12670 = rx(t96,t185,t1441,1,1)
        t12672 = rx(t96,t185,t1441,2,2)
        t12674 = rx(t96,t185,t1441,1,2)
        t12676 = rx(t96,t185,t1441,2,1)
        t12678 = rx(t96,t185,t1441,1,0)
        t12680 = rx(t96,t185,t1441,0,2)
        t12682 = rx(t96,t185,t1441,0,1)
        t12685 = rx(t96,t185,t1441,2,0)
        t12691 = 0.1E1 / (t12669 * t12670 * t12672 - t12669 * t12674 * t
     #12676 + t12678 * t12676 * t12680 - t12678 * t12682 * t12672 + t126
     #85 * t12682 * t12674 - t12685 * t12670 * t12680)
        t12692 = t4 * t12691
        t12700 = t8976 / 0.2E1 + (t8303 - t12025) * t94 / 0.2E1
        t12713 = t8357 / 0.2E1 + (t8303 - t12659) * t183 / 0.2E1
        t12719 = t12685 ** 2
        t12720 = t12676 ** 2
        t12721 = t12672 ** 2
        t11955 = t12630 * (t12607 * t12616 + t12620 * t12608 + t12618 * 
     #t12612)
        t11969 = t12630 * (t12623 * t12616 + t12608 * t12614 + t12612 * 
     #t12610)
        t11976 = t12692 * (t12669 * t12685 + t12676 * t12682 + t12680 * 
     #t12672)
        t11981 = t12692 * (t12678 * t12685 + t12670 * t12676 + t12674 * 
     #t12672)
        t12730 = (t8872 - t4 * (t8868 / 0.2E1 + t11627 * (t12586 + t1258
     #7 + t12588) / 0.2E1) * t7912) * t94 + t8879 + (t8876 - t11360 * t1
     #1664) * t94 / 0.2E1 + t8884 + (t8881 - t10967 * t12029) * t94 / 0.
     #2E1 + t8265 + (t8262 - t11955 * t12638) * t183 / 0.2E1 + (t8288 - 
     #t4 * (t8284 / 0.2E1 + t12629 * (t12644 + t12645 + t12646) / 0.2E1)
     # * t7940) * t183 + t8312 + (t8309 - t11969 * t12663) * t183 / 0.2E
     #1 + t7919 + (t7916 - t11976 * t12700) * t236 / 0.2E1 + t7947 + (t7
     #944 - t11981 * t12713) * t236 / 0.2E1 + (t7970 - t4 * (t7966 / 0.2
     #E1 + t12691 * (t12719 + t12720 + t12721) / 0.2E1) * t8305) * t236
        t12731 = t12730 * t7904
        t12735 = (t12583 - t7974) * t236 / 0.2E1 + (t7974 - t12731) * t2
     #36 / 0.2E1
        t12748 = t590 * t11158
        t12766 = (t12278 - t8179) * t183 / 0.2E1 + (t8179 - t12583) * t1
     #83 / 0.2E1
        t12770 = t6863 * t7978
        t12779 = (t12426 - t8377) * t183 / 0.2E1 + (t8377 - t12731) * t1
     #83 / 0.2E1
        t12789 = (t7446 - t6583 * t11156) * t94 + t7983 + (t7980 - t6339
     # * ((t11432 - t11154) * t183 / 0.2E1 + (t11154 - t11696) * t183 / 
     #0.2E1)) * t94 / 0.2E1 + t8386 + (t8383 - t6244 * ((t11901 - t11154
     #) * t236 / 0.2E1 + (t11154 - t12099) * t236 / 0.2E1)) * t94 / 0.2E
     #1 + (t3630 * (t8388 / 0.2E1 + (t7710 - t11432) * t94 / 0.2E1) - t1
     #2116) * t183 / 0.2E1 + (t12116 - t3877 * (t8399 / 0.2E1 + (t7974 -
     # t11696) * t94 / 0.2E1)) * t183 / 0.2E1 + (t7333 * t7712 - t7342 *
     # t7976) * t183 + (t6856 * t12430 - t12434) * t183 / 0.2E1 + (t1243
     #4 - t6872 * t12735) * t183 / 0.2E1 + (t4807 * (t9020 / 0.2E1 + (t8
     #179 - t11901) * t94 / 0.2E1) - t12748) * t236 / 0.2E1 + (t12748 - 
     #t4947 * (t9031 / 0.2E1 + (t8377 - t12099) * t94 / 0.2E1)) * t236 /
     # 0.2E1 + (t6889 * t12766 - t12770) * t236 / 0.2E1 + (t12770 - t689
     #5 * t12779) * t236 / 0.2E1 + (t7418 * t8181 - t7427 * t8379) * t23
     #6
        t12790 = t12789 * t118
        t12800 = t6841 / 0.2E1 + t10583 / 0.2E1
        t12802 = t6339 * t12800
        t12817 = ut(t6542,t180,t233,n)
        t12820 = ut(t6542,t180,t238,n)
        t12824 = (t12817 - t7048) * t236 / 0.2E1 + (t7048 - t12820) * t2
     #36 / 0.2E1
        t12828 = t10511 * t6882
        t12832 = ut(t6542,t185,t233,n)
        t12835 = ut(t6542,t185,t238,n)
        t12839 = (t12832 - t7067) * t236 / 0.2E1 + (t7067 - t12835) * t2
     #36 / 0.2E1
        t12850 = t6244 * t12800
        t12866 = (t12817 - t6875) * t183 / 0.2E1 + (t6875 - t12832) * t1
     #83 / 0.2E1
        t12870 = t10511 * t7244
        t12879 = (t12820 - t6878) * t183 / 0.2E1 + (t6878 - t12835) * t1
     #83 / 0.2E1
        t12889 = t10851 + t9071 + t10606 / 0.2E1 + t9072 + t10888 / 0.2E
     #1 + (t6985 * (t7050 / 0.2E1 + t10898 / 0.2E1) - t12802) * t183 / 0
     #.2E1 + (t12802 - t7208 * (t7069 / 0.2E1 + t10912 / 0.2E1)) * t183 
     #/ 0.2E1 + (t11055 * t7240 - t11064 * t7242) * t183 + (t10507 * t12
     #824 - t12828) * t183 / 0.2E1 + (t12828 - t10519 * t12839) * t183 /
     # 0.2E1 + (t7486 * (t7084 / 0.2E1 + t10789 / 0.2E1) - t12850) * t23
     #6 / 0.2E1 + (t12850 - t7668 * (t7098 / 0.2E1 + t10808 / 0.2E1)) * 
     #t236 / 0.2E1 + (t10533 * t12866 - t12870) * t236 / 0.2E1 + (t12870
     # - t10540 * t12879) * t236 / 0.2E1 + (t11140 * t6877 - t11149 * t6
     #880) * t236
        t12895 = dx * (t9170 / 0.2E1 + (t9168 - t12889 * t6574) * t94 / 
     #0.2E1)
        t12899 = dx * (t7434 - t11156)
        t12902 = t2 + t6837 - t6852 + t7300 - t7439 + t7445 + t9070 - t9
     #175 + t9179 - t145 - t136 * t10578 - t10594 - t2076 * t11020 / 0.2
     #E1 - t136 * t11159 / 0.2E1 - t11167 - t2917 * t12790 / 0.6E1 - t20
     #76 * t12895 / 0.4E1 - t136 * t12899 / 0.12E2
        t12915 = sqrt(t9184 + t9185 + t9186 + 0.8E1 * t120 + 0.8E1 * t12
     #1 + 0.8E1 * t122 - 0.2E1 * dx * ((t29 + t30 + t31 - t58 - t59 - t6
     #0) * t94 / 0.2E1 - (t120 + t121 + t122 - t6576 - t6577 - t6578) * 
     #t94 / 0.2E1))
        t12916 = 0.1E1 / t12915
        t12921 = t6616 * t9202 * t10153
        t12924 = t568 * t9205 * t10158 / 0.2E1
        t12927 = t568 * t9209 * t10163 / 0.6E1
        t12929 = t9202 * t10166 / 0.24E2
        t12941 = t2 + t9228 - t6852 + t9230 - t9232 + t7445 + t9234 - t9
     #236 + t9238 - t145 - t9202 * t10578 - t10594 - t9216 * t11020 / 0.
     #2E1 - t9202 * t11159 / 0.2E1 - t11167 - t9221 * t12790 / 0.6E1 - t
     #9216 * t12895 / 0.4E1 - t9202 * t12899 / 0.12E2
        t12944 = 0.2E1 * t10169 * t12941 * t12916
        t12946 = (t6616 * t136 * t10153 + t568 * t159 * t10158 / 0.2E1 +
     # t568 * t893 * t10163 / 0.6E1 - t136 * t10166 / 0.24E2 + 0.2E1 * t
     #10169 * t12902 * t12916 - t12921 - t12924 - t12927 + t12929 - t129
     #44) * t133
        t12952 = t6616 * (t571 - dx * t6548 / 0.24E2)
        t12954 = dx * t6587 / 0.24E2
        t12970 = t4 * (t9260 + t9264 / 0.2E1 - dx * ((t9257 - t9259) * t
     #94 / 0.2E1 - (t9264 - t6575 * t6715) * t94 / 0.2E1) / 0.8E1)
        t12981 = (t7240 - t7242) * t183
        t12998 = t9279 + t9280 - t9284 + t1089 / 0.4E1 + t1092 / 0.4E1 -
     # t9323 / 0.12E2 - dx * ((t9301 + t9302 - t9303 - t9306 - t9307 + t
     #9308) * t94 / 0.2E1 - (t9309 + t9310 - t9324 - t7240 / 0.2E1 - t72
     #42 / 0.2E1 + t1330 * (((t10823 - t7240) * t183 - t12981) * t183 / 
     #0.2E1 + (t12981 - (t7242 - t10828) * t183) * t183 / 0.2E1) / 0.6E1
     #) * t94 / 0.2E1) / 0.8E1
        t13003 = t4 * (t9259 / 0.2E1 + t9264 / 0.2E1)
        t13005 = t4128 / 0.4E1 + t4392 / 0.4E1 + t7712 / 0.4E1 + t7976 /
     # 0.4E1
        t13011 = (t9510 - t7479 * t7050) * t94
        t13017 = (t9516 - t6985 * (t10823 / 0.2E1 + t7240 / 0.2E1)) * t9
     #4
        t13022 = (t9521 - t6998 * t12824) * t94
        t13027 = (t9095 - t12817) * t94
        t13029 = t9528 / 0.2E1 + t13027 / 0.2E1
        t13033 = t3645 * t9074
        t13038 = (t9098 - t12820) * t94
        t13040 = t9539 / 0.2E1 + t13038 / 0.2E1
        t13047 = t10742 / 0.2E1 + t9140 / 0.2E1
        t13051 = t6856 * t9514
        t13056 = t10763 / 0.2E1 + t9153 / 0.2E1
        t13066 = t13011 + t9519 + t13017 / 0.2E1 + t9524 + t13022 / 0.2E
     #1 + t10683 / 0.2E1 + t9083 + t10932 + t10993 / 0.2E1 + t9109 + (t7
     #086 * t13029 - t13033) * t236 / 0.2E1 + (t13033 - t7122 * t13040) 
     #* t236 / 0.2E1 + (t7140 * t13047 - t13051) * t236 / 0.2E1 + (t1305
     #1 - t7152 * t13056) * t236 / 0.2E1 + (t7696 * t9097 - t7705 * t910
     #0) * t236
        t13067 = t13066 * t3886
        t13072 = (t9571 - t7743 * t7069) * t94
        t13078 = (t9577 - t7208 * (t7242 / 0.2E1 + t10828 / 0.2E1)) * t9
     #4
        t13083 = (t9582 - t7222 * t12839) * t94
        t13088 = (t9110 - t12832) * t94
        t13090 = t9589 / 0.2E1 + t13088 / 0.2E1
        t13094 = t3893 * t9085
        t13099 = (t9113 - t12835) * t94
        t13101 = t9600 / 0.2E1 + t13099 / 0.2E1
        t13108 = t9142 / 0.2E1 + t10748 / 0.2E1
        t13112 = t6872 * t9575
        t13117 = t9155 / 0.2E1 + t10769 / 0.2E1
        t13127 = t13072 + t9580 + t13078 / 0.2E1 + t9585 + t13083 / 0.2E
     #1 + t9090 + t10698 / 0.2E1 + t10937 + t9122 + t11009 / 0.2E1 + (t7
     #307 * t13090 - t13094) * t236 / 0.2E1 + (t13094 - t7354 * t13101) 
     #* t236 / 0.2E1 + (t7370 * t13108 - t13112) * t236 / 0.2E1 + (t1311
     #2 - t7387 * t13117) * t236 / 0.2E1 + (t7960 * t9112 - t7969 * t911
     #5) * t236
        t13128 = t13127 * t4150
        t13132 = t9570 / 0.4E1 + t9631 / 0.4E1 + (t13067 - t9168) * t183
     # / 0.4E1 + (t9168 - t13128) * t183 / 0.4E1
        t13138 = dx * (t934 / 0.2E1 - t7248 / 0.2E1)
        t13142 = t12970 * t9202 * t12998
        t13145 = t13003 * t9216 * t13005 / 0.2E1
        t13148 = t13003 * t9221 * t13132 / 0.6E1
        t13150 = t9202 * t13138 / 0.24E2
        t13152 = (t12970 * t136 * t12998 + t13003 * t2076 * t13005 / 0.2
     #E1 + t13003 * t2917 * t13132 / 0.6E1 - t136 * t13138 / 0.24E2 - t1
     #3142 - t13145 - t13148 + t13150) * t133
        t13165 = (t6717 - t6719) * t183
        t13183 = t12970 * (t9662 + t9663 - t9667 + t582 / 0.4E1 + t585 /
     # 0.4E1 - t9706 / 0.12E2 - dx * ((t9684 + t9685 - t9686 - t9689 - t
     #9690 + t9691) * t94 / 0.2E1 - (t9692 + t9693 - t9707 - t6717 / 0.2
     #E1 - t6719 / 0.2E1 + t1330 * (((t7490 - t6717) * t183 - t13165) * 
     #t183 / 0.2E1 + (t13165 - (t6719 - t7754) * t183) * t183 / 0.2E1) /
     # 0.6E1) * t94 / 0.2E1) / 0.8E1)
        t13187 = dx * (t227 / 0.2E1 - t6725 / 0.2E1) / 0.24E2
        t13203 = t4 * (t9727 + t9731 / 0.2E1 - dx * ((t9724 - t9726) * t
     #94 / 0.2E1 - (t9731 - t6575 * t6624) * t94 / 0.2E1) / 0.8E1)
        t13214 = (t6877 - t6880) * t236
        t13231 = t9746 + t9747 - t9751 + t1102 / 0.4E1 + t1105 / 0.4E1 -
     # t9790 / 0.12E2 - dx * ((t9768 + t9769 - t9770 - t9773 - t9774 + t
     #9775) * t94 / 0.2E1 - (t9776 + t9777 - t9791 - t6877 / 0.2E1 - t68
     #80 / 0.2E1 + t1429 * (((t10860 - t6877) * t236 - t13214) * t236 / 
     #0.2E1 + (t13214 - (t6880 - t10865) * t236) * t236 / 0.2E1) / 0.6E1
     #) * t94 / 0.2E1) / 0.8E1
        t13236 = t4 * (t9726 / 0.2E1 + t9731 / 0.2E1)
        t13238 = t5272 / 0.4E1 + t5470 / 0.4E1 + t8181 / 0.4E1 + t8379 /
     # 0.4E1
        t13244 = (t9953 - t8014 * t7084) * t94
        t13248 = (t9957 - t7469 * t12866) * t94
        t13255 = (t9964 - t7486 * (t10860 / 0.2E1 + t6877 / 0.2E1)) * t9
     #4
        t13260 = t4798 * t9124
        t13274 = t10946 / 0.2E1 + t9097 / 0.2E1
        t13278 = t6889 * t9962
        t13283 = t10965 / 0.2E1 + t9112 / 0.2E1
        t13291 = t13244 + t9960 + t13248 / 0.2E1 + t9967 + t13255 / 0.2E
     #1 + (t7498 * t13029 - t13260) * t183 / 0.2E1 + (t13260 - t7510 * t
     #13090) * t183 / 0.2E1 + (t8080 * t9140 - t8089 * t9142) * t183 + (
     #t7140 * t13274 - t13278) * t183 / 0.2E1 + (t13278 - t7370 * t13283
     #) * t183 / 0.2E1 + t10715 / 0.2E1 + t9131 + t10648 / 0.2E1 + t9151
     # + t10621
        t13292 = t13291 * t5096
        t13297 = (t10006 - t8212 * t7098) * t94
        t13301 = (t10010 - t7656 * t12879) * t94
        t13308 = (t10017 - t7668 * (t6880 / 0.2E1 + t10865 / 0.2E1)) * t
     #94
        t13313 = t4925 * t9133
        t13327 = t9100 / 0.2E1 + t10951 / 0.2E1
        t13331 = t6895 * t10015
        t13336 = t9115 / 0.2E1 + t10970 / 0.2E1
        t13344 = t13297 + t10013 + t13301 / 0.2E1 + t10020 + t13308 / 0.
     #2E1 + (t7675 * t13040 - t13313) * t183 / 0.2E1 + (t13313 - t7692 *
     # t13101) * t183 / 0.2E1 + (t8278 * t9153 - t8287 * t9155) * t183 +
     # (t7152 * t13327 - t13331) * t183 / 0.2E1 + (t13331 - t7387 * t133
     #36) * t183 / 0.2E1 + t9138 + t10730 / 0.2E1 + t9162 + t10666 / 0.2
     #E1 + t10626
        t13345 = t13344 * t5294
        t13349 = t10005 / 0.4E1 + t10058 / 0.4E1 + (t13292 - t9168) * t2
     #36 / 0.4E1 + (t9168 - t13345) * t236 / 0.4E1
        t13355 = dx * (t970 / 0.2E1 - t6886 / 0.2E1)
        t13359 = t13203 * t9202 * t13231
        t13362 = t13236 * t9216 * t13238 / 0.2E1
        t13365 = t13236 * t9221 * t13349 / 0.6E1
        t13367 = t9202 * t13355 / 0.24E2
        t13369 = (t13203 * t136 * t13231 + t13236 * t2076 * t13238 / 0.2
     #E1 + t13236 * t2917 * t13349 / 0.6E1 - t136 * t13355 / 0.24E2 - t1
     #3359 - t13362 - t13365 + t13367) * t133
        t13382 = (t6627 - t6630) * t236
        t13400 = t13203 * (t10089 + t10090 - t10094 + t599 / 0.4E1 + t60
     #2 / 0.4E1 - t10133 / 0.12E2 - dx * ((t10111 + t10112 - t10113 - t1
     #0116 - t10117 + t10118) * t94 / 0.2E1 - (t10119 + t10120 - t10134 
     #- t6627 / 0.2E1 - t6630 / 0.2E1 + t1429 * (((t8040 - t6627) * t236
     # - t13382) * t236 / 0.2E1 + (t13382 - (t6630 - t8238) * t236) * t2
     #36 / 0.2E1) / 0.6E1) * t94 / 0.2E1) / 0.8E1)
        t13404 = dx * (t278 / 0.2E1 - t6636 / 0.2E1) / 0.24E2
        t13409 = t9244 * t161 / 0.6E1 + (t9250 + t9204 + t9208 - t9252 +
     # t9212 - t9214 + t9242 - t9244 * t9201) * t161 / 0.2E1 + t9653 * t
     #161 / 0.6E1 + (t9715 + t9643 + t9646 - t9719 + t9649 - t9651 - t96
     #53 * t9201) * t161 / 0.2E1 + t10080 * t161 / 0.6E1 + (t10142 + t10
     #070 + t10073 - t10146 + t10076 - t10078 - t10080 * t9201) * t161 /
     # 0.2E1 - t12946 * t161 / 0.6E1 - (t12952 + t12921 + t12924 - t1295
     #4 + t12927 - t12929 + t12944 - t12946 * t9201) * t161 / 0.2E1 - t1
     #3152 * t161 / 0.6E1 - (t13183 + t13142 + t13145 - t13187 + t13148 
     #- t13150 - t13152 * t9201) * t161 / 0.2E1 - t13369 * t161 / 0.6E1 
     #- (t13400 + t13359 + t13362 - t13404 + t13365 - t13367 - t13369 * 
     #t9201) * t161 / 0.2E1
        t13412 = t632 * t637
        t13417 = t673 * t678
        t13425 = t4 * (t13412 / 0.2E1 + t9260 - dy * ((t3953 * t3958 - t
     #13412) * t183 / 0.2E1 - (t9259 - t13417) * t183 / 0.2E1) / 0.8E1)
        t13431 = (t975 - t1114) * t94
        t13433 = ((t973 - t975) * t94 - t13431) * t94
        t13437 = (t13431 - (t1114 - t7050) * t94) * t94
        t13440 = t1227 * (t13433 / 0.2E1 + t13437 / 0.2E1)
        t13442 = t139 / 0.4E1
        t13443 = t147 / 0.4E1
        t13444 = t6848 / 0.12E2
        t13450 = (t2591 - t6926) * t94
        t13461 = t975 / 0.2E1
        t13462 = t1114 / 0.2E1
        t13463 = t13440 / 0.6E1
        t13466 = t990 / 0.2E1
        t13467 = t1127 / 0.2E1
        t13471 = (t990 - t1127) * t94
        t13473 = ((t988 - t990) * t94 - t13471) * t94
        t13477 = (t13471 - (t1127 - t7069) * t94) * t94
        t13480 = t1227 * (t13473 / 0.2E1 + t13477 / 0.2E1)
        t13481 = t13480 / 0.6E1
        t13488 = t975 / 0.4E1 + t1114 / 0.4E1 - t13440 / 0.12E2 + t13442
     # + t13443 - t13444 - dy * ((t2591 / 0.2E1 + t6926 / 0.2E1 - t1227 
     #* (((t2589 - t2591) * t94 - t13450) * t94 / 0.2E1 + (t13450 - (t69
     #26 - t10677) * t94) * t94 / 0.2E1) / 0.6E1 - t13461 - t13462 + t13
     #463) * t183 / 0.2E1 - (t2060 + t6838 - t6849 - t13466 - t13467 + t
     #13481) * t183 / 0.2E1) / 0.8E1
        t13493 = t4 * (t13412 / 0.2E1 + t9259 / 0.2E1)
        t13495 = t5481 / 0.4E1 + t8388 / 0.4E1 + t2905 / 0.4E1 + t7434 /
     # 0.4E1
        t13504 = (t9423 - t9568) * t94 / 0.4E1 + (t9568 - t13067) * t94 
     #/ 0.4E1 + t6398 / 0.4E1 + t9170 / 0.4E1
        t13510 = dy * (t6932 / 0.2E1 - t1133 / 0.2E1)
        t13514 = t13425 * t9202 * t13488
        t13517 = t13493 * t9216 * t13495 / 0.2E1
        t13520 = t13493 * t9221 * t13504 / 0.6E1
        t13522 = t9202 * t13510 / 0.24E2
        t13524 = (t13425 * t136 * t13488 + t13493 * t2076 * t13495 / 0.2
     #E1 + t13493 * t2917 * t13504 / 0.6E1 - t136 * t13510 / 0.24E2 - t1
     #3514 - t13517 - t13520 + t13522) * t133
        t13532 = (t311 - t639) * t94
        t13534 = ((t309 - t311) * t94 - t13532) * t94
        t13538 = (t13532 - (t639 - t6648) * t94) * t94
        t13541 = t1227 * (t13534 / 0.2E1 + t13538 / 0.2E1)
        t13543 = t171 / 0.4E1
        t13544 = t571 / 0.4E1
        t13547 = t1227 * (t1614 / 0.2E1 + t6549 / 0.2E1)
        t13548 = t13547 / 0.12E2
        t13554 = (t1783 - t3960) * t94
        t13565 = t311 / 0.2E1
        t13566 = t639 / 0.2E1
        t13567 = t13541 / 0.6E1
        t13570 = t171 / 0.2E1
        t13571 = t571 / 0.2E1
        t13572 = t13547 / 0.6E1
        t13573 = t354 / 0.2E1
        t13574 = t680 / 0.2E1
        t13578 = (t354 - t680) * t94
        t13580 = ((t352 - t354) * t94 - t13578) * t94
        t13584 = (t13578 - (t680 - t6667) * t94) * t94
        t13587 = t1227 * (t13580 / 0.2E1 + t13584 / 0.2E1)
        t13588 = t13587 / 0.6E1
        t13596 = t13425 * (t311 / 0.4E1 + t639 / 0.4E1 - t13541 / 0.12E2
     # + t13543 + t13544 - t13548 - dy * ((t1783 / 0.2E1 + t3960 / 0.2E1
     # - t1227 * (((t1780 - t1783) * t94 - t13554) * t94 / 0.2E1 + (t135
     #54 - (t3960 - t7544) * t94) * t94 / 0.2E1) / 0.6E1 - t13565 - t135
     #66 + t13567) * t183 / 0.2E1 - (t13570 + t13571 - t13572 - t13573 -
     # t13574 + t13588) * t183 / 0.2E1) / 0.8E1)
        t13600 = dy * (t3966 / 0.2E1 - t686 / 0.2E1) / 0.24E2
        t13607 = t925 - dy * t6965 / 0.24E2
        t13612 = t161 * t4127 * t183
        t13617 = t895 * t9569 * t183
        t13620 = dy * t6978
        t13623 = cc * t6774
        t13627 = (t3558 - t3912) * t94
        t13639 = j + 3
        t13640 = rx(i,t13639,k,0,0)
        t13641 = rx(i,t13639,k,1,1)
        t13643 = rx(i,t13639,k,2,2)
        t13645 = rx(i,t13639,k,1,2)
        t13647 = rx(i,t13639,k,2,1)
        t13649 = rx(i,t13639,k,1,0)
        t13651 = rx(i,t13639,k,0,2)
        t13653 = rx(i,t13639,k,0,1)
        t13656 = rx(i,t13639,k,2,0)
        t13662 = 0.1E1 / (t13640 * t13641 * t13643 - t13640 * t13645 * t
     #13647 + t13649 * t13647 * t13651 - t13649 * t13653 * t13643 + t136
     #56 * t13653 * t13645 - t13656 * t13641 * t13651)
        t13663 = t13649 ** 2
        t13664 = t13641 ** 2
        t13665 = t13645 ** 2
        t13667 = t13662 * (t13663 + t13664 + t13665)
        t13675 = t4 * (t3972 / 0.2E1 + t6764 - dy * ((t13667 - t3972) * 
     #t183 / 0.2E1 - t6779 / 0.2E1) / 0.8E1)
        t13679 = t4 * t13662
        t13684 = u(t5,t13639,k,n)
        t13685 = u(i,t13639,k,n)
        t13687 = (t13684 - t13685) * t94
        t13688 = u(t96,t13639,k,n)
        t13690 = (t13685 - t13688) * t94
        t13000 = t13679 * (t13640 * t13649 + t13653 * t13641 + t13651 * 
     #t13645)
        t13696 = (t13000 * (t13687 / 0.2E1 + t13690 / 0.2E1) - t3964) * 
     #t183
        t13719 = t3546 / 0.2E1
        t13729 = t4 * (t2963 / 0.2E1 + t13719 - dx * ((t2954 - t2963) * 
     #t94 / 0.2E1 - (t3546 - t3892) * t94 / 0.2E1) / 0.8E1)
        t13741 = t4 * (t13719 + t3892 / 0.2E1 - dx * ((t2963 - t3546) * 
     #t94 / 0.2E1 - (t3892 - t7476) * t94 / 0.2E1) / 0.8E1)
        t13745 = u(i,t13639,t233,n)
        t13747 = (t13745 - t3983) * t183
        t13756 = (t13685 - t1781) * t183
        t13039 = ((t13756 / 0.2E1 - t218 / 0.2E1) * t183 - t2039) * t183
        t13763 = t708 * t13039
        t13766 = u(i,t13639,t238,n)
        t13768 = (t13766 - t3986) * t183
        t13783 = (t13684 - t1361) * t183
        t13793 = t629 * t13039
        t13797 = (t13688 - t3904) * t183
        t13812 = t4109 / 0.2E1
        t13822 = t4 * (t4104 / 0.2E1 + t13812 - dz * ((t8548 - t4104) * 
     #t236 / 0.2E1 - (t4109 - t4118) * t236 / 0.2E1) / 0.8E1)
        t13834 = t4 * (t13812 + t4118 / 0.2E1 - dz * ((t4104 - t4109) * 
     #t236 / 0.2E1 - (t4118 - t8696) * t236 / 0.2E1) / 0.8E1)
        t13838 = -t1227 * (((t2994 - t3558) * t94 - t13627) * t94 / 0.2E
     #1 + (t13627 - (t3912 - t7496) * t94) * t94 / 0.2E1) / 0.6E1 + t391
     #3 + t3930 + t3967 + (t13675 * t2036 - t6776) * t183 + t3995 - t133
     #0 * (((t13696 - t3966) * t183 - t6751) * t183 / 0.2E1 + t6755 / 0.
     #2E1) / 0.6E1 + t4034 + t4071 + t4086 - t1227 * ((t3549 * t13534 - 
     #t3895 * t13538) * t94 + ((t3552 - t3898) * t94 - (t3898 - t7482) *
     # t94) * t94) / 0.24E2 + (t13729 * t311 - t13741 * t639) * t94 - t1
     #330 * ((t3800 * ((t13747 / 0.2E1 - t834 / 0.2E1) * t183 - t6433) *
     # t183 - t13763) * t236 / 0.2E1 + (t13763 - t3811 * ((t13768 / 0.2E
     #1 - t851 / 0.2E1) * t183 - t6448) * t183) * t236 / 0.2E1) / 0.6E1 
     #- t1330 * ((t306 * ((t13783 / 0.2E1 - t200 / 0.2E1) * t183 - t1947
     #) * t183 - t13793) * t94 / 0.2E1 + (t13793 - t3630 * ((t13797 / 0.
     #2E1 - t582 / 0.2E1) * t183 - t6528) * t183) * t94 / 0.2E1) / 0.6E1
     # + (t13822 * t719 - t13834 * t722) * t236
        t13842 = (t4085 - t4098) * t236
        t13858 = t3371 * t6320
        t13873 = (t3567 - t3929) * t94
        t13909 = t4 * (t13667 / 0.2E1 + t3972 / 0.2E1)
        t13912 = (t13909 * t13756 - t3976) * t183
        t13923 = (t719 - t722) * t236
        t13925 = ((t5186 - t719) * t236 - t13923) * t236
        t13930 = (t13923 - (t722 - t5384) * t236) * t236
        t13946 = (t4033 - t4070) * t236
        t13978 = (t13745 - t13685) * t236
        t13980 = (t13685 - t13766) * t236
        t13195 = t13679 * (t13649 * t13656 + t13641 * t13647 + t13645 * 
     #t13643)
        t13986 = (t13195 * (t13978 / 0.2E1 + t13980 / 0.2E1) - t3992) * 
     #t183
        t14007 = t3371 * t6304
        t13346 = ((t3124 / 0.2E1 - t4025 / 0.2E1) * t94 - (t3600 / 0.2E1
     # - t7609 / 0.2E1) * t94) * t94
        t13351 = ((t3165 / 0.2E1 - t4064 / 0.2E1) * t94 - (t3639 / 0.2E1
     # - t7648 / 0.2E1) * t94) * t94
        t14026 = -t1429 * (((t8542 - t4085) * t236 - t13842) * t236 / 0.
     #2E1 + (t13842 - (t4098 - t8690) * t236) * t236 / 0.2E1) / 0.6E1 - 
     #t1429 * ((t2760 * t1931 - t13858) * t94 / 0.2E1 + (t13858 - t3645 
     #* t9682) * t94 / 0.2E1) / 0.6E1 - t1227 * (((t3026 - t3567) * t94 
     #- t13873) * t94 / 0.2E1 + (t13873 - (t3929 - t7513) * t94) * t94 /
     # 0.2E1) / 0.6E1 - t1227 * ((t3682 * ((t1780 / 0.2E1 - t3960 / 0.2E
     #1) * t94 - (t1783 / 0.2E1 - t7544 / 0.2E1) * t94) * t94 - t6655) *
     # t183 / 0.2E1 + t6664 / 0.2E1) / 0.6E1 - t1330 * ((t3975 * ((t1375
     #6 - t2036) * t183 - t6409) * t183 - t6414) * t183 + ((t13912 - t39
     #78) * t183 - t6423) * t183) / 0.24E2 - t1429 * ((t4112 * t13925 - 
     #t4121 * t13930) * t236 + ((t8554 - t4124) * t236 - (t4124 - t8702)
     # * t236) * t236) / 0.24E2 - t1429 * (((t8529 - t4033) * t236 - t13
     #946) * t236 / 0.2E1 + (t13946 - (t4070 - t8677) * t236) * t236 / 0
     #.2E1) / 0.6E1 + t650 - t1429 * ((t3713 * ((t8486 / 0.2E1 - t3988 /
     # 0.2E1) * t236 - (t3985 / 0.2E1 - t8634 / 0.2E1) * t236) * t236 - 
     #t6690) * t183 / 0.2E1 + t6695 / 0.2E1) / 0.6E1 + t3559 + t3568 - t
     #1330 * (((t13986 - t3994) * t183 - t6821) * t183 / 0.2E1 + t6825 /
     # 0.2E1) / 0.6E1 + t4099 - t1227 * ((t3753 * t13346 - t14007) * t23
     #6 / 0.2E1 + (t14007 - t3787 * t13351) * t236 / 0.2E1) / 0.6E1 + t7
     #35
        t14028 = (t13838 + t14026) * t631
        t14031 = ut(i,t13639,k,n)
        t14033 = (t14031 - t2111) * t183
        t14037 = ((t14033 - t2113) * t183 - t6962) * t183
        t14044 = dy * (t2113 / 0.2E1 + t9306 - t1330 * (t14037 / 0.2E1 +
     # t6966 / 0.2E1) / 0.6E1) / 0.2E1
        t14052 = (t1141 - t1144) * t236
        t14054 = ((t7258 - t1141) * t236 - t14052) * t236
        t14059 = (t14052 - (t1144 - t7263) * t236) * t236
        t14065 = (t8551 * t7258 - t9563) * t236
        t14070 = (t9564 - t8699 * t7263) * t236
        t14094 = ut(i,t13639,t233,n)
        t14097 = ut(i,t13639,t238,n)
        t14105 = (t13195 * ((t14094 - t14031) * t236 / 0.2E1 + (t14031 -
     # t14097) * t236 / 0.2E1) - t6999) * t183
        t14114 = ut(t5,t13639,k,n)
        t14117 = ut(t96,t13639,k,n)
        t14125 = (t13000 * ((t14114 - t14031) * t94 / 0.2E1 + (t14031 - 
     #t14117) * t94 / 0.2E1) - t6930) * t183
        t14142 = (t13909 * t14033 - t6975) * t183
        t14150 = (t13729 * t975 - t13741 * t1114) * t94 - t1429 * ((t411
     #2 * t14054 - t4121 * t14059) * t236 + ((t14065 - t9566) * t236 - (
     #t9566 - t14070) * t236) * t236) / 0.24E2 - t1227 * ((t3682 * ((t25
     #89 / 0.2E1 - t6926 / 0.2E1) * t94 - (t2591 / 0.2E1 - t10677 / 0.2E
     #1) * t94) * t94 - t7057) * t183 / 0.2E1 + t7066 / 0.2E1) / 0.6E1 +
     # t1125 + t1153 + t9525 + t9363 + t9375 - t1330 * (((t14105 - t7001
     #) * t183 - t7003) * t183 / 0.2E1 + t7007 / 0.2E1) / 0.6E1 + t9526 
     #- t1330 * (((t14125 - t6932) * t183 - t6934) * t183 / 0.2E1 + t693
     #8 / 0.2E1) / 0.6E1 + (t13675 * t2113 - t6956) * t183 + t9519 + t95
     #24 - t1330 * ((t3975 * t14037 - t6967) * t183 + ((t14142 - t6977) 
     #* t183 - t6979) * t183) / 0.24E2
        t14163 = t3371 * t6585
        t14187 = (t2263 - t7112) * t94 / 0.2E1 + (t7112 - t10637) * t94 
     #/ 0.2E1
        t14191 = (t7928 * t14187 - t9532) * t236
        t14195 = (t9536 - t9545) * t236
        t14203 = (t2281 - t7130) * t94 / 0.2E1 + (t7130 - t10655) * t94 
     #/ 0.2E1
        t14207 = (t9543 - t8071 * t14203) * t236
        t14217 = (t14094 - t6990) * t183
        t13535 = ((t14033 / 0.2E1 - t925 / 0.2E1) * t183 - t2116) * t183
        t14231 = t708 * t13535
        t14235 = (t14097 - t6993) * t183
        t14249 = ut(i,t1331,t1430,n)
        t14251 = (t14249 - t7112) * t183
        t14257 = (t7939 * (t14251 / 0.2E1 + t7114 / 0.2E1) - t9550) * t2
     #36
        t14261 = (t9554 - t9561) * t236
        t14264 = ut(i,t1331,t1441,n)
        t14266 = (t14264 - t7130) * t183
        t14272 = (t9559 - t8091 * (t14266 / 0.2E1 + t7132 / 0.2E1)) * t2
     #36
        t14301 = (t9362 - t9518) * t94
        t14313 = (t14114 - t2093) * t183
        t14323 = t629 * t13535
        t14327 = (t14117 - t6924) * t183
        t14342 = (t14249 - t6990) * t236
        t14347 = (t6993 - t14264) * t236
        t14364 = (t9374 - t9523) * t94
        t14380 = t3371 * t6716
        t13561 = ((t9379 / 0.2E1 - t9528 / 0.2E1) * t94 - (t9381 / 0.2E1
     # - t13027 / 0.2E1) * t94) * t94
        t13568 = ((t9392 / 0.2E1 - t9539 / 0.2E1) * t94 - (t9394 / 0.2E1
     # - t13038 / 0.2E1) * t94) * t94
        t14392 = t9537 + t9546 + t9555 + t9562 - t1227 * ((t3753 * t1356
     #1 - t14163) * t236 / 0.2E1 + (t14163 - t3787 * t13568) * t236 / 0.
     #2E1) / 0.6E1 - t1429 * (((t14191 - t9536) * t236 - t14195) * t236 
     #/ 0.2E1 + (t14195 - (t9545 - t14207) * t236) * t236 / 0.2E1) / 0.6
     #E1 - t1330 * ((t3800 * ((t14217 / 0.2E1 - t1188 / 0.2E1) * t183 - 
     #t7204) * t183 - t14231) * t236 / 0.2E1 + (t14231 - t3811 * ((t1423
     #5 / 0.2E1 - t1201 / 0.2E1) * t183 - t7223) * t183) * t236 / 0.2E1)
     # / 0.6E1 - t1429 * (((t14257 - t9554) * t236 - t14261) * t236 / 0.
     #2E1 + (t14261 - (t9561 - t14272) * t236) * t236 / 0.2E1) / 0.6E1 -
     # t1227 * ((t3549 * t13433 - t3895 * t13437) * t94 + ((t9345 - t951
     #2) * t94 - (t9512 - t13011) * t94) * t94) / 0.24E2 + (t13822 * t11
     #41 - t13834 * t1144) * t236 - t1227 * (((t9355 - t9362) * t94 - t1
     #4301) * t94 / 0.2E1 + (t14301 - (t9518 - t13017) * t94) * t94 / 0.
     #2E1) / 0.6E1 - t1330 * ((t306 * ((t14313 / 0.2E1 - t912 / 0.2E1) *
     # t183 - t2098) * t183 - t14323) * t94 / 0.2E1 + (t14323 - t3630 * 
     #((t14327 / 0.2E1 - t1089 / 0.2E1) * t183 - t7154) * t183) * t94 / 
     #0.2E1) / 0.6E1 - t1429 * ((t3713 * ((t14342 / 0.2E1 - t6995 / 0.2E
     #1) * t236 - (t6992 / 0.2E1 - t14347 / 0.2E1) * t236) * t236 - t727
     #0) * t183 / 0.2E1 + t7275 / 0.2E1) / 0.6E1 - t1227 * (((t9369 - t9
     #374) * t94 - t14364) * t94 / 0.2E1 + (t14364 - (t9523 - t13022) * 
     #t94) * t94 / 0.2E1) / 0.6E1 - t1429 * ((t2760 * t2383 - t14380) * 
     #t94 / 0.2E1 + (t14380 - t3645 * t10457) * t94 / 0.2E1) / 0.6E1
        t14394 = (t14150 + t14392) * t631
        t14397 = t1332 ** 2
        t14398 = t1345 ** 2
        t14399 = t1343 ** 2
        t14401 = t1354 * (t14397 + t14398 + t14399)
        t14402 = t3931 ** 2
        t14403 = t3944 ** 2
        t14404 = t3942 ** 2
        t14406 = t3953 * (t14402 + t14403 + t14404)
        t14409 = t4 * (t14401 / 0.2E1 + t14406 / 0.2E1)
        t14410 = t14409 * t1783
        t14411 = t7515 ** 2
        t14412 = t7528 ** 2
        t14413 = t7526 ** 2
        t14415 = t7537 * (t14411 + t14412 + t14413)
        t14418 = t4 * (t14406 / 0.2E1 + t14415 / 0.2E1)
        t14419 = t14418 * t3960
        t14423 = t13783 / 0.2E1 + t1858 / 0.2E1
        t14425 = t1648 * t14423
        t14427 = t13756 / 0.2E1 + t2036 / 0.2E1
        t14429 = t3682 * t14427
        t14432 = (t14425 - t14429) * t94 / 0.2E1
        t14434 = t13797 / 0.2E1 + t3906 / 0.2E1
        t14436 = t7034 * t14434
        t14439 = (t14429 - t14436) * t94 / 0.2E1
        t13776 = t1355 * (t1332 * t1348 + t1345 * t1339 + t1343 * t1335)
        t14445 = t13776 * t1368
        t13780 = t3954 * (t3931 * t3947 + t3944 * t3938 + t3942 * t3934)
        t14451 = t13780 * t3990
        t14454 = (t14445 - t14451) * t94 / 0.2E1
        t13787 = t7538 * (t7515 * t7531 + t7528 * t7522 + t7526 * t7518)
        t14460 = t13787 * t7574
        t14463 = (t14451 - t14460) * t94 / 0.2E1
        t13794 = t8455 * (t8432 * t8448 + t8439 * t8445 + t8443 * t8435)
        t14471 = t13794 * t8463
        t14473 = t13780 * t3962
        t14476 = (t14471 - t14473) * t236 / 0.2E1
        t13801 = t8603 * (t8580 * t8596 + t8593 * t8587 + t8591 * t8583)
        t14482 = t13801 * t8611
        t14485 = (t14473 - t14482) * t236 / 0.2E1
        t14487 = t13747 / 0.2E1 + t4077 / 0.2E1
        t14489 = t7897 * t14487
        t14491 = t3713 * t14427
        t14494 = (t14489 - t14491) * t236 / 0.2E1
        t14496 = t13768 / 0.2E1 + t4092 / 0.2E1
        t14498 = t8035 * t14496
        t14501 = (t14491 - t14498) * t236 / 0.2E1
        t14502 = t8448 ** 2
        t14503 = t8439 ** 2
        t14504 = t8435 ** 2
        t14506 = t8454 * (t14502 + t14503 + t14504)
        t14507 = t3947 ** 2
        t14508 = t3938 ** 2
        t14509 = t3934 ** 2
        t14511 = t3953 * (t14507 + t14508 + t14509)
        t14514 = t4 * (t14506 / 0.2E1 + t14511 / 0.2E1)
        t14515 = t14514 * t3985
        t14516 = t8596 ** 2
        t14517 = t8587 ** 2
        t14518 = t8583 ** 2
        t14520 = t8602 * (t14516 + t14517 + t14518)
        t14523 = t4 * (t14511 / 0.2E1 + t14520 / 0.2E1)
        t14524 = t14523 * t3988
        t14527 = (t14410 - t14419) * t94 + t14432 + t14439 + t14454 + t1
     #4463 + t13696 / 0.2E1 + t3967 + t13912 + t13986 / 0.2E1 + t3995 + 
     #t14476 + t14485 + t14494 + t14501 + (t14515 - t14524) * t236
        t14528 = t14527 * t3952
        t14530 = (t14528 - t4126) * t183
        t14532 = t14530 / 0.2E1 + t4128 / 0.2E1
        t14533 = dy * t14532
        t14541 = t1330 * (t6962 - dy * (t14037 - t6966) / 0.12E2) / 0.12
     #E2
        t14546 = t3028 ** 2
        t14547 = t3041 ** 2
        t14548 = t3039 ** 2
        t14557 = u(t64,t13639,k,n)
        t14576 = rx(t5,t13639,k,0,0)
        t14577 = rx(t5,t13639,k,1,1)
        t14579 = rx(t5,t13639,k,2,2)
        t14581 = rx(t5,t13639,k,1,2)
        t14583 = rx(t5,t13639,k,2,1)
        t14585 = rx(t5,t13639,k,1,0)
        t14587 = rx(t5,t13639,k,0,2)
        t14589 = rx(t5,t13639,k,0,1)
        t14592 = rx(t5,t13639,k,2,0)
        t14598 = 0.1E1 / (t14576 * t14577 * t14579 - t14576 * t14581 * t
     #14583 + t14585 * t14583 * t14587 - t14585 * t14589 * t14579 + t145
     #92 * t14589 * t14581 - t14592 * t14577 * t14587)
        t14599 = t4 * t14598
        t14613 = t14585 ** 2
        t14614 = t14577 ** 2
        t14615 = t14581 ** 2
        t14628 = u(t5,t13639,t233,n)
        t14631 = u(t5,t13639,t238,n)
        t14648 = t13776 * t1785
        t14664 = (t14628 - t1360) * t183 / 0.2E1 + t1932 / 0.2E1
        t14668 = t1240 * t14423
        t14675 = (t14631 - t1364) * t183 / 0.2E1 + t1958 / 0.2E1
        t14681 = t5571 ** 2
        t14682 = t5562 ** 2
        t14683 = t5558 ** 2
        t14686 = t1348 ** 2
        t14687 = t1339 ** 2
        t14688 = t1335 ** 2
        t14690 = t1354 * (t14686 + t14687 + t14688)
        t14695 = t5751 ** 2
        t14696 = t5742 ** 2
        t14697 = t5738 ** 2
        t13928 = t5578 * (t5555 * t5571 + t5568 * t5562 + t5566 * t5558)
        t13934 = t5758 * (t5735 * t5751 + t5748 * t5742 + t5746 * t5738)
        t14706 = (t4 * (t3050 * (t14546 + t14547 + t14548) / 0.2E1 + t14
     #401 / 0.2E1) * t1780 - t14410) * t94 + (t2973 * ((t14557 - t1778) 
     #* t183 / 0.2E1 + t2017 / 0.2E1) - t14425) * t94 / 0.2E1 + t14432 +
     # (t3051 * (t3028 * t3044 + t3041 * t3035 + t3039 * t3031) * t3087 
     #- t14445) * t94 / 0.2E1 + t14454 + (t14599 * (t14576 * t14585 + t1
     #4589 * t14577 + t14587 * t14581) * ((t14557 - t13684) * t94 / 0.2E
     #1 + t13687 / 0.2E1) - t1787) * t183 / 0.2E1 + t3569 + (t4 * (t1459
     #8 * (t14613 + t14614 + t14615) / 0.2E1 + t1826 / 0.2E1) * t13783 -
     # t1878) * t183 + (t14599 * (t14585 * t14592 + t14577 * t14583 + t1
     #4581 * t14579) * ((t14628 - t13684) * t236 / 0.2E1 + (t13684 - t14
     #631) * t236 / 0.2E1) - t1370) * t183 / 0.2E1 + t3570 + (t13928 * t
     #5588 - t14648) * t236 / 0.2E1 + (t14648 - t13934 * t5768) * t236 /
     # 0.2E1 + (t5267 * t14664 - t14668) * t236 / 0.2E1 + (t14668 - t549
     #8 * t14675) * t236 / 0.2E1 + (t4 * (t5577 * (t14681 + t14682 + t14
     #683) / 0.2E1 + t14690 / 0.2E1) * t1363 - t4 * (t14690 / 0.2E1 + t5
     #757 * (t14695 + t14696 + t14697) / 0.2E1) * t1366) * t236
        t14707 = t14706 * t1353
        t14715 = t629 * t14532
        t14719 = t11237 ** 2
        t14720 = t11250 ** 2
        t14721 = t11248 ** 2
        t14730 = u(t6542,t13639,k,n)
        t14749 = rx(t96,t13639,k,0,0)
        t14750 = rx(t96,t13639,k,1,1)
        t14752 = rx(t96,t13639,k,2,2)
        t14754 = rx(t96,t13639,k,1,2)
        t14756 = rx(t96,t13639,k,2,1)
        t14758 = rx(t96,t13639,k,1,0)
        t14760 = rx(t96,t13639,k,0,2)
        t14762 = rx(t96,t13639,k,0,1)
        t14765 = rx(t96,t13639,k,2,0)
        t14771 = 0.1E1 / (t14749 * t14750 * t14752 - t14749 * t14754 * t
     #14756 + t14758 * t14756 * t14760 - t14758 * t14762 * t14752 + t147
     #65 * t14762 * t14754 - t14765 * t14750 * t14760)
        t14772 = t4 * t14771
        t14786 = t14758 ** 2
        t14787 = t14750 ** 2
        t14788 = t14754 ** 2
        t14801 = u(t96,t13639,t233,n)
        t14804 = u(t96,t13639,t238,n)
        t14821 = t13787 * t7546
        t14837 = (t14801 - t7567) * t183 / 0.2E1 + t7661 / 0.2E1
        t14841 = t7055 * t14434
        t14848 = (t14804 - t7570) * t183 / 0.2E1 + t7676 / 0.2E1
        t14854 = t12170 ** 2
        t14855 = t12161 ** 2
        t14856 = t12157 ** 2
        t14859 = t7531 ** 2
        t14860 = t7522 ** 2
        t14861 = t7518 ** 2
        t14863 = t7537 * (t14859 + t14860 + t14861)
        t14868 = t12318 ** 2
        t14869 = t12309 ** 2
        t14870 = t12305 ** 2
        t14066 = t12177 * (t12154 * t12170 + t12167 * t12161 + t12165 * 
     #t12157)
        t14072 = t12325 * (t12302 * t12318 + t12315 * t12309 + t12313 * 
     #t12305)
        t14879 = (t14419 - t4 * (t14415 / 0.2E1 + t11259 * (t14719 + t14
     #720 + t14721) / 0.2E1) * t7544) * t94 + t14439 + (t14436 - t10717 
     #* ((t14730 - t7488) * t183 / 0.2E1 + t7490 / 0.2E1)) * t94 / 0.2E1
     # + t14463 + (t14460 - t11260 * (t11237 * t11253 + t11250 * t11244 
     #+ t11248 * t11240) * t11296) * t94 / 0.2E1 + (t14772 * (t14749 * t
     #14758 + t14762 * t14750 + t14760 * t14754) * (t13690 / 0.2E1 + (t1
     #3688 - t14730) * t94 / 0.2E1) - t7548) * t183 / 0.2E1 + t7551 + (t
     #4 * (t14771 * (t14786 + t14787 + t14788) / 0.2E1 + t7556 / 0.2E1) 
     #* t13797 - t7560) * t183 + (t14772 * (t14758 * t14765 + t14750 * t
     #14756 + t14754 * t14752) * ((t14801 - t13688) * t236 / 0.2E1 + (t1
     #3688 - t14804) * t236 / 0.2E1) - t7576) * t183 / 0.2E1 + t7579 + (
     #t14066 * t12185 - t14821) * t236 / 0.2E1 + (t14821 - t14072 * t123
     #33) * t236 / 0.2E1 + (t11545 * t14837 - t14841) * t236 / 0.2E1 + (
     #t14841 - t11692 * t14848) * t236 / 0.2E1 + (t4 * (t12176 * (t14854
     # + t14855 + t14856) / 0.2E1 + t14863 / 0.2E1) * t7569 - t4 * (t148
     #63 / 0.2E1 + t12324 * (t14868 + t14869 + t14870) / 0.2E1) * t7572)
     # * t236
        t14880 = t14879 * t7536
        t14893 = t3371 * t8708
        t14916 = t5555 ** 2
        t14917 = t5568 ** 2
        t14918 = t5566 ** 2
        t14921 = t8432 ** 2
        t14922 = t8445 ** 2
        t14923 = t8443 ** 2
        t14925 = t8454 * (t14921 + t14922 + t14923)
        t14930 = t12154 ** 2
        t14931 = t12167 ** 2
        t14932 = t12165 ** 2
        t14944 = t7874 * t14487
        t14956 = t13794 * t8488
        t14965 = rx(i,t13639,t233,0,0)
        t14966 = rx(i,t13639,t233,1,1)
        t14968 = rx(i,t13639,t233,2,2)
        t14970 = rx(i,t13639,t233,1,2)
        t14972 = rx(i,t13639,t233,2,1)
        t14974 = rx(i,t13639,t233,1,0)
        t14976 = rx(i,t13639,t233,0,2)
        t14978 = rx(i,t13639,t233,0,1)
        t14981 = rx(i,t13639,t233,2,0)
        t14987 = 0.1E1 / (t14965 * t14966 * t14968 - t14965 * t14970 * t
     #14972 + t14974 * t14972 * t14976 - t14974 * t14978 * t14968 + t149
     #81 * t14978 * t14970 - t14981 * t14966 * t14976)
        t14988 = t4 * t14987
        t15004 = t14974 ** 2
        t15005 = t14966 ** 2
        t15006 = t14970 ** 2
        t15019 = u(i,t13639,t1430,n)
        t15029 = rx(i,t1331,t1430,0,0)
        t15030 = rx(i,t1331,t1430,1,1)
        t15032 = rx(i,t1331,t1430,2,2)
        t15034 = rx(i,t1331,t1430,1,2)
        t15036 = rx(i,t1331,t1430,2,1)
        t15038 = rx(i,t1331,t1430,1,0)
        t15040 = rx(i,t1331,t1430,0,2)
        t15042 = rx(i,t1331,t1430,0,1)
        t15045 = rx(i,t1331,t1430,2,0)
        t15051 = 0.1E1 / (t15029 * t15030 * t15032 - t15029 * t15034 * t
     #15036 + t15038 * t15036 * t15040 - t15038 * t15042 * t15032 + t150
     #45 * t15042 * t15034 - t15045 * t15030 * t15040)
        t15052 = t4 * t15051
        t15062 = (t5609 - t8484) * t94 / 0.2E1 + (t8484 - t12206) * t94 
     #/ 0.2E1
        t15081 = t15045 ** 2
        t15082 = t15036 ** 2
        t15083 = t15032 ** 2
        t14226 = t15052 * (t15038 * t15045 + t15030 * t15036 + t15034 * 
     #t15032)
        t15092 = (t4 * (t5577 * (t14916 + t14917 + t14918) / 0.2E1 + t14
     #925 / 0.2E1) * t5586 - t4 * (t14925 / 0.2E1 + t12176 * (t14930 + t
     #14931 + t14932) / 0.2E1) * t8461) * t94 + (t5238 * t14664 - t14944
     #) * t94 / 0.2E1 + (t14944 - t11529 * t14837) * t94 / 0.2E1 + (t139
     #28 * t5613 - t14956) * t94 / 0.2E1 + (t14956 - t14066 * t12210) * 
     #t94 / 0.2E1 + (t14988 * (t14965 * t14974 + t14978 * t14966 + t1497
     #6 * t14970) * ((t14628 - t13745) * t94 / 0.2E1 + (t13745 - t14801)
     # * t94 / 0.2E1) - t8465) * t183 / 0.2E1 + t8468 + (t4 * (t14987 * 
     #(t15004 + t15005 + t15006) / 0.2E1 + t8473 / 0.2E1) * t13747 - t84
     #77) * t183 + (t14988 * (t14974 * t14981 + t14966 * t14972 + t14970
     # * t14968) * ((t15019 - t13745) * t236 / 0.2E1 + t13978 / 0.2E1) -
     # t8490) * t183 / 0.2E1 + t8493 + (t15052 * (t15029 * t15045 + t150
     #42 * t15036 + t15040 * t15032) * t15062 - t14471) * t236 / 0.2E1 +
     # t14476 + (t14226 * ((t15019 - t8484) * t183 / 0.2E1 + t8536 / 0.2
     #E1) - t14489) * t236 / 0.2E1 + t14494 + (t4 * (t15051 * (t15081 + 
     #t15082 + t15083) / 0.2E1 + t14506 / 0.2E1) * t8486 - t14515) * t23
     #6
        t15093 = t15092 * t8453
        t15096 = t5735 ** 2
        t15097 = t5748 ** 2
        t15098 = t5746 ** 2
        t15101 = t8580 ** 2
        t15102 = t8593 ** 2
        t15103 = t8591 ** 2
        t15105 = t8602 * (t15101 + t15102 + t15103)
        t15110 = t12302 ** 2
        t15111 = t12315 ** 2
        t15112 = t12313 ** 2
        t15124 = t8013 * t14496
        t15136 = t13801 * t8636
        t15145 = rx(i,t13639,t238,0,0)
        t15146 = rx(i,t13639,t238,1,1)
        t15148 = rx(i,t13639,t238,2,2)
        t15150 = rx(i,t13639,t238,1,2)
        t15152 = rx(i,t13639,t238,2,1)
        t15154 = rx(i,t13639,t238,1,0)
        t15156 = rx(i,t13639,t238,0,2)
        t15158 = rx(i,t13639,t238,0,1)
        t15161 = rx(i,t13639,t238,2,0)
        t15167 = 0.1E1 / (t15145 * t15146 * t15148 - t15145 * t15150 * t
     #15152 + t15154 * t15152 * t15156 - t15154 * t15158 * t15148 + t151
     #61 * t15158 * t15150 - t15161 * t15146 * t15156)
        t15168 = t4 * t15167
        t15184 = t15154 ** 2
        t15185 = t15146 ** 2
        t15186 = t15150 ** 2
        t15199 = u(i,t13639,t1441,n)
        t15209 = rx(i,t1331,t1441,0,0)
        t15210 = rx(i,t1331,t1441,1,1)
        t15212 = rx(i,t1331,t1441,2,2)
        t15214 = rx(i,t1331,t1441,1,2)
        t15216 = rx(i,t1331,t1441,2,1)
        t15218 = rx(i,t1331,t1441,1,0)
        t15220 = rx(i,t1331,t1441,0,2)
        t15222 = rx(i,t1331,t1441,0,1)
        t15225 = rx(i,t1331,t1441,2,0)
        t15231 = 0.1E1 / (t15209 * t15210 * t15212 - t15209 * t15214 * t
     #15216 + t15218 * t15216 * t15220 - t15218 * t15222 * t15212 + t152
     #25 * t15222 * t15214 - t15225 * t15210 * t15220)
        t15232 = t4 * t15231
        t15242 = (t5789 - t8632) * t94 / 0.2E1 + (t8632 - t12354) * t94 
     #/ 0.2E1
        t15261 = t15225 ** 2
        t15262 = t15216 ** 2
        t15263 = t15212 ** 2
        t14370 = t15232 * (t15225 * t15218 + t15210 * t15216 + t15214 * 
     #t15212)
        t15272 = (t4 * (t5757 * (t15096 + t15097 + t15098) / 0.2E1 + t15
     #105 / 0.2E1) * t5766 - t4 * (t15105 / 0.2E1 + t12324 * (t15110 + t
     #15111 + t15112) / 0.2E1) * t8609) * t94 + (t5483 * t14675 - t15124
     #) * t94 / 0.2E1 + (t15124 - t11669 * t14848) * t94 / 0.2E1 + (t139
     #34 * t5793 - t15136) * t94 / 0.2E1 + (t15136 - t14072 * t12358) * 
     #t94 / 0.2E1 + (t15168 * (t15145 * t15154 + t15158 * t15146 + t1515
     #6 * t15150) * ((t14631 - t13766) * t94 / 0.2E1 + (t13766 - t14804)
     # * t94 / 0.2E1) - t8613) * t183 / 0.2E1 + t8616 + (t4 * (t15167 * 
     #(t15184 + t15185 + t15186) / 0.2E1 + t8621 / 0.2E1) * t13768 - t86
     #25) * t183 + (t15168 * (t15154 * t15161 + t15146 * t15152 + t15150
     # * t15148) * (t13980 / 0.2E1 + (t13766 - t15199) * t236 / 0.2E1) -
     # t8638) * t183 / 0.2E1 + t8641 + t14485 + (t14482 - t15232 * (t152
     #09 * t15225 + t15222 * t15216 + t15220 * t15212) * t15242) * t236 
     #/ 0.2E1 + t14501 + (t14498 - t14370 * ((t15199 - t8632) * t183 / 0
     #.2E1 + t8684 / 0.2E1)) * t236 / 0.2E1 + (t14524 - t4 * (t14520 / 0
     #.2E1 + t15231 * (t15261 + t15262 + t15263) / 0.2E1) * t8634) * t23
     #6
        t15273 = t15272 * t8601
        t15288 = (t5683 - t8556) * t94 / 0.2E1 + (t8556 - t12278) * t94 
     #/ 0.2E1
        t15292 = t3371 * t8390
        t15301 = (t5863 - t8704) * t94 / 0.2E1 + (t8704 - t12426) * t94 
     #/ 0.2E1
        t15314 = t708 * t14532
        t15331 = (t3549 * t5481 - t3895 * t8388) * t94 + (t306 * ((t1470
     #7 - t3697) * t183 / 0.2E1 + t3699 / 0.2E1) - t14715) * t94 / 0.2E1
     # + (t14715 - t3630 * ((t14880 - t7710) * t183 / 0.2E1 + t7712 / 0.
     #2E1)) * t94 / 0.2E1 + (t2760 * t5867 - t14893) * t94 / 0.2E1 + (t1
     #4893 - t3645 * t12430) * t94 / 0.2E1 + (t3682 * ((t14707 - t14528)
     # * t94 / 0.2E1 + (t14528 - t14880) * t94 / 0.2E1) - t8392) * t183 
     #/ 0.2E1 + t8397 + (t3975 * t14530 - t8407) * t183 + (t3713 * ((t15
     #093 - t14528) * t236 / 0.2E1 + (t14528 - t15273) * t236 / 0.2E1) -
     # t8710) * t183 / 0.2E1 + t8715 + (t3753 * t15288 - t15292) * t236 
     #/ 0.2E1 + (t15292 - t3787 * t15301) * t236 / 0.2E1 + (t3800 * ((t1
     #5093 - t8556) * t183 / 0.2E1 + t9040 / 0.2E1) - t15314) * t236 / 0
     #.2E1 + (t15314 - t3811 * ((t15273 - t8704) * t183 / 0.2E1 + t9053 
     #/ 0.2E1)) * t236 / 0.2E1 + (t4112 * t8558 - t4121 * t8706) * t236
        t15332 = t15331 * t631
        t15344 = t14033 / 0.2E1 + t2113 / 0.2E1
        t15346 = t3682 * t15344
        t15360 = t13780 * t6997
        t15376 = (t2345 - t6990) * t94 / 0.2E1 + (t6990 - t10740) * t94 
     #/ 0.2E1
        t15380 = t13780 * t6928
        t15389 = (t2348 - t6993) * t94 / 0.2E1 + (t6993 - t10761) * t94 
     #/ 0.2E1
        t15400 = t3713 * t15344
        t15415 = (t14409 * t2591 - t14418 * t6926) * t94 + (t1648 * (t14
     #313 / 0.2E1 + t2095 / 0.2E1) - t15346) * t94 / 0.2E1 + (t15346 - t
     #7034 * (t14327 / 0.2E1 + t7151 / 0.2E1)) * t94 / 0.2E1 + (t13776 *
     # t2352 - t15360) * t94 / 0.2E1 + (t15360 - t13787 * t10989) * t94 
     #/ 0.2E1 + t14125 / 0.2E1 + t9525 + t14142 + t14105 / 0.2E1 + t9526
     # + (t13794 * t15376 - t15380) * t236 / 0.2E1 + (t15380 - t13801 * 
     #t15389) * t236 / 0.2E1 + (t7897 * (t14217 / 0.2E1 + t7201 / 0.2E1)
     # - t15400) * t236 / 0.2E1 + (t15400 - t8035 * (t14235 / 0.2E1 + t7
     #220 / 0.2E1)) * t236 / 0.2E1 + (t14514 * t6992 - t14523 * t6995) *
     # t236
        t15421 = dy * ((t15415 * t3952 - t9568) * t183 / 0.2E1 + t9570 /
     # 0.2E1)
        t15425 = dy * (t14530 - t4128)
        t15430 = dy * (t9306 + t9307 - t9308) / 0.2E1
        t15431 = dy * t4394
        t15433 = t136 * t15431 / 0.2E1
        t15439 = t1330 * (t6964 - dy * (t6966 - t6971) / 0.12E2) / 0.12E
     #2
        t15442 = dy * (t9570 / 0.2E1 + t9631 / 0.2E1)
        t15444 = t2076 * t15442 / 0.4E1
        t15446 = dy * (t4128 - t4392)
        t15448 = t136 * t15446 / 0.12E2
        t15449 = t923 + t136 * t14028 - t14044 + t2076 * t14394 / 0.2E1 
     #- t136 * t14533 / 0.2E1 + t14541 + t2917 * t15332 / 0.6E1 - t2076 
     #* t15421 / 0.4E1 + t136 * t15425 / 0.12E2 - t2 - t6837 - t15430 - 
     #t7300 - t15433 - t15439 - t9070 - t15444 - t15448
        t15453 = 0.8E1 * t693
        t15454 = 0.8E1 * t694
        t15455 = 0.8E1 * t695
        t15465 = sqrt(0.8E1 * t688 + 0.8E1 * t689 + 0.8E1 * t690 + t1545
     #3 + t15454 + t15455 - 0.2E1 * dy * ((t3968 + t3969 + t3970 - t688 
     #- t689 - t690) * t183 / 0.2E1 - (t693 + t694 + t695 - t702 - t703 
     #- t704) * t183 / 0.2E1))
        t15466 = 0.1E1 / t15465
        t15471 = t6775 * t9202 * t13607
        t15474 = t700 * t9205 * t13612 / 0.2E1
        t15477 = t700 * t9209 * t13617 / 0.6E1
        t15479 = t9202 * t13620 / 0.24E2
        t15492 = t9202 * t15431 / 0.2E1
        t15494 = t9216 * t15442 / 0.4E1
        t15496 = t9202 * t15446 / 0.12E2
        t15497 = t923 + t9202 * t14028 - t14044 + t9216 * t14394 / 0.2E1
     # - t9202 * t14533 / 0.2E1 + t14541 + t9221 * t15332 / 0.6E1 - t921
     #6 * t15421 / 0.4E1 + t9202 * t15425 / 0.12E2 - t2 - t9228 - t15430
     # - t9230 - t15492 - t15439 - t9234 - t15494 - t15496
        t15500 = 0.2E1 * t13623 * t15497 * t15466
        t15502 = (t6775 * t136 * t13607 + t700 * t159 * t13612 / 0.2E1 +
     # t700 * t893 * t13617 / 0.6E1 - t136 * t13620 / 0.24E2 + 0.2E1 * t
     #13623 * t15449 * t15466 - t15471 - t15474 - t15477 + t15479 - t155
     #00) * t133
        t15508 = t6775 * (t218 - dy * t6412 / 0.24E2)
        t15510 = dy * t6422 / 0.24E2
        t15515 = t632 * t716
        t15517 = t57 * t730
        t15518 = t15517 / 0.2E1
        t15522 = t673 * t739
        t15530 = t4 * (t15515 / 0.2E1 + t15518 - dy * ((t3953 * t3982 - 
     #t15515) * t183 / 0.2E1 - (t15517 - t15522) * t183 / 0.2E1) / 0.8E1
     #)
        t15535 = t1429 * (t14054 / 0.2E1 + t14059 / 0.2E1)
        t15542 = (t6992 - t6995) * t236
        t15553 = t1141 / 0.2E1
        t15554 = t1144 / 0.2E1
        t15555 = t15535 / 0.6E1
        t15558 = t1156 / 0.2E1
        t15559 = t1159 / 0.2E1
        t15563 = (t1156 - t1159) * t236
        t15565 = ((t7277 - t1156) * t236 - t15563) * t236
        t15569 = (t15563 - (t1159 - t7282) * t236) * t236
        t15572 = t1429 * (t15565 / 0.2E1 + t15569 / 0.2E1)
        t15573 = t15572 / 0.6E1
        t15580 = t1141 / 0.4E1 + t1144 / 0.4E1 - t15535 / 0.12E2 + t9746
     # + t9747 - t9751 - dy * ((t6992 / 0.2E1 + t6995 / 0.2E1 - t1429 * 
     #(((t14342 - t6992) * t236 - t15542) * t236 / 0.2E1 + (t15542 - (t6
     #995 - t14347) * t236) * t236 / 0.2E1) / 0.6E1 - t15553 - t15554 + 
     #t15555) * t183 / 0.2E1 - (t9773 + t9774 - t9775 - t15558 - t15559 
     #+ t15573) * t183 / 0.2E1) / 0.8E1
        t15585 = t4 * (t15515 / 0.2E1 + t15517 / 0.2E1)
        t15587 = t8558 / 0.4E1 + t8706 / 0.4E1 + t5272 / 0.4E1 + t5470 /
     # 0.4E1
        t15598 = t4817 * t9548
        t15610 = t3753 * t9985
        t15622 = (t7874 * t15376 - t9969) * t183
        t15626 = (t8476 * t7201 - t9980) * t183
        t15632 = (t7897 * (t14342 / 0.2E1 + t6992 / 0.2E1) - t9987) * t1
     #83
        t15636 = (t5527 * t9381 - t8418 * t9528) * t94 + (t4678 * t9403 
     #- t15598) * t94 / 0.2E1 + (t15598 - t7498 * t13047) * t94 / 0.2E1 
     #+ (t3396 * t9860 - t15610) * t94 / 0.2E1 + (t15610 - t7086 * t1327
     #4) * t94 / 0.2E1 + t15622 / 0.2E1 + t9974 + t15626 + t15632 / 0.2E
     #1 + t9992 + t14191 / 0.2E1 + t9537 + t14257 / 0.2E1 + t9555 + t140
     #65
        t15637 = t15636 * t4017
        t15647 = t4956 * t9557
        t15659 = t3787 * t10038
        t15671 = (t8013 * t15389 - t10022) * t183
        t15675 = (t8624 * t7220 - t10033) * t183
        t15681 = (t8035 * (t6995 / 0.2E1 + t14347 / 0.2E1) - t10040) * t
     #183
        t15685 = (t5707 * t9394 - t8566 * t9539) * t94 + (t4728 * t9412 
     #- t15647) * t94 / 0.2E1 + (t15647 - t7675 * t13056) * t94 / 0.2E1 
     #+ (t3427 * t9932 - t15659) * t94 / 0.2E1 + (t15659 - t7122 * t1332
     #7) * t94 / 0.2E1 + t15671 / 0.2E1 + t10027 + t15675 + t15681 / 0.2
     #E1 + t10045 + t9546 + t14207 / 0.2E1 + t9562 + t14272 / 0.2E1 + t1
     #4070
        t15686 = t15685 * t4056
        t15690 = (t15637 - t9568) * t236 / 0.4E1 + (t9568 - t15686) * t2
     #36 / 0.4E1 + t10005 / 0.4E1 + t10058 / 0.4E1
        t15696 = dy * (t7001 / 0.2E1 - t1165 / 0.2E1)
        t15700 = t15530 * t9202 * t15580
        t15703 = t15585 * t9216 * t15587 / 0.2E1
        t15706 = t15585 * t9221 * t15690 / 0.6E1
        t15708 = t9202 * t15696 / 0.24E2
        t15710 = (t15530 * t136 * t15580 + t15585 * t2076 * t15587 / 0.2
     #E1 + t15585 * t2917 * t15690 / 0.6E1 - t136 * t15696 / 0.24E2 - t1
     #5700 - t15703 - t15706 + t15708) * t133
        t15717 = t1429 * (t13925 / 0.2E1 + t13930 / 0.2E1)
        t15724 = (t3985 - t3988) * t236
        t15735 = t719 / 0.2E1
        t15736 = t722 / 0.2E1
        t15737 = t15717 / 0.6E1
        t15740 = t742 / 0.2E1
        t15741 = t745 / 0.2E1
        t15745 = (t742 - t745) * t236
        t15747 = ((t5198 - t742) * t236 - t15745) * t236
        t15751 = (t15745 - (t745 - t5396) * t236) * t236
        t15754 = t1429 * (t15747 / 0.2E1 + t15751 / 0.2E1)
        t15755 = t15754 / 0.6E1
        t15763 = t15530 * (t719 / 0.4E1 + t722 / 0.4E1 - t15717 / 0.12E2
     # + t10089 + t10090 - t10094 - dy * ((t3985 / 0.2E1 + t3988 / 0.2E1
     # - t1429 * (((t8486 - t3985) * t236 - t15724) * t236 / 0.2E1 + (t1
     #5724 - (t3988 - t8634) * t236) * t236 / 0.2E1) / 0.6E1 - t15735 - 
     #t15736 + t15737) * t183 / 0.2E1 - (t10116 + t10117 - t10118 - t157
     #40 - t15741 + t15755) * t183 / 0.2E1) / 0.8E1)
        t15767 = dy * (t3994 / 0.2E1 - t751 / 0.2E1) / 0.24E2
        t15783 = t4 * (t9260 + t13417 / 0.2E1 - dy * ((t13412 - t9259) *
     # t183 / 0.2E1 - (t13417 - t4217 * t4222) * t183 / 0.2E1) / 0.8E1)
        t15794 = (t2607 - t6941) * t94
        t15811 = t13442 + t13443 - t13444 + t990 / 0.4E1 + t1127 / 0.4E1
     # - t13480 / 0.12E2 - dy * ((t13461 + t13462 - t13463 - t2060 - t68
     #38 + t6849) * t183 / 0.2E1 - (t13466 + t13467 - t13481 - t2607 / 0
     #.2E1 - t6941 / 0.2E1 + t1227 * (((t2605 - t2607) * t94 - t15794) *
     # t94 / 0.2E1 + (t15794 - (t6941 - t10692) * t94) * t94 / 0.2E1) / 
     #0.6E1) * t183 / 0.2E1) / 0.8E1
        t15816 = t4 * (t9259 / 0.2E1 + t13417 / 0.2E1)
        t15818 = t2905 / 0.4E1 + t7434 / 0.4E1 + t5494 / 0.4E1 + t8399 /
     # 0.4E1
        t15827 = t6398 / 0.4E1 + t9170 / 0.4E1 + (t9507 - t9629) * t94 /
     # 0.4E1 + (t9629 - t13128) * t94 / 0.4E1
        t15833 = dy * (t1124 / 0.2E1 - t6947 / 0.2E1)
        t15837 = t15783 * t9202 * t15811
        t15840 = t15816 * t9216 * t15818 / 0.2E1
        t15843 = t15816 * t9221 * t15827 / 0.6E1
        t15845 = t9202 * t15833 / 0.24E2
        t15847 = (t15783 * t136 * t15811 + t15816 * t2076 * t15818 / 0.2
     #E1 + t15816 * t2917 * t15827 / 0.6E1 - t136 * t15833 / 0.24E2 - t1
     #5837 - t15840 - t15843 + t15845) * t133
        t15860 = (t1805 - t4224) * t94
        t15878 = t15783 * (t13543 + t13544 - t13548 + t354 / 0.4E1 + t68
     #0 / 0.4E1 - t13587 / 0.12E2 - dy * ((t13565 + t13566 - t13567 - t1
     #3570 - t13571 + t13572) * t183 / 0.2E1 - (t13573 + t13574 - t13588
     # - t1805 / 0.2E1 - t4224 / 0.2E1 + t1227 * (((t1802 - t1805) * t94
     # - t15860) * t94 / 0.2E1 + (t15860 - (t4224 - t7808) * t94) * t94 
     #/ 0.2E1) / 0.6E1) * t183 / 0.2E1) / 0.8E1)
        t15882 = dy * (t649 / 0.2E1 - t4230 / 0.2E1) / 0.24E2
        t15889 = t928 - dy * t6970 / 0.24E2
        t15894 = t161 * t4391 * t183
        t15899 = t895 * t9630 * t183
        t15902 = dy * t6983
        t15905 = cc * t6786
        t15923 = t3704 / 0.2E1
        t15933 = t4 * (t3271 / 0.2E1 + t15923 - dx * ((t3262 - t3271) * 
     #t94 / 0.2E1 - (t3704 - t4156) * t94 / 0.2E1) / 0.8E1)
        t15945 = t4 * (t15923 + t4156 / 0.2E1 - dx * ((t3271 - t3704) * 
     #t94 / 0.2E1 - (t4156 - t7740) * t94 / 0.2E1) / 0.8E1)
        t15961 = t3481 * t6314
        t15983 = (t4297 - t4334) * t236
        t15994 = j - 3
        t15995 = u(t5,t15994,k,n)
        t15997 = (t1409 - t15995) * t183
        t16005 = u(i,t15994,k,n)
        t16007 = (t1803 - t16005) * t183
        t15141 = (t2044 - (t221 / 0.2E1 - t16007 / 0.2E1) * t183) * t183
        t16014 = t669 * t15141
        t16017 = u(t96,t15994,k,n)
        t16019 = (t4168 - t16017) * t183
        t16033 = u(i,t15994,t233,n)
        t16035 = (t4247 - t16033) * t183
        t16045 = t731 * t15141
        t16048 = u(i,t15994,t238,n)
        t16050 = (t4250 - t16048) * t183
        t16065 = t4373 / 0.2E1
        t16075 = t4 * (t4368 / 0.2E1 + t16065 - dz * ((t8853 - t4368) * 
     #t236 / 0.2E1 - (t4373 - t4382) * t236 / 0.2E1) / 0.8E1)
        t16087 = t4 * (t16065 + t4382 / 0.2E1 - dz * ((t4368 - t4373) * 
     #t236 / 0.2E1 - (t4382 - t9001) * t236 / 0.2E1) / 0.8E1)
        t16094 = (t4349 - t4362) * t236
        t16108 = (t3716 - t4176) * t94
        t16150 = t3481 * t6328
        t16165 = (t3725 - t4193) * t94
        t15202 = ((t3432 / 0.2E1 - t4289 / 0.2E1) * t94 - (t3758 / 0.2E1
     # - t7873 / 0.2E1) * t94) * t94
        t15206 = ((t3473 / 0.2E1 - t4328 / 0.2E1) * t94 - (t3797 / 0.2E1
     # - t7912 / 0.2E1) * t94) * t94
        t16192 = t3726 - t1429 * (t6707 / 0.2E1 + (t6705 - t3956 * ((t87
     #91 / 0.2E1 - t4252 / 0.2E1) * t236 - (t4249 / 0.2E1 - t8939 / 0.2E
     #1) * t236) * t236) * t183 / 0.2E1) / 0.6E1 + (t15933 * t354 - t159
     #45 * t680) * t94 - t1227 * ((t4000 * t15202 - t15961) * t236 / 0.2
     #E1 + (t15961 - t4037 * t15206) * t236 / 0.2E1) / 0.6E1 - t1429 * (
     #((t8834 - t4297) * t236 - t15983) * t236 / 0.2E1 + (t15983 - (t433
     #4 - t8982) * t236) * t236 / 0.2E1) / 0.6E1 - t1330 * ((t348 * (t19
     #50 - (t203 / 0.2E1 - t15997 / 0.2E1) * t183) * t183 - t16014) * t9
     #4 / 0.2E1 + (t16014 - t3877 * (t6531 - (t585 / 0.2E1 - t16019 / 0.
     #2E1) * t183) * t183) * t94 / 0.2E1) / 0.6E1 - t1330 * ((t4050 * (t
     #6436 - (t836 / 0.2E1 - t16035 / 0.2E1) * t183) * t183 - t16045) * 
     #t236 / 0.2E1 + (t16045 - t4061 * (t6451 - (t853 / 0.2E1 - t16050 /
     # 0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 + (t16075 * t742 -
     # t16087 * t745) * t236 - t1429 * (((t8847 - t4349) * t236 - t16094
     #) * t236 / 0.2E1 + (t16094 - (t4362 - t8995) * t236) * t236 / 0.2E
     #1) / 0.6E1 - t1227 * (((t3302 - t3716) * t94 - t16108) * t94 / 0.2
     #E1 + (t16108 - (t4176 - t7760) * t94) * t94 / 0.2E1) / 0.6E1 - t12
     #27 * ((t3707 * t13580 - t4159 * t13584) * t94 + ((t3710 - t4162) *
     # t94 - (t4162 - t7746) * t94) * t94) / 0.24E2 - t1429 * ((t4376 * 
     #t15747 - t4385 * t15751) * t236 + ((t8859 - t4388) * t236 - (t4388
     # - t9007) * t236) * t236) / 0.24E2 - t1429 * ((t3095 * t1936 - t16
     #150) * t94 / 0.2E1 + (t16150 - t3893 * t9694) * t94 / 0.2E1) / 0.6
     #E1 - t1227 * (((t3334 - t3725) * t94 - t16165) * t94 / 0.2E1 + (t1
     #6165 - (t4193 - t7777) * t94) * t94 / 0.2E1) / 0.6E1 - t1227 * (t6
     #676 / 0.2E1 + (t6674 - t3935 * ((t1802 / 0.2E1 - t4224 / 0.2E1) * 
     #t94 - (t1805 / 0.2E1 - t7808 / 0.2E1) * t94) * t94) * t183 / 0.2E1
     #) / 0.6E1
        t16194 = rx(i,t15994,k,0,0)
        t16195 = rx(i,t15994,k,1,1)
        t16197 = rx(i,t15994,k,2,2)
        t16199 = rx(i,t15994,k,1,2)
        t16201 = rx(i,t15994,k,2,1)
        t16203 = rx(i,t15994,k,1,0)
        t16205 = rx(i,t15994,k,0,2)
        t16207 = rx(i,t15994,k,0,1)
        t16210 = rx(i,t15994,k,2,0)
        t16216 = 0.1E1 / (t16194 * t16195 * t16197 - t16194 * t16199 * t
     #16201 + t16203 * t16201 * t16205 - t16203 * t16207 * t16197 + t162
     #10 * t16207 * t16199 - t16210 * t16195 * t16205)
        t16217 = t16203 ** 2
        t16218 = t16195 ** 2
        t16219 = t16199 ** 2
        t16221 = t16216 * (t16217 + t16218 + t16219)
        t16229 = t4 * (t6777 + t4236 / 0.2E1 - dy * (t6769 / 0.2E1 - (t4
     #236 - t16221) * t183 / 0.2E1) / 0.8E1)
        t16233 = t4 * t16216
        t16239 = (t15995 - t16005) * t94
        t16241 = (t16005 - t16017) * t94
        t15416 = t16233 * (t16194 * t16203 + t16207 * t16195 + t16205 * 
     #t16199)
        t16247 = (t4228 - t15416 * (t16239 / 0.2E1 + t16241 / 0.2E1)) * 
     #t183
        t16265 = t4 * (t4236 / 0.2E1 + t16221 / 0.2E1)
        t16268 = (t4240 - t16265 * t16007) * t183
        t16281 = (t16033 - t16005) * t236
        t16283 = (t16005 - t16048) * t236
        t15435 = t16233 * (t16203 * t16210 + t16195 * t16201 + t16199 * 
     #t16197)
        t16289 = (t4256 - t15435 * (t16281 / 0.2E1 + t16283 / 0.2E1)) * 
     #t183
        t16298 = t4177 + t4194 + t4231 + t4259 + t4298 + t4335 + t4363 +
     # t4350 + (t6788 - t16229 * t2041) * t183 - t1330 * (t6759 / 0.2E1 
     #+ (t6757 - (t4230 - t16247) * t183) * t183 / 0.2E1) / 0.6E1 + t687
     # + t3717 - t1330 * ((t6419 - t4239 * (t6416 - (t2041 - t16007) * t
     #183) * t183) * t183 + (t6425 - (t4242 - t16268) * t183) * t183) / 
     #0.24E2 - t1330 * (t6829 / 0.2E1 + (t6827 - (t4258 - t16289) * t183
     #) * t183 / 0.2E1) / 0.6E1 + t752
        t16300 = (t16192 + t16298) * t672
        t16303 = ut(i,t15994,k,n)
        t16305 = (t2117 - t16303) * t183
        t16309 = (t6969 - (t2119 - t16305) * t183) * t183
        t16316 = dy * (t9307 + t2119 / 0.2E1 - t1330 * (t6971 / 0.2E1 + 
     #t16309 / 0.2E1) / 0.6E1) / 0.2E1
        t16317 = ut(t5,t15994,k,n)
        t16319 = (t2099 - t16317) * t183
        t15512 = (t2122 - (t928 / 0.2E1 - t16305 / 0.2E1) * t183) * t183
        t16333 = t669 * t15512
        t16336 = ut(t96,t15994,k,n)
        t16338 = (t6939 - t16336) * t183
        t16358 = (t8856 * t7277 - t9624) * t236
        t16363 = (t9625 - t9004 * t7282) * t236
        t16389 = t3481 * t6731
        t16401 = ut(i,t1379,t1430,n)
        t16403 = (t16401 - t7008) * t236
        t16407 = ut(i,t1379,t1441,n)
        t16409 = (t7011 - t16407) * t236
        t16435 = t3481 * t6598
        t16459 = (t2266 - t7115) * t94 / 0.2E1 + (t7115 - t10640) * t94 
     #/ 0.2E1
        t16463 = (t8225 * t16459 - t9593) * t236
        t16467 = (t9597 - t9606) * t236
        t16475 = (t2284 - t7133) * t94 / 0.2E1 + (t7133 - t10658) * t94 
     #/ 0.2E1
        t16479 = (t9604 - t8368 * t16475) * t236
        t15645 = ((t9463 / 0.2E1 - t9589 / 0.2E1) * t94 - (t9465 / 0.2E1
     # - t13088 / 0.2E1) * t94) * t94
        t15650 = ((t9476 / 0.2E1 - t9600 / 0.2E1) * t94 - (t9478 / 0.2E1
     # - t13099 / 0.2E1) * t94) * t94
        t16488 = -t1330 * ((t348 * (t2104 - (t915 / 0.2E1 - t16319 / 0.2
     #E1) * t183) * t183 - t16333) * t94 / 0.2E1 + (t16333 - t3877 * (t7
     #159 - (t1092 / 0.2E1 - t16338 / 0.2E1) * t183) * t183) * t94 / 0.2
     #E1) / 0.6E1 - t1429 * ((t4376 * t15565 - t4385 * t15569) * t236 + 
     #((t16358 - t9627) * t236 - (t9627 - t16363) * t236) * t236) / 0.24
     #E2 - t1227 * ((t3707 * t13473 - t4159 * t13477) * t94 + ((t9429 - 
     #t9573) * t94 - (t9573 - t13072) * t94) * t94) / 0.24E2 - t1429 * (
     #(t3095 * t2388 - t16389) * t94 / 0.2E1 + (t16389 - t3893 * t10461)
     # * t94 / 0.2E1) / 0.6E1 - t1429 * (t7291 / 0.2E1 + (t7289 - t3956 
     #* ((t16403 / 0.2E1 - t7013 / 0.2E1) * t236 - (t7010 / 0.2E1 - t164
     #09 / 0.2E1) * t236) * t236) * t183 / 0.2E1) / 0.6E1 - t1227 * ((t4
     #000 * t15645 - t16435) * t236 / 0.2E1 + (t16435 - t4037 * t15650) 
     #* t236 / 0.2E1) / 0.6E1 - t1429 * (((t16463 - t9597) * t236 - t164
     #67) * t236 / 0.2E1 + (t16467 - (t9606 - t16479) * t236) * t236 / 0
     #.2E1) / 0.6E1 + t1134 + t9598 + t9607 + t9616 + t9623 + t9447 + t9
     #459 + t9586
        t16489 = ut(i,t15994,t233,n)
        t16491 = (t7008 - t16489) * t183
        t16501 = t731 * t15512
        t16504 = ut(i,t15994,t238,n)
        t16506 = (t7011 - t16504) * t183
        t16523 = (t9458 - t9584) * t94
        t16535 = (t7115 - t16401) * t183
        t16541 = (t8239 * (t7117 / 0.2E1 + t16535 / 0.2E1) - t9611) * t2
     #36
        t16545 = (t9615 - t9622) * t236
        t16549 = (t7133 - t16407) * t183
        t16555 = (t9620 - t8384 * (t7135 / 0.2E1 + t16549 / 0.2E1)) * t2
     #36
        t16577 = (t6980 - t16265 * t16305) * t183
        t16588 = (t9446 - t9579) * t94
        t16608 = (t7017 - t15435 * ((t16489 - t16303) * t236 / 0.2E1 + (
     #t16303 - t16504) * t236 / 0.2E1)) * t183
        t16642 = (t6945 - t15416 * ((t16317 - t16303) * t94 / 0.2E1 + (t
     #16303 - t16336) * t94 / 0.2E1)) * t183
        t16654 = t1166 + t9587 + t9580 + t9585 - t1330 * ((t4050 * (t720
     #9 - (t1190 / 0.2E1 - t16491 / 0.2E1) * t183) * t183 - t16501) * t2
     #36 / 0.2E1 + (t16501 - t4061 * (t7228 - (t1203 / 0.2E1 - t16506 / 
     #0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 - t1227 * (((t9453 
     #- t9458) * t94 - t16523) * t94 / 0.2E1 + (t16523 - (t9584 - t13083
     #) * t94) * t94 / 0.2E1) / 0.6E1 - t1429 * (((t16541 - t9615) * t23
     #6 - t16545) * t236 / 0.2E1 + (t16545 - (t9622 - t16555) * t236) * 
     #t236 / 0.2E1) / 0.6E1 + (t16075 * t1156 - t16087 * t1159) * t236 +
     # (t15933 * t990 - t15945 * t1127) * t94 - t1330 * ((t6972 - t4239 
     #* t16309) * t183 + (t6984 - (t6982 - t16577) * t183) * t183) / 0.2
     #4E2 - t1227 * (((t9439 - t9446) * t94 - t16588) * t94 / 0.2E1 + (t
     #16588 - (t9579 - t13078) * t94) * t94 / 0.2E1) / 0.6E1 - t1330 * (
     #t7023 / 0.2E1 + (t7021 - (t7019 - t16608) * t183) * t183 / 0.2E1) 
     #/ 0.6E1 - t1227 * (t7078 / 0.2E1 + (t7076 - t3935 * ((t2605 / 0.2E
     #1 - t6941 / 0.2E1) * t94 - (t2607 / 0.2E1 - t10692 / 0.2E1) * t94)
     # * t94) * t183 / 0.2E1) / 0.6E1 - t1330 * (t6951 / 0.2E1 + (t6949 
     #- (t6947 - t16642) * t183) * t183 / 0.2E1) / 0.6E1 + (t6957 - t162
     #29 * t2119) * t183
        t16656 = (t16488 + t16654) * t672
        t16659 = t1380 ** 2
        t16660 = t1393 ** 2
        t16661 = t1391 ** 2
        t16663 = t1402 * (t16659 + t16660 + t16661)
        t16664 = t4195 ** 2
        t16665 = t4208 ** 2
        t16666 = t4206 ** 2
        t16668 = t4217 * (t16664 + t16665 + t16666)
        t16671 = t4 * (t16663 / 0.2E1 + t16668 / 0.2E1)
        t16672 = t16671 * t1805
        t16673 = t7779 ** 2
        t16674 = t7792 ** 2
        t16675 = t7790 ** 2
        t16677 = t7801 * (t16673 + t16674 + t16675)
        t16680 = t4 * (t16668 / 0.2E1 + t16677 / 0.2E1)
        t16681 = t16680 * t4224
        t16685 = t1867 / 0.2E1 + t15997 / 0.2E1
        t16687 = t1662 * t16685
        t16689 = t2041 / 0.2E1 + t16007 / 0.2E1
        t16691 = t3935 * t16689
        t16694 = (t16687 - t16691) * t94 / 0.2E1
        t16696 = t4170 / 0.2E1 + t16019 / 0.2E1
        t16698 = t7254 * t16696
        t16701 = (t16691 - t16698) * t94 / 0.2E1
        t15869 = t1403 * (t1380 * t1396 + t1393 * t1387 + t1391 * t1383)
        t16707 = t15869 * t1416
        t15873 = t4218 * (t4195 * t4211 + t4208 * t4202 + t4206 * t4198)
        t16713 = t15873 * t4254
        t16716 = (t16707 - t16713) * t94 / 0.2E1
        t15880 = t7802 * (t7779 * t7795 + t7792 * t7786 + t7790 * t7782)
        t16722 = t15880 * t7838
        t16725 = (t16713 - t16722) * t94 / 0.2E1
        t15887 = t8760 * (t8737 * t8753 + t8750 * t8744 + t8748 * t8740)
        t16733 = t15887 * t8768
        t16735 = t15873 * t4226
        t16738 = (t16733 - t16735) * t236 / 0.2E1
        t15895 = t8908 * (t8885 * t8901 + t8898 * t8892 + t8896 * t8888)
        t16744 = t15895 * t8916
        t16747 = (t16735 - t16744) * t236 / 0.2E1
        t16749 = t4341 / 0.2E1 + t16035 / 0.2E1
        t16751 = t8188 * t16749
        t16753 = t3956 * t16689
        t16756 = (t16751 - t16753) * t236 / 0.2E1
        t16758 = t4356 / 0.2E1 + t16050 / 0.2E1
        t16760 = t8330 * t16758
        t16763 = (t16753 - t16760) * t236 / 0.2E1
        t16764 = t8753 ** 2
        t16765 = t8744 ** 2
        t16766 = t8740 ** 2
        t16768 = t8759 * (t16764 + t16765 + t16766)
        t16769 = t4211 ** 2
        t16770 = t4202 ** 2
        t16771 = t4198 ** 2
        t16773 = t4217 * (t16769 + t16770 + t16771)
        t16776 = t4 * (t16768 / 0.2E1 + t16773 / 0.2E1)
        t16777 = t16776 * t4249
        t16778 = t8901 ** 2
        t16779 = t8892 ** 2
        t16780 = t8888 ** 2
        t16782 = t8907 * (t16778 + t16779 + t16780)
        t16785 = t4 * (t16773 / 0.2E1 + t16782 / 0.2E1)
        t16786 = t16785 * t4252
        t16789 = (t16672 - t16681) * t94 + t16694 + t16701 + t16716 + t1
     #6725 + t4231 + t16247 / 0.2E1 + t16268 + t4259 + t16289 / 0.2E1 + 
     #t16738 + t16747 + t16756 + t16763 + (t16777 - t16786) * t236
        t16790 = t16789 * t4216
        t16792 = (t4390 - t16790) * t183
        t16794 = t4392 / 0.2E1 + t16792 / 0.2E1
        t16795 = dy * t16794
        t16803 = t1330 * (t6969 - dy * (t6971 - t16309) / 0.12E2) / 0.12
     #E2
        t16808 = t3336 ** 2
        t16809 = t3349 ** 2
        t16810 = t3347 ** 2
        t16819 = u(t64,t15994,k,n)
        t16838 = rx(t5,t15994,k,0,0)
        t16839 = rx(t5,t15994,k,1,1)
        t16841 = rx(t5,t15994,k,2,2)
        t16843 = rx(t5,t15994,k,1,2)
        t16845 = rx(t5,t15994,k,2,1)
        t16847 = rx(t5,t15994,k,1,0)
        t16849 = rx(t5,t15994,k,0,2)
        t16851 = rx(t5,t15994,k,0,1)
        t16854 = rx(t5,t15994,k,2,0)
        t16860 = 0.1E1 / (t16838 * t16839 * t16841 - t16838 * t16843 * t
     #16845 + t16847 * t16845 * t16849 - t16847 * t16851 * t16841 + t168
     #54 * t16851 * t16843 - t16854 * t16839 * t16849)
        t16861 = t4 * t16860
        t16875 = t16847 ** 2
        t16876 = t16839 ** 2
        t16877 = t16843 ** 2
        t16890 = u(t5,t15994,t233,n)
        t16893 = u(t5,t15994,t238,n)
        t16910 = t15869 * t1807
        t16926 = t1937 / 0.2E1 + (t1408 - t16890) * t183 / 0.2E1
        t16930 = t1269 * t16685
        t16937 = t1963 / 0.2E1 + (t1412 - t16893) * t183 / 0.2E1
        t16943 = t5940 ** 2
        t16944 = t5931 ** 2
        t16945 = t5927 ** 2
        t16948 = t1396 ** 2
        t16949 = t1387 ** 2
        t16950 = t1383 ** 2
        t16952 = t1402 * (t16948 + t16949 + t16950)
        t16957 = t6120 ** 2
        t16958 = t6111 ** 2
        t16959 = t6107 ** 2
        t16030 = t5947 * (t5924 * t5940 + t5937 * t5931 + t5935 * t5927)
        t16037 = t6127 * (t6104 * t6120 + t6117 * t6111 + t6115 * t6107)
        t16968 = (t4 * (t3358 * (t16808 + t16809 + t16810) / 0.2E1 + t16
     #663 / 0.2E1) * t1802 - t16672) * t94 + (t3272 * (t2022 / 0.2E1 + (
     #t1800 - t16819) * t183 / 0.2E1) - t16687) * t94 / 0.2E1 + t16694 +
     # (t3359 * (t3336 * t3352 + t3349 * t3343 + t3347 * t3339) * t3395 
     #- t16707) * t94 / 0.2E1 + t16716 + t3727 + (t1809 - t16861 * (t168
     #38 * t16847 + t16851 * t16839 + t16849 * t16843) * ((t16819 - t159
     #95) * t94 / 0.2E1 + t16239 / 0.2E1)) * t183 / 0.2E1 + (t1886 - t4 
     #* (t1845 / 0.2E1 + t16860 * (t16875 + t16876 + t16877) / 0.2E1) * 
     #t15997) * t183 + t3728 + (t1418 - t16861 * (t16847 * t16854 + t168
     #39 * t16845 + t16843 * t16841) * ((t16890 - t15995) * t236 / 0.2E1
     # + (t15995 - t16893) * t236 / 0.2E1)) * t183 / 0.2E1 + (t16030 * t
     #5957 - t16910) * t236 / 0.2E1 + (t16910 - t16037 * t6137) * t236 /
     # 0.2E1 + (t5670 * t16926 - t16930) * t236 / 0.2E1 + (t16930 - t584
     #0 * t16937) * t236 / 0.2E1 + (t4 * (t5946 * (t16943 + t16944 + t16
     #945) / 0.2E1 + t16952 / 0.2E1) * t1411 - t4 * (t16952 / 0.2E1 + t6
     #126 * (t16957 + t16958 + t16959) / 0.2E1) * t1414) * t236
        t16969 = t16968 * t1401
        t16977 = t669 * t16794
        t16981 = t11501 ** 2
        t16982 = t11514 ** 2
        t16983 = t11512 ** 2
        t16992 = u(t6542,t15994,k,n)
        t17011 = rx(t96,t15994,k,0,0)
        t17012 = rx(t96,t15994,k,1,1)
        t17014 = rx(t96,t15994,k,2,2)
        t17016 = rx(t96,t15994,k,1,2)
        t17018 = rx(t96,t15994,k,2,1)
        t17020 = rx(t96,t15994,k,1,0)
        t17022 = rx(t96,t15994,k,0,2)
        t17024 = rx(t96,t15994,k,0,1)
        t17027 = rx(t96,t15994,k,2,0)
        t17033 = 0.1E1 / (t17011 * t17012 * t17014 - t17011 * t17016 * t
     #17018 + t17020 * t17018 * t17022 - t17020 * t17024 * t17014 + t170
     #27 * t17024 * t17016 - t17027 * t17012 * t17022)
        t17034 = t4 * t17033
        t17048 = t17020 ** 2
        t17049 = t17012 ** 2
        t17050 = t17016 ** 2
        t17063 = u(t96,t15994,t233,n)
        t17066 = u(t96,t15994,t238,n)
        t17083 = t15880 * t7810
        t17099 = t7925 / 0.2E1 + (t7831 - t17063) * t183 / 0.2E1
        t17103 = t7272 * t16696
        t17110 = t7940 / 0.2E1 + (t7834 - t17066) * t183 / 0.2E1
        t17116 = t12475 ** 2
        t17117 = t12466 ** 2
        t17118 = t12462 ** 2
        t17121 = t7795 ** 2
        t17122 = t7786 ** 2
        t17123 = t7782 ** 2
        t17125 = t7801 * (t17121 + t17122 + t17123)
        t17130 = t12623 ** 2
        t17131 = t12614 ** 2
        t17132 = t12610 ** 2
        t16160 = t12482 * (t12459 * t12475 + t12472 * t12466 + t12470 * 
     #t12462)
        t16166 = t12630 * (t12607 * t12623 + t12620 * t12614 + t12618 * 
     #t12610)
        t17141 = (t16681 - t4 * (t16677 / 0.2E1 + t11523 * (t16981 + t16
     #982 + t16983) / 0.2E1) * t7808) * t94 + t16701 + (t16698 - t10929 
     #* (t7754 / 0.2E1 + (t7752 - t16992) * t183 / 0.2E1)) * t94 / 0.2E1
     # + t16725 + (t16722 - t11524 * (t11501 * t11517 + t11514 * t11508 
     #+ t11512 * t11504) * t11560) * t94 / 0.2E1 + t7815 + (t7812 - t170
     #34 * (t17011 * t17020 + t17024 * t17012 + t17022 * t17016) * (t162
     #41 / 0.2E1 + (t16017 - t16992) * t94 / 0.2E1)) * t183 / 0.2E1 + (t
     #7824 - t4 * (t7820 / 0.2E1 + t17033 * (t17048 + t17049 + t17050) /
     # 0.2E1) * t16019) * t183 + t7843 + (t7840 - t17034 * (t17020 * t17
     #027 + t17012 * t17018 + t17016 * t17014) * ((t17063 - t16017) * t2
     #36 / 0.2E1 + (t16017 - t17066) * t236 / 0.2E1)) * t183 / 0.2E1 + (
     #t16160 * t12490 - t17083) * t236 / 0.2E1 + (t17083 - t16166 * t126
     #38) * t236 / 0.2E1 + (t11834 * t17099 - t17103) * t236 / 0.2E1 + (
     #t17103 - t11969 * t17110) * t236 / 0.2E1 + (t4 * (t12481 * (t17116
     # + t17117 + t17118) / 0.2E1 + t17125 / 0.2E1) * t7833 - t4 * (t171
     #25 / 0.2E1 + t12629 * (t17130 + t17131 + t17132) / 0.2E1) * t7836)
     # * t236
        t17142 = t17141 * t7800
        t17155 = t3481 * t9013
        t17178 = t5924 ** 2
        t17179 = t5937 ** 2
        t17180 = t5935 ** 2
        t17183 = t8737 ** 2
        t17184 = t8750 ** 2
        t17185 = t8748 ** 2
        t17187 = t8759 * (t17183 + t17184 + t17185)
        t17192 = t12459 ** 2
        t17193 = t12472 ** 2
        t17194 = t12470 ** 2
        t17206 = t8158 * t16749
        t17218 = t15887 * t8793
        t17227 = rx(i,t15994,t233,0,0)
        t17228 = rx(i,t15994,t233,1,1)
        t17230 = rx(i,t15994,t233,2,2)
        t17232 = rx(i,t15994,t233,1,2)
        t17234 = rx(i,t15994,t233,2,1)
        t17236 = rx(i,t15994,t233,1,0)
        t17238 = rx(i,t15994,t233,0,2)
        t17240 = rx(i,t15994,t233,0,1)
        t17243 = rx(i,t15994,t233,2,0)
        t17249 = 0.1E1 / (t17227 * t17228 * t17230 - t17227 * t17232 * t
     #17234 + t17236 * t17234 * t17238 - t17236 * t17240 * t17230 + t172
     #43 * t17240 * t17232 - t17243 * t17228 * t17238)
        t17250 = t4 * t17249
        t17266 = t17236 ** 2
        t17267 = t17228 ** 2
        t17268 = t17232 ** 2
        t17281 = u(i,t15994,t1430,n)
        t17291 = rx(i,t1379,t1430,0,0)
        t17292 = rx(i,t1379,t1430,1,1)
        t17294 = rx(i,t1379,t1430,2,2)
        t17296 = rx(i,t1379,t1430,1,2)
        t17298 = rx(i,t1379,t1430,2,1)
        t17300 = rx(i,t1379,t1430,1,0)
        t17302 = rx(i,t1379,t1430,0,2)
        t17304 = rx(i,t1379,t1430,0,1)
        t17307 = rx(i,t1379,t1430,2,0)
        t17313 = 0.1E1 / (t17291 * t17292 * t17294 - t17291 * t17296 * t
     #17298 + t17300 * t17298 * t17302 - t17300 * t17304 * t17294 + t173
     #07 * t17304 * t17296 - t17307 * t17292 * t17302)
        t17314 = t4 * t17313
        t17324 = (t5978 - t8789) * t94 / 0.2E1 + (t8789 - t12511) * t94 
     #/ 0.2E1
        t17343 = t17307 ** 2
        t17344 = t17298 ** 2
        t17345 = t17294 ** 2
        t16342 = t17314 * (t17307 * t17300 + t17292 * t17298 + t17296 * 
     #t17294)
        t17354 = (t4 * (t5946 * (t17178 + t17179 + t17180) / 0.2E1 + t17
     #187 / 0.2E1) * t5955 - t4 * (t17187 / 0.2E1 + t12481 * (t17192 + t
     #17193 + t17194) / 0.2E1) * t8766) * t94 + (t5655 * t16926 - t17206
     #) * t94 / 0.2E1 + (t17206 - t11817 * t17099) * t94 / 0.2E1 + (t160
     #30 * t5982 - t17218) * t94 / 0.2E1 + (t17218 - t16160 * t12515) * 
     #t94 / 0.2E1 + t8773 + (t8770 - t17250 * (t17227 * t17236 + t17240 
     #* t17228 + t17238 * t17232) * ((t16890 - t16033) * t94 / 0.2E1 + (
     #t16033 - t17063) * t94 / 0.2E1)) * t183 / 0.2E1 + (t8782 - t4 * (t
     #8778 / 0.2E1 + t17249 * (t17266 + t17267 + t17268) / 0.2E1) * t160
     #35) * t183 + t8798 + (t8795 - t17250 * (t17236 * t17243 + t17228 *
     # t17234 + t17232 * t17230) * ((t17281 - t16033) * t236 / 0.2E1 + t
     #16281 / 0.2E1)) * t183 / 0.2E1 + (t17314 * (t17291 * t17307 + t173
     #04 * t17298 + t17302 * t17294) * t17324 - t16733) * t236 / 0.2E1 +
     # t16738 + (t16342 * (t8841 / 0.2E1 + (t8789 - t17281) * t183 / 0.2
     #E1) - t16751) * t236 / 0.2E1 + t16756 + (t4 * (t17313 * (t17343 + 
     #t17344 + t17345) / 0.2E1 + t16768 / 0.2E1) * t8791 - t16777) * t23
     #6
        t17355 = t17354 * t8758
        t17358 = t6104 ** 2
        t17359 = t6117 ** 2
        t17360 = t6115 ** 2
        t17363 = t8885 ** 2
        t17364 = t8898 ** 2
        t17365 = t8896 ** 2
        t17367 = t8907 * (t17363 + t17364 + t17365)
        t17372 = t12607 ** 2
        t17373 = t12620 ** 2
        t17374 = t12618 ** 2
        t17386 = t8308 * t16758
        t17398 = t15895 * t8941
        t17407 = rx(i,t15994,t238,0,0)
        t17408 = rx(i,t15994,t238,1,1)
        t17410 = rx(i,t15994,t238,2,2)
        t17412 = rx(i,t15994,t238,1,2)
        t17414 = rx(i,t15994,t238,2,1)
        t17416 = rx(i,t15994,t238,1,0)
        t17418 = rx(i,t15994,t238,0,2)
        t17420 = rx(i,t15994,t238,0,1)
        t17423 = rx(i,t15994,t238,2,0)
        t17429 = 0.1E1 / (t17407 * t17408 * t17410 - t17407 * t17412 * t
     #17414 + t17416 * t17414 * t17418 - t17416 * t17420 * t17410 + t174
     #23 * t17420 * t17412 - t17423 * t17408 * t17418)
        t17430 = t4 * t17429
        t17446 = t17416 ** 2
        t17447 = t17408 ** 2
        t17448 = t17412 ** 2
        t17461 = u(i,t15994,t1441,n)
        t17471 = rx(i,t1379,t1441,0,0)
        t17472 = rx(i,t1379,t1441,1,1)
        t17474 = rx(i,t1379,t1441,2,2)
        t17476 = rx(i,t1379,t1441,1,2)
        t17478 = rx(i,t1379,t1441,2,1)
        t17480 = rx(i,t1379,t1441,1,0)
        t17482 = rx(i,t1379,t1441,0,2)
        t17484 = rx(i,t1379,t1441,0,1)
        t17487 = rx(i,t1379,t1441,2,0)
        t17493 = 0.1E1 / (t17471 * t17472 * t17474 - t17471 * t17476 * t
     #17478 + t17480 * t17478 * t17482 - t17480 * t17484 * t17474 + t174
     #87 * t17484 * t17476 - t17487 * t17472 * t17482)
        t17494 = t4 * t17493
        t17504 = (t6158 - t8937) * t94 / 0.2E1 + (t8937 - t12659) * t94 
     #/ 0.2E1
        t17523 = t17487 ** 2
        t17524 = t17478 ** 2
        t17525 = t17474 ** 2
        t16484 = t17494 * (t17480 * t17487 + t17472 * t17478 + t17476 * 
     #t17474)
        t17534 = (t4 * (t6126 * (t17358 + t17359 + t17360) / 0.2E1 + t17
     #367 / 0.2E1) * t6135 - t4 * (t17367 / 0.2E1 + t12629 * (t17372 + t
     #17373 + t17374) / 0.2E1) * t8914) * t94 + (t5824 * t16937 - t17386
     #) * t94 / 0.2E1 + (t17386 - t11955 * t17110) * t94 / 0.2E1 + (t160
     #37 * t6162 - t17398) * t94 / 0.2E1 + (t17398 - t16166 * t12663) * 
     #t94 / 0.2E1 + t8921 + (t8918 - t17430 * (t17407 * t17416 + t17420 
     #* t17408 + t17418 * t17412) * ((t16893 - t16048) * t94 / 0.2E1 + (
     #t16048 - t17066) * t94 / 0.2E1)) * t183 / 0.2E1 + (t8930 - t4 * (t
     #8926 / 0.2E1 + t17429 * (t17446 + t17447 + t17448) / 0.2E1) * t160
     #50) * t183 + t8946 + (t8943 - t17430 * (t17423 * t17416 + t17408 *
     # t17414 + t17412 * t17410) * (t16283 / 0.2E1 + (t16048 - t17461) *
     # t236 / 0.2E1)) * t183 / 0.2E1 + t16747 + (t16744 - t17494 * (t174
     #71 * t17487 + t17478 * t17484 + t17482 * t17474) * t17504) * t236 
     #/ 0.2E1 + t16763 + (t16760 - t16484 * (t8989 / 0.2E1 + (t8937 - t1
     #7461) * t183 / 0.2E1)) * t236 / 0.2E1 + (t16786 - t4 * (t16782 / 0
     #.2E1 + t17493 * (t17523 + t17524 + t17525) / 0.2E1) * t8939) * t23
     #6
        t17535 = t17534 * t8906
        t17550 = (t6052 - t8861) * t94 / 0.2E1 + (t8861 - t12583) * t94 
     #/ 0.2E1
        t17554 = t3481 * t8401
        t17563 = (t6232 - t9009) * t94 / 0.2E1 + (t9009 - t12731) * t94 
     #/ 0.2E1
        t17576 = t731 * t16794
        t17593 = (t3707 * t5494 - t4159 * t8399) * t94 + (t348 * (t3857 
     #/ 0.2E1 + (t3855 - t16969) * t183 / 0.2E1) - t16977) * t94 / 0.2E1
     # + (t16977 - t3877 * (t7976 / 0.2E1 + (t7974 - t17142) * t183 / 0.
     #2E1)) * t94 / 0.2E1 + (t3095 * t6236 - t17155) * t94 / 0.2E1 + (t1
     #7155 - t3893 * t12735) * t94 / 0.2E1 + t8406 + (t8403 - t3935 * ((
     #t16969 - t16790) * t94 / 0.2E1 + (t16790 - t17142) * t94 / 0.2E1))
     # * t183 / 0.2E1 + (t8408 - t4239 * t16792) * t183 + t9018 + (t9015
     # - t3956 * ((t17355 - t16790) * t236 / 0.2E1 + (t16790 - t17535) *
     # t236 / 0.2E1)) * t183 / 0.2E1 + (t4000 * t17550 - t17554) * t236 
     #/ 0.2E1 + (t17554 - t4037 * t17563) * t236 / 0.2E1 + (t4050 * (t90
     #42 / 0.2E1 + (t8861 - t17355) * t183 / 0.2E1) - t17576) * t236 / 0
     #.2E1 + (t17576 - t4061 * (t9055 / 0.2E1 + (t9009 - t17535) * t183 
     #/ 0.2E1)) * t236 / 0.2E1 + (t4376 * t8863 - t4385 * t9011) * t236
        t17594 = t17593 * t672
        t17606 = t2119 / 0.2E1 + t16305 / 0.2E1
        t17608 = t3935 * t17606
        t17622 = t15873 * t7015
        t17638 = (t2363 - t7008) * t94 / 0.2E1 + (t7008 - t10746) * t94 
     #/ 0.2E1
        t17642 = t15873 * t6943
        t17651 = (t2366 - t7011) * t94 / 0.2E1 + (t7011 - t10767) * t94 
     #/ 0.2E1
        t17662 = t3956 * t17606
        t17677 = (t16671 * t2607 - t16680 * t6941) * t94 + (t1662 * (t21
     #01 / 0.2E1 + t16319 / 0.2E1) - t17608) * t94 / 0.2E1 + (t17608 - t
     #7254 * (t7156 / 0.2E1 + t16338 / 0.2E1)) * t94 / 0.2E1 + (t15869 *
     # t2370 - t17622) * t94 / 0.2E1 + (t17622 - t15880 * t11005) * t94 
     #/ 0.2E1 + t9586 + t16642 / 0.2E1 + t16577 + t9587 + t16608 / 0.2E1
     # + (t15887 * t17638 - t17642) * t236 / 0.2E1 + (t17642 - t15895 * 
     #t17651) * t236 / 0.2E1 + (t8188 * (t7206 / 0.2E1 + t16491 / 0.2E1)
     # - t17662) * t236 / 0.2E1 + (t17662 - t8330 * (t7225 / 0.2E1 + t16
     #506 / 0.2E1)) * t236 / 0.2E1 + (t16776 * t7010 - t16785 * t7013) *
     # t236
        t17683 = dy * (t9631 / 0.2E1 + (t9629 - t17677 * t4216) * t183 /
     # 0.2E1)
        t17687 = dy * (t4392 - t16792)
        t17690 = t2 + t6837 - t15430 + t7300 - t15433 + t15439 + t9070 -
     # t15444 + t15448 - t926 - t136 * t16300 - t16316 - t2076 * t16656 
     #/ 0.2E1 - t136 * t16795 / 0.2E1 - t16803 - t2917 * t17594 / 0.6E1 
     #- t2076 * t17683 / 0.4E1 - t136 * t17687 / 0.12E2
        t17703 = sqrt(t15453 + t15454 + t15455 + 0.8E1 * t702 + 0.8E1 * 
     #t703 + 0.8E1 * t704 - 0.2E1 * dy * ((t688 + t689 + t690 - t693 - t
     #694 - t695) * t183 / 0.2E1 - (t702 + t703 + t704 - t4232 - t4233 -
     # t4234) * t183 / 0.2E1))
        t17704 = 0.1E1 / t17703
        t17709 = t6787 * t9202 * t15889
        t17712 = t709 * t9205 * t15894 / 0.2E1
        t17715 = t709 * t9209 * t15899 / 0.6E1
        t17717 = t9202 * t15902 / 0.24E2
        t17729 = t2 + t9228 - t15430 + t9230 - t15492 + t15439 + t9234 -
     # t15494 + t15496 - t926 - t9202 * t16300 - t16316 - t9216 * t16656
     # / 0.2E1 - t9202 * t16795 / 0.2E1 - t16803 - t9221 * t17594 / 0.6E
     #1 - t9216 * t17683 / 0.4E1 - t9202 * t17687 / 0.12E2
        t17732 = 0.2E1 * t15905 * t17729 * t17704
        t17734 = (t6787 * t136 * t15889 + t709 * t159 * t15894 / 0.2E1 +
     # t709 * t893 * t15899 / 0.6E1 - t136 * t15902 / 0.24E2 + 0.2E1 * t
     #15905 * t17690 * t17704 - t17709 - t17712 - t17715 + t17717 - t177
     #32) * t133
        t17740 = t6787 * (t221 - dy * t6417 / 0.24E2)
        t17742 = dy * t6424 / 0.24E2
        t17758 = t4 * (t15518 + t15522 / 0.2E1 - dy * ((t15515 - t15517)
     # * t183 / 0.2E1 - (t15522 - t4217 * t4246) * t183 / 0.2E1) / 0.8E1
     #)
        t17769 = (t7010 - t7013) * t236
        t17786 = t9746 + t9747 - t9751 + t1156 / 0.4E1 + t1159 / 0.4E1 -
     # t15572 / 0.12E2 - dy * ((t15553 + t15554 - t15555 - t9773 - t9774
     # + t9775) * t183 / 0.2E1 - (t15558 + t15559 - t15573 - t7010 / 0.2
     #E1 - t7013 / 0.2E1 + t1429 * (((t16403 - t7010) * t236 - t17769) *
     # t236 / 0.2E1 + (t17769 - (t7013 - t16409) * t236) * t236 / 0.2E1)
     # / 0.6E1) * t183 / 0.2E1) / 0.8E1
        t17791 = t4 * (t15517 / 0.2E1 + t15522 / 0.2E1)
        t17793 = t5272 / 0.4E1 + t5470 / 0.4E1 + t8863 / 0.4E1 + t9011 /
     # 0.4E1
        t17804 = t4826 * t9609
        t17816 = t4000 * t9994
        t17828 = (t9976 - t8158 * t17638) * t183
        t17832 = (t9981 - t8781 * t7206) * t183
        t17838 = (t9996 - t8188 * (t16403 / 0.2E1 + t7010 / 0.2E1)) * t1
     #83
        t17842 = (t5896 * t9465 - t8723 * t9589) * t94 + (t4688 * t9487 
     #- t17804) * t94 / 0.2E1 + (t17804 - t7510 * t13108) * t94 / 0.2E1 
     #+ (t3505 * t9869 - t17816) * t94 / 0.2E1 + (t17816 - t7307 * t1328
     #3) * t94 / 0.2E1 + t9979 + t17828 / 0.2E1 + t17832 + t9999 + t1783
     #8 / 0.2E1 + t16463 / 0.2E1 + t9598 + t16541 / 0.2E1 + t9616 + t163
     #58
        t17843 = t17842 * t4281
        t17853 = t4965 * t9618
        t17865 = t4037 * t10047
        t17877 = (t10029 - t8308 * t17651) * t183
        t17881 = (t10034 - t8929 * t7225) * t183
        t17887 = (t10049 - t8330 * (t7013 / 0.2E1 + t16409 / 0.2E1)) * t
     #183
        t17891 = (t6076 * t9478 - t8871 * t9600) * t94 + (t4737 * t9496 
     #- t17853) * t94 / 0.2E1 + (t17853 - t7692 * t13117) * t94 / 0.2E1 
     #+ (t3537 * t9941 - t17865) * t94 / 0.2E1 + (t17865 - t7354 * t1333
     #6) * t94 / 0.2E1 + t10032 + t17877 / 0.2E1 + t17881 + t10052 + t17
     #887 / 0.2E1 + t9607 + t16479 / 0.2E1 + t9623 + t16555 / 0.2E1 + t1
     #6363
        t17892 = t17891 * t4320
        t17896 = t10005 / 0.4E1 + t10058 / 0.4E1 + (t17843 - t9629) * t2
     #36 / 0.4E1 + (t9629 - t17892) * t236 / 0.4E1
        t17902 = dy * (t1152 / 0.2E1 - t7019 / 0.2E1)
        t17906 = t17758 * t9202 * t17786
        t17909 = t17791 * t9216 * t17793 / 0.2E1
        t17912 = t17791 * t9221 * t17896 / 0.6E1
        t17914 = t9202 * t17902 / 0.24E2
        t17916 = (t17758 * t136 * t17786 + t17791 * t2076 * t17793 / 0.2
     #E1 + t17791 * t2917 * t17896 / 0.6E1 - t136 * t17902 / 0.24E2 - t1
     #7906 - t17909 - t17912 + t17914) * t133
        t17929 = (t4249 - t4252) * t236
        t17947 = t17758 * (t10089 + t10090 - t10094 + t742 / 0.4E1 + t74
     #5 / 0.4E1 - t15754 / 0.12E2 - dy * ((t15735 + t15736 - t15737 - t1
     #0116 - t10117 + t10118) * t183 / 0.2E1 - (t15740 + t15741 - t15755
     # - t4249 / 0.2E1 - t4252 / 0.2E1 + t1429 * (((t8791 - t4249) * t23
     #6 - t17929) * t236 / 0.2E1 + (t17929 - (t4252 - t8939) * t236) * t
     #236 / 0.2E1) / 0.6E1) * t183 / 0.2E1) / 0.8E1)
        t17951 = dy * (t734 / 0.2E1 - t4258 / 0.2E1) / 0.24E2
        t17956 = t13524 * t161 / 0.6E1 + (t13596 + t13514 + t13517 - t13
     #600 + t13520 - t13522 - t13524 * t9201) * t161 / 0.2E1 + t15502 * 
     #t161 / 0.6E1 + (t15508 + t15471 + t15474 - t15510 + t15477 - t1547
     #9 + t15500 - t15502 * t9201) * t161 / 0.2E1 + t15710 * t161 / 0.6E
     #1 + (t15763 + t15700 + t15703 - t15767 + t15706 - t15708 - t15710 
     #* t9201) * t161 / 0.2E1 - t15847 * t161 / 0.6E1 - (t15878 + t15837
     # + t15840 - t15882 + t15843 - t15845 - t15847 * t9201) * t161 / 0.
     #2E1 - t17734 * t161 / 0.6E1 - (t17740 + t17709 + t17712 - t17742 +
     # t17715 - t17717 + t17732 - t17734 * t9201) * t161 / 0.2E1 - t1791
     #6 * t161 / 0.6E1 - (t17947 + t17906 + t17909 - t17951 + t17912 - t
     #17914 - t17916 * t9201) * t161 / 0.2E1
        t17959 = t775 * t780
        t17964 = t814 * t819
        t17972 = t4 * (t17959 / 0.2E1 + t9727 - dz * ((t5233 * t5228 - t
     #17959) * t236 / 0.2E1 - (t9726 - t17964) * t236 / 0.2E1) / 0.8E1)
        t17978 = (t1033 - t1168) * t94
        t17980 = ((t1031 - t1033) * t94 - t17978) * t94
        t17984 = (t17978 - (t1168 - t7084) * t94) * t94
        t17987 = t1227 * (t17980 / 0.2E1 + t17984 / 0.2E1)
        t17994 = (t2454 - t7171) * t94
        t18005 = t1033 / 0.2E1
        t18006 = t1168 / 0.2E1
        t18007 = t17987 / 0.6E1
        t18010 = t1046 / 0.2E1
        t18011 = t1179 / 0.2E1
        t18015 = (t1046 - t1179) * t94
        t18017 = ((t1044 - t1046) * t94 - t18015) * t94
        t18021 = (t18015 - (t1179 - t7098) * t94) * t94
        t18024 = t1227 * (t18017 / 0.2E1 + t18021 / 0.2E1)
        t18025 = t18024 / 0.6E1
        t18032 = t1033 / 0.4E1 + t1168 / 0.4E1 - t17987 / 0.12E2 + t1344
     #2 + t13443 - t13444 - dz * ((t2454 / 0.2E1 + t7171 / 0.2E1 - t1227
     # * (((t2452 - t2454) * t94 - t17994) * t94 / 0.2E1 + (t17994 - (t7
     #171 - t10709) * t94) * t94 / 0.2E1) / 0.6E1 - t18005 - t18006 + t1
     #8007) * t236 / 0.2E1 - (t2060 + t6838 - t6849 - t18010 - t18011 + 
     #t18025) * t236 / 0.2E1) / 0.8E1
        t18037 = t4 * (t17959 / 0.2E1 + t9726 / 0.2E1)
        t18039 = t6245 / 0.4E1 + t9020 / 0.4E1 + t2905 / 0.4E1 + t7434 /
     # 0.4E1
        t18048 = (t9878 - t10003) * t94 / 0.4E1 + (t10003 - t13292) * t9
     #4 / 0.4E1 + t6398 / 0.4E1 + t9170 / 0.4E1
        t18054 = dz * (t7177 / 0.2E1 - t1185 / 0.2E1)
        t18058 = t17972 * t9202 * t18032
        t18061 = t18037 * t9216 * t18039 / 0.2E1
        t18064 = t18037 * t9221 * t18048 / 0.6E1
        t18066 = t9202 * t18054 / 0.24E2
        t18068 = (t17972 * t136 * t18032 + t18037 * t2076 * t18039 / 0.2
     #E1 + t18037 * t2917 * t18048 / 0.6E1 - t136 * t18054 / 0.24E2 - t1
     #8058 - t18061 - t18064 + t18066) * t133
        t18076 = (t458 - t782) * t94
        t18078 = ((t456 - t458) * t94 - t18076) * t94
        t18082 = (t18076 - (t782 - t6792) * t94) * t94
        t18085 = t1227 * (t18078 / 0.2E1 + t18082 / 0.2E1)
        t18092 = (t1739 - t5235) * t94
        t18103 = t458 / 0.2E1
        t18104 = t782 / 0.2E1
        t18105 = t18085 / 0.6E1
        t18108 = t499 / 0.2E1
        t18109 = t821 / 0.2E1
        t18113 = (t499 - t821) * t94
        t18115 = ((t497 - t499) * t94 - t18113) * t94
        t18119 = (t18113 - (t821 - t6806) * t94) * t94
        t18122 = t1227 * (t18115 / 0.2E1 + t18119 / 0.2E1)
        t18123 = t18122 / 0.6E1
        t18131 = t17972 * (t458 / 0.4E1 + t782 / 0.4E1 - t18085 / 0.12E2
     # + t13543 + t13544 - t13548 - dz * ((t1739 / 0.2E1 + t5235 / 0.2E1
     # - t1227 * (((t1737 - t1739) * t94 - t18092) * t94 / 0.2E1 + (t180
     #92 - (t5235 - t8144) * t94) * t94 / 0.2E1) / 0.6E1 - t18103 - t181
     #04 + t18105) * t236 / 0.2E1 - (t13570 + t13571 - t13572 - t18108 -
     # t18109 + t18123) * t236 / 0.2E1) / 0.8E1)
        t18135 = dz * (t5241 / 0.2E1 - t827 / 0.2E1) / 0.24E2
        t18140 = t775 * t832
        t18145 = t814 * t849
        t18153 = t4 * (t18140 / 0.2E1 + t15518 - dz * ((t5228 * t5246 - 
     #t18140) * t236 / 0.2E1 - (t15517 - t18145) * t236 / 0.2E1) / 0.8E1
     #)
        t18159 = (t1188 - t1190) * t183
        t18161 = ((t7201 - t1188) * t183 - t18159) * t183
        t18165 = (t18159 - (t1190 - t7206) * t183) * t183
        t18168 = t1330 * (t18161 / 0.2E1 + t18165 / 0.2E1)
        t18175 = (t7114 - t7117) * t183
        t18186 = t1188 / 0.2E1
        t18187 = t1190 / 0.2E1
        t18188 = t18168 / 0.6E1
        t18191 = t1201 / 0.2E1
        t18192 = t1203 / 0.2E1
        t18196 = (t1201 - t1203) * t183
        t18198 = ((t7220 - t1201) * t183 - t18196) * t183
        t18202 = (t18196 - (t1203 - t7225) * t183) * t183
        t18205 = t1330 * (t18198 / 0.2E1 + t18202 / 0.2E1)
        t18206 = t18205 / 0.6E1
        t18213 = t1188 / 0.4E1 + t1190 / 0.4E1 - t18168 / 0.12E2 + t9279
     # + t9280 - t9284 - dz * ((t7114 / 0.2E1 + t7117 / 0.2E1 - t1330 * 
     #(((t14251 - t7114) * t183 - t18175) * t183 / 0.2E1 + (t18175 - (t7
     #117 - t16535) * t183) * t183 / 0.2E1) / 0.6E1 - t18186 - t18187 + 
     #t18188) * t236 / 0.2E1 - (t9306 + t9307 - t9308 - t18191 - t18192 
     #+ t18206) * t236 / 0.2E1) / 0.8E1
        t18218 = t4 * (t18140 / 0.2E1 + t15517 / 0.2E1)
        t18220 = t9040 / 0.4E1 + t9042 / 0.4E1 + t4128 / 0.4E1 + t4392 /
     # 0.4E1
        t18229 = (t15637 - t10003) * t183 / 0.4E1 + (t10003 - t17843) * 
     #t183 / 0.4E1 + t9570 / 0.4E1 + t9631 / 0.4E1
        t18235 = dz * (t7123 / 0.2E1 - t1209 / 0.2E1)
        t18239 = t18153 * t9202 * t18213
        t18242 = t18218 * t9216 * t18220 / 0.2E1
        t18245 = t18218 * t9221 * t18229 / 0.6E1
        t18247 = t9202 * t18235 / 0.24E2
        t18249 = (t18153 * t136 * t18213 + t18218 * t2076 * t18220 / 0.2
     #E1 + t18218 * t2917 * t18229 / 0.6E1 - t136 * t18235 / 0.24E2 - t1
     #8239 - t18242 - t18245 + t18247) * t133
        t18257 = (t834 - t836) * t183
        t18259 = ((t4077 - t834) * t183 - t18257) * t183
        t18263 = (t18257 - (t836 - t4341) * t183) * t183
        t18266 = t1330 * (t18259 / 0.2E1 + t18263 / 0.2E1)
        t18273 = (t5248 - t5250) * t183
        t18284 = t834 / 0.2E1
        t18285 = t836 / 0.2E1
        t18286 = t18266 / 0.6E1
        t18289 = t851 / 0.2E1
        t18290 = t853 / 0.2E1
        t18294 = (t851 - t853) * t183
        t18296 = ((t4092 - t851) * t183 - t18294) * t183
        t18300 = (t18294 - (t853 - t4356) * t183) * t183
        t18303 = t1330 * (t18296 / 0.2E1 + t18300 / 0.2E1)
        t18304 = t18303 / 0.6E1
        t18312 = t18153 * (t834 / 0.4E1 + t836 / 0.4E1 - t18266 / 0.12E2
     # + t9662 + t9663 - t9667 - dz * ((t5248 / 0.2E1 + t5250 / 0.2E1 - 
     #t1330 * (((t8536 - t5248) * t183 - t18273) * t183 / 0.2E1 + (t1827
     #3 - (t5250 - t8841) * t183) * t183 / 0.2E1) / 0.6E1 - t18284 - t18
     #285 + t18286) * t236 / 0.2E1 - (t9689 + t9690 - t9691 - t18289 - t
     #18290 + t18304) * t236 / 0.2E1) / 0.8E1)
        t18316 = dz * (t5256 / 0.2E1 - t859 / 0.2E1) / 0.24E2
        t18323 = t961 - dz * t6899 / 0.24E2
        t18328 = t161 * t5271 * t236
        t18333 = t895 * t10004 * t236
        t18336 = dz * t6912
        t18339 = cc * t6486
        t18343 = (t4898 - t5123) * t94
        t18383 = k + 3
        t18384 = u(t5,j,t18383,n)
        t18386 = (t18384 - t1431) * t236
        t18394 = u(i,j,t18383,n)
        t18396 = (t18394 - t1710) * t236
        t17402 = ((t18396 / 0.2E1 - t269 / 0.2E1) * t236 - t1715) * t236
        t18403 = t771 * t17402
        t18406 = u(t96,j,t18383,n)
        t18408 = (t18406 - t5129) * t236
        t18422 = rx(i,j,t18383,0,0)
        t18423 = rx(i,j,t18383,1,1)
        t18425 = rx(i,j,t18383,2,2)
        t18427 = rx(i,j,t18383,1,2)
        t18429 = rx(i,j,t18383,2,1)
        t18431 = rx(i,j,t18383,1,0)
        t18433 = rx(i,j,t18383,0,2)
        t18435 = rx(i,j,t18383,0,1)
        t18438 = rx(i,j,t18383,2,0)
        t18444 = 0.1E1 / (t18422 * t18423 * t18425 - t18422 * t18427 * t
     #18429 + t18431 * t18429 * t18433 - t18431 * t18435 * t18425 + t184
     #38 * t18435 * t18427 - t18438 * t18423 * t18433)
        t18445 = t4 * t18444
        t18451 = (t18384 - t18394) * t94
        t18453 = (t18394 - t18406) * t94
        t17433 = t18445 * (t18422 * t18438 + t18435 * t18429 + t18433 * 
     #t18425)
        t18459 = (t17433 * (t18451 / 0.2E1 + t18453 / 0.2E1) - t5239) * 
     #t236
        t18489 = t4664 * t6148
        t18502 = t18438 ** 2
        t18503 = t18429 ** 2
        t18504 = t18425 ** 2
        t18506 = t18444 * (t18502 + t18503 + t18504)
        t18514 = t4 * (t5262 / 0.2E1 + t6476 - dz * ((t18506 - t5262) * 
     #t236 / 0.2E1 - t6491 / 0.2E1) / 0.8E1)
        t18522 = u(i,t180,t18383,n)
        t18524 = (t18522 - t18394) * t183
        t18525 = u(i,t185,t18383,n)
        t18527 = (t18394 - t18525) * t183
        t17455 = t18445 * (t18431 * t18438 + t18429 * t18423 + t18427 * 
     #t18425)
        t18533 = (t17455 * (t18524 / 0.2E1 + t18527 / 0.2E1) - t5254) * 
     #t236
        t18545 = (t4905 - t5137) * t94
        t18565 = t4 * (t18506 / 0.2E1 + t5262 / 0.2E1)
        t18568 = (t18565 * t18396 - t5266) * t236
        t18577 = t5168 / 0.2E1
        t18587 = t4 * (t5163 / 0.2E1 + t18577 - dy * ((t8473 - t5163) * 
     #t183 / 0.2E1 - (t5168 - t5177) * t183 / 0.2E1) / 0.8E1)
        t18599 = t4 * (t18577 + t5177 / 0.2E1 - dy * ((t5163 - t5168) * 
     #t183 / 0.2E1 - (t5177 - t8778) * t183 / 0.2E1) / 0.8E1)
        t18608 = t4664 * t6376
        t18621 = t4884 / 0.2E1
        t18631 = t4 * (t4441 / 0.2E1 + t18621 - dx * ((t4432 - t4441) * 
     #t94 / 0.2E1 - (t4884 - t5102) * t94 / 0.2E1) / 0.8E1)
        t18643 = t4 * (t18621 + t5102 / 0.2E1 - dx * ((t4441 - t4884) * 
     #t94 / 0.2E1 - (t5102 - t8011) * t94 / 0.2E1) / 0.8E1)
        t18648 = (t18522 - t5184) * t236
        t18658 = t822 * t17402
        t18662 = (t18525 - t5196) * t236
        t18676 = -t1227 * (((t4477 - t4898) * t94 - t18343) * t94 / 0.2E
     #1 + (t18343 - (t5123 - t8032) * t94) * t94 / 0.2E1) / 0.6E1 - t122
     #7 * ((t4887 * t18078 - t5105 * t18082) * t94 + ((t4890 - t5108) * 
     #t94 - (t5108 - t8017) * t94) * t94) / 0.24E2 - t1227 * ((t4866 * (
     #(t1737 / 0.2E1 - t5235 / 0.2E1) * t94 - (t1739 / 0.2E1 - t8144 / 0
     #.2E1) * t94) * t94 - t6799) * t236 / 0.2E1 + t6804 / 0.2E1) / 0.6E
     #1 - t1429 * ((t452 * ((t18386 / 0.2E1 - t252 / 0.2E1) * t236 - t17
     #00) * t236 - t18403) * t94 / 0.2E1 + (t18403 - t4807 * ((t18408 / 
     #0.2E1 - t599 / 0.2E1) * t236 - t6736) * t236) * t94 / 0.2E1) / 0.6
     #E1 - t1429 * (((t18459 - t5241) * t236 - t6595) * t236 / 0.2E1 + t
     #6599 / 0.2E1) / 0.6E1 - t1330 * ((t4876 * ((t8536 / 0.2E1 - t5250 
     #/ 0.2E1) * t183 - (t5248 / 0.2E1 - t8841 / 0.2E1) * t183) * t183 -
     # t6440) * t236 / 0.2E1 + t6445 / 0.2E1) / 0.6E1 - t1330 * ((t4147 
     #* t1918 - t18489) * t94 / 0.2E1 + (t18489 - t4798 * t9961) * t94 /
     # 0.2E1) / 0.6E1 + (t18514 * t1712 - t6488) * t236 - t1429 * (((t18
     #533 - t5256) * t236 - t6463) * t236 / 0.2E1 + t6467 / 0.2E1) / 0.6
     #E1 - t1227 * (((t4502 - t4905) * t94 - t18545) * t94 / 0.2E1 + (t1
     #8545 - (t5137 - t8046) * t94) * t94 / 0.2E1) / 0.6E1 - t1429 * ((t
     #5265 * ((t18396 - t1712) * t236 - t6504) * t236 - t6509) * t236 + 
     #((t18568 - t5268) * t236 - t6518) * t236) / 0.24E2 + (t18587 * t83
     #4 - t18599 * t836) * t183 - t1227 * ((t4817 * t13346 - t18608) * t
     #183 / 0.2E1 + (t18608 - t4826 * t15202) * t183 / 0.2E1) / 0.6E1 + 
     #(t18631 * t458 - t18643 * t782) * t94 - t1429 * ((t3800 * ((t18648
     # / 0.2E1 - t719 / 0.2E1) * t236 - t6683) * t236 - t18658) * t183 /
     # 0.2E1 + (t18658 - t4050 * ((t18662 / 0.2E1 - t742 / 0.2E1) * t236
     # - t6698) * t236) * t183 / 0.2E1) / 0.6E1
        t18680 = (t5194 - t5204) * t183
        t18707 = (t5148 - t5157) * t183
        t18718 = t5158 + t5149 + t5195 + t5205 + t5124 + t5257 + t5242 +
     # t5138 + t791 - t1330 * (((t8492 - t5194) * t183 - t18680) * t183 
     #/ 0.2E1 + (t18680 - (t5204 - t8797) * t183) * t183 / 0.2E1) / 0.6E
     #1 - t1330 * ((t5171 * t18259 - t5180 * t18263) * t183 + ((t8479 - 
     #t5183) * t183 - (t5183 - t8784) * t183) * t183) / 0.24E2 - t1330 *
     # (((t8467 - t5148) * t183 - t18707) * t183 / 0.2E1 + (t18707 - (t5
     #157 - t8772) * t183) * t183 / 0.2E1) / 0.6E1 + t4899 + t4906 + t84
     #5
        t18720 = (t18676 + t18718) * t774
        t18723 = ut(i,j,t18383,n)
        t18725 = (t18723 - t2241) * t236
        t18729 = ((t18725 - t2243) * t236 - t6896) * t236
        t18736 = dz * (t2243 / 0.2E1 + t9773 - t1429 * (t18729 / 0.2E1 +
     # t6900 / 0.2E1) / 0.6E1) / 0.2E1
        t18740 = (t9991 - t9998) * t183
        t18772 = t4664 * t6682
        t18787 = (t9823 - t9959) * t94
        t18814 = (t9841 - t9966) * t94
        t18825 = ut(t5,j,t18383,n)
        t18827 = (t18825 - t2223) * t236
        t17782 = ((t18725 / 0.2E1 - t961 / 0.2E1) * t236 - t2246) * t236
        t18841 = t771 * t17782
        t18844 = ut(t96,j,t18383,n)
        t18846 = (t18844 - t6853) * t236
        t18863 = ut(i,t180,t18383,n)
        t18866 = ut(i,t185,t18383,n)
        t18874 = (t17455 * ((t18863 - t18723) * t183 / 0.2E1 + (t18723 -
     # t18866) * t183 / 0.2E1) - t7121) * t236
        t18883 = -t1330 * (((t15632 - t9991) * t183 - t18740) * t183 / 0
     #.2E1 + (t18740 - (t9998 - t17838) * t183) * t183 / 0.2E1) / 0.6E1 
     #+ t9992 - t1330 * ((t4876 * ((t14251 / 0.2E1 - t7117 / 0.2E1) * t1
     #83 - (t7114 / 0.2E1 - t16535 / 0.2E1) * t183) * t183 - t7213) * t2
     #36 / 0.2E1 + t7218 / 0.2E1) / 0.6E1 + t9979 + t9974 + t9824 + t984
     #2 - t1330 * ((t4147 * t2471 - t18772) * t94 / 0.2E1 + (t18772 - t4
     #798 * t10320) * t94 / 0.2E1) / 0.6E1 - t1227 * (((t9818 - t9823) *
     # t94 - t18787) * t94 / 0.2E1 + (t18787 - (t9959 - t13248) * t94) *
     # t94 / 0.2E1) / 0.6E1 + t9967 - t1227 * ((t4887 * t17980 - t5105 *
     # t17984) * t94 + ((t9812 - t9955) * t94 - (t9955 - t13244) * t94) 
     #* t94) / 0.24E2 - t1227 * (((t9834 - t9841) * t94 - t18814) * t94 
     #/ 0.2E1 + (t18814 - (t9966 - t13255) * t94) * t94 / 0.2E1) / 0.6E1
     # - t1429 * ((t452 * ((t18827 / 0.2E1 - t948 / 0.2E1) * t236 - t222
     #8) * t236 - t18841) * t94 / 0.2E1 + (t18841 - t4807 * ((t18846 / 0
     #.2E1 - t1102 / 0.2E1) * t236 - t6858) * t236) * t94 / 0.2E1) / 0.6
     #E1 + (t18514 * t2243 - t7028) * t236 - t1429 * (((t18874 - t7123) 
     #* t236 - t7125) * t236 / 0.2E1 + t7129 / 0.2E1) / 0.6E1
        t18889 = (t18565 * t18725 - t6909) * t236
        t18915 = (t18863 - t7112) * t236
        t18925 = t822 * t17782
        t18929 = (t18866 - t7115) * t236
        t18950 = (t9973 - t9978) * t183
        t18986 = (t17433 * ((t18825 - t18723) * t94 / 0.2E1 + (t18723 - 
     #t18844) * t94 / 0.2E1) - t7175) * t236
        t19000 = t4664 * t6606
        t19012 = -t1429 * ((t5265 * t18729 - t6901) * t236 + ((t18889 - 
     #t6911) * t236 - t6913) * t236) / 0.24E2 + t1177 + t1199 + t10001 +
     # t9999 - t1330 * ((t5171 * t18161 - t5180 * t18165) * t183 + ((t15
     #626 - t9983) * t183 - (t9983 - t17832) * t183) * t183) / 0.24E2 + 
     #(t18631 * t1033 - t18643 * t1168) * t94 - t1429 * ((t3800 * ((t189
     #15 / 0.2E1 - t1141 / 0.2E1) * t236 - t7261) * t236 - t18925) * t18
     #3 / 0.2E1 + (t18925 - t4050 * ((t18929 / 0.2E1 - t1156 / 0.2E1) * 
     #t236 - t7280) * t236) * t183 / 0.2E1) / 0.6E1 + (t18587 * t1188 - 
     #t18599 * t1190) * t183 - t1330 * (((t15622 - t9973) * t183 - t1895
     #0) * t183 / 0.2E1 + (t18950 - (t9978 - t17828) * t183) * t183 / 0.
     #2E1) / 0.6E1 + t9960 - t1227 * ((t4866 * ((t2452 / 0.2E1 - t7171 /
     # 0.2E1) * t94 - (t2454 / 0.2E1 - t10709 / 0.2E1) * t94) * t94 - t7
     #091) * t236 / 0.2E1 + t7096 / 0.2E1) / 0.6E1 - t1429 * (((t18986 -
     # t7177) * t236 - t7179) * t236 / 0.2E1 + t7183 / 0.2E1) / 0.6E1 - 
     #t1227 * ((t4817 * t13561 - t19000) * t183 / 0.2E1 + (t19000 - t482
     #6 * t15645) * t183 / 0.2E1) / 0.6E1 + t10000
        t19014 = (t18883 + t19012) * t774
        t19017 = t1452 ** 2
        t19018 = t1465 ** 2
        t19019 = t1463 ** 2
        t19021 = t1474 * (t19017 + t19018 + t19019)
        t19022 = t5206 ** 2
        t19023 = t5219 ** 2
        t19024 = t5217 ** 2
        t19026 = t5228 * (t19022 + t19023 + t19024)
        t19029 = t4 * (t19021 / 0.2E1 + t19026 / 0.2E1)
        t19030 = t19029 * t1739
        t19031 = t8115 ** 2
        t19032 = t8128 ** 2
        t19033 = t8126 ** 2
        t19035 = t8137 * (t19031 + t19032 + t19033)
        t19038 = t4 * (t19026 / 0.2E1 + t19035 / 0.2E1)
        t19039 = t19038 * t5235
        t18073 = t1529 * (t1452 * t1461 + t1465 * t1453 + t1463 * t1457)
        t19047 = t18073 * t1541
        t18079 = t5229 * (t5206 * t5215 + t5219 * t5207 + t5217 * t5211)
        t19053 = t18079 * t5252
        t19056 = (t19047 - t19053) * t94 / 0.2E1
        t18087 = t8138 * (t8115 * t8124 + t8128 * t8116 + t8126 * t8120)
        t19062 = t18087 * t8161
        t19065 = (t19053 - t19062) * t94 / 0.2E1
        t19067 = t18386 / 0.2E1 + t1433 / 0.2E1
        t19069 = t1626 * t19067
        t19071 = t18396 / 0.2E1 + t1712 / 0.2E1
        t19073 = t4866 * t19071
        t19076 = (t19069 - t19073) * t94 / 0.2E1
        t19078 = t18408 / 0.2E1 + t5131 / 0.2E1
        t19080 = t7586 * t19078
        t19083 = (t19073 - t19080) * t94 / 0.2E1
        t18101 = t8517 * (t8494 * t8503 + t8507 * t8495 + t8505 * t8499)
        t19089 = t18101 * t8525
        t19091 = t18079 * t5237
        t19094 = (t19089 - t19091) * t183 / 0.2E1
        t18112 = t8822 * (t8799 * t8808 + t8812 * t8800 + t8810 * t8804)
        t19100 = t18112 * t8830
        t19103 = (t19091 - t19100) * t183 / 0.2E1
        t19104 = t8503 ** 2
        t19105 = t8495 ** 2
        t19106 = t8499 ** 2
        t19108 = t8516 * (t19104 + t19105 + t19106)
        t19109 = t5215 ** 2
        t19110 = t5207 ** 2
        t19111 = t5211 ** 2
        t19113 = t5228 * (t19109 + t19110 + t19111)
        t19116 = t4 * (t19108 / 0.2E1 + t19113 / 0.2E1)
        t19117 = t19116 * t5248
        t19118 = t8808 ** 2
        t19119 = t8800 ** 2
        t19120 = t8804 ** 2
        t19122 = t8821 * (t19118 + t19119 + t19120)
        t19125 = t4 * (t19113 / 0.2E1 + t19122 / 0.2E1)
        t19126 = t19125 * t5250
        t19130 = t18648 / 0.2E1 + t5186 / 0.2E1
        t19132 = t7939 * t19130
        t19134 = t4876 * t19071
        t19137 = (t19132 - t19134) * t183 / 0.2E1
        t19139 = t18662 / 0.2E1 + t5198 / 0.2E1
        t19141 = t8239 * t19139
        t19144 = (t19134 - t19141) * t183 / 0.2E1
        t19147 = (t19030 - t19039) * t94 + t19056 + t19065 + t19076 + t1
     #9083 + t19094 + t19103 + (t19117 - t19126) * t183 + t19137 + t1914
     #4 + t18459 / 0.2E1 + t5242 + t18533 / 0.2E1 + t5257 + t18568
        t19148 = t19147 * t5227
        t19150 = (t19148 - t5270) * t236
        t19152 = t19150 / 0.2E1 + t5272 / 0.2E1
        t19153 = dz * t19152
        t19161 = t1429 * (t6896 - dz * (t18729 - t6900) / 0.12E2) / 0.12
     #E2
        t19169 = t4664 * t9044
        t19178 = t4571 ** 2
        t19179 = t4584 ** 2
        t19180 = t4582 ** 2
        t19198 = u(t64,j,t18383,n)
        t19215 = t18073 * t1741
        t19228 = t5628 ** 2
        t19229 = t5620 ** 2
        t19230 = t5624 ** 2
        t19233 = t1461 ** 2
        t19234 = t1453 ** 2
        t19235 = t1457 ** 2
        t19237 = t1474 * (t19233 + t19234 + t19235)
        t19242 = t5997 ** 2
        t19243 = t5989 ** 2
        t19244 = t5993 ** 2
        t19253 = u(t5,t180,t18383,n)
        t19257 = (t19253 - t1534) * t236 / 0.2E1 + t1978 / 0.2E1
        t19261 = t1349 * t19067
        t19265 = u(t5,t185,t18383,n)
        t19269 = (t19265 - t1537) * t236 / 0.2E1 + t1997 / 0.2E1
        t19275 = rx(t5,j,t18383,0,0)
        t19276 = rx(t5,j,t18383,1,1)
        t19278 = rx(t5,j,t18383,2,2)
        t19280 = rx(t5,j,t18383,1,2)
        t19282 = rx(t5,j,t18383,2,1)
        t19284 = rx(t5,j,t18383,1,0)
        t19286 = rx(t5,j,t18383,0,2)
        t19288 = rx(t5,j,t18383,0,1)
        t19291 = rx(t5,j,t18383,2,0)
        t19297 = 0.1E1 / (t19275 * t19276 * t19278 - t19275 * t19280 * t
     #19282 + t19284 * t19282 * t19286 - t19284 * t19288 * t19278 + t192
     #91 * t19288 * t19280 - t19291 * t19276 * t19286)
        t19298 = t4 * t19297
        t19327 = t19291 ** 2
        t19328 = t19282 ** 2
        t19329 = t19278 ** 2
        t18219 = t5642 * (t5619 * t5628 + t5632 * t5620 + t5630 * t5624)
        t18225 = t6011 * (t5988 * t5997 + t6001 * t5989 + t5999 * t5993)
        t19338 = (t4 * (t4593 * (t19178 + t19179 + t19180) / 0.2E1 + t19
     #021 / 0.2E1) * t1737 - t19030) * t94 + (t4594 * (t4571 * t4580 + t
     #4584 * t4572 + t4582 * t4576) * t4617 - t19047) * t94 / 0.2E1 + t1
     #9056 + (t4367 * ((t19198 - t1682) * t236 / 0.2E1 + t1684 / 0.2E1) 
     #- t19069) * t94 / 0.2E1 + t19076 + (t18219 * t5652 - t19215) * t18
     #3 / 0.2E1 + (t19215 - t18225 * t6021) * t183 / 0.2E1 + (t4 * (t564
     #1 * (t19228 + t19229 + t19230) / 0.2E1 + t19237 / 0.2E1) * t1536 -
     # t4 * (t19237 / 0.2E1 + t6010 * (t19242 + t19243 + t19244) / 0.2E1
     #) * t1539) * t183 + (t5291 * t19257 - t19261) * t183 / 0.2E1 + (t1
     #9261 - t5685 * t19269) * t183 / 0.2E1 + (t19298 * (t19275 * t19291
     # + t19282 * t19288 + t19286 * t19278) * ((t19198 - t18384) * t94 /
     # 0.2E1 + t18451 / 0.2E1) - t1743) * t236 / 0.2E1 + t4968 + (t19298
     # * (t19284 * t19291 + t19276 * t19282 + t19280 * t19278) * ((t1925
     #3 - t18384) * t183 / 0.2E1 + (t18384 - t19265) * t183 / 0.2E1) - t
     #1543) * t236 / 0.2E1 + t4969 + (t4 * (t19297 * (t19327 + t19328 + 
     #t19329) / 0.2E1 + t1479 / 0.2E1) * t18386 - t1483) * t236
        t19339 = t19338 * t1473
        t19347 = t771 * t19152
        t19351 = t11837 ** 2
        t19352 = t11850 ** 2
        t19353 = t11848 ** 2
        t19371 = u(t6542,j,t18383,n)
        t19388 = t18087 * t8146
        t19401 = t12225 ** 2
        t19402 = t12217 ** 2
        t19403 = t12221 ** 2
        t19406 = t8124 ** 2
        t19407 = t8116 ** 2
        t19408 = t8120 ** 2
        t19410 = t8137 * (t19406 + t19407 + t19408)
        t19415 = t12530 ** 2
        t19416 = t12522 ** 2
        t19417 = t12526 ** 2
        t19426 = u(t96,t180,t18383,n)
        t19430 = (t19426 - t8093) * t236 / 0.2E1 + t8095 / 0.2E1
        t19434 = t7599 * t19078
        t19438 = u(t96,t185,t18383,n)
        t19442 = (t19438 - t8105) * t236 / 0.2E1 + t8107 / 0.2E1
        t19448 = rx(t96,j,t18383,0,0)
        t19449 = rx(t96,j,t18383,1,1)
        t19451 = rx(t96,j,t18383,2,2)
        t19453 = rx(t96,j,t18383,1,2)
        t19455 = rx(t96,j,t18383,2,1)
        t19457 = rx(t96,j,t18383,1,0)
        t19459 = rx(t96,j,t18383,0,2)
        t19461 = rx(t96,j,t18383,0,1)
        t19464 = rx(t96,j,t18383,2,0)
        t19470 = 0.1E1 / (t19448 * t19449 * t19451 - t19448 * t19453 * t
     #19455 + t19457 * t19455 * t19459 - t19457 * t19461 * t19451 + t194
     #64 * t19461 * t19453 - t19464 * t19449 * t19459)
        t19471 = t4 * t19470
        t19500 = t19464 ** 2
        t19501 = t19455 ** 2
        t19502 = t19451 ** 2
        t18369 = t12239 * (t12216 * t12225 + t12229 * t12217 + t12227 * 
     #t12221)
        t18374 = t12544 * (t12521 * t12530 + t12534 * t12522 + t12532 * 
     #t12526)
        t19511 = (t19039 - t4 * (t19035 / 0.2E1 + t11859 * (t19351 + t19
     #352 + t19353) / 0.2E1) * t8144) * t94 + t19065 + (t19062 - t11860 
     #* (t11837 * t11846 + t11850 * t11838 + t11848 * t11842) * t11883) 
     #* t94 / 0.2E1 + t19083 + (t19080 - t11201 * ((t19371 - t8038) * t2
     #36 / 0.2E1 + t8040 / 0.2E1)) * t94 / 0.2E1 + (t18369 * t12247 - t1
     #9388) * t183 / 0.2E1 + (t19388 - t18374 * t12552) * t183 / 0.2E1 +
     # (t4 * (t12238 * (t19401 + t19402 + t19403) / 0.2E1 + t19410 / 0.2
     #E1) * t8157 - t4 * (t19410 / 0.2E1 + t12543 * (t19415 + t19416 + t
     #19417) / 0.2E1) * t8159) * t183 + (t11557 * t19430 - t19434) * t18
     #3 / 0.2E1 + (t19434 - t11852 * t19442) * t183 / 0.2E1 + (t19471 * 
     #(t19448 * t19464 + t19461 * t19455 + t19459 * t19451) * (t18453 / 
     #0.2E1 + (t18406 - t19371) * t94 / 0.2E1) - t8148) * t236 / 0.2E1 +
     # t8151 + (t19471 * (t19457 * t19464 + t19449 * t19455 + t19453 * t
     #19451) * ((t19426 - t18406) * t183 / 0.2E1 + (t18406 - t19438) * t
     #183 / 0.2E1) - t8163) * t236 / 0.2E1 + t8166 + (t4 * (t19470 * (t1
     #9500 + t19501 + t19502) / 0.2E1 + t8171 / 0.2E1) * t18408 - t8175)
     # * t236
        t19512 = t19511 * t8136
        t19525 = t4664 * t9022
        t19538 = t5619 ** 2
        t19539 = t5632 ** 2
        t19540 = t5630 ** 2
        t19543 = t8494 ** 2
        t19544 = t8507 ** 2
        t19545 = t8505 ** 2
        t19547 = t8516 * (t19543 + t19544 + t19545)
        t19552 = t12216 ** 2
        t19553 = t12229 ** 2
        t19554 = t12227 ** 2
        t19566 = t18101 * t8538
        t19578 = t7928 * t19130
        t19596 = t15038 ** 2
        t19597 = t15030 ** 2
        t19598 = t15034 ** 2
        t19607 = u(i,t1331,t18383,n)
        t19617 = rx(i,t180,t18383,0,0)
        t19618 = rx(i,t180,t18383,1,1)
        t19620 = rx(i,t180,t18383,2,2)
        t19622 = rx(i,t180,t18383,1,2)
        t19624 = rx(i,t180,t18383,2,1)
        t19626 = rx(i,t180,t18383,1,0)
        t19628 = rx(i,t180,t18383,0,2)
        t19630 = rx(i,t180,t18383,0,1)
        t19633 = rx(i,t180,t18383,2,0)
        t19639 = 0.1E1 / (t19617 * t19618 * t19620 - t19617 * t19622 * t
     #19624 + t19626 * t19624 * t19628 - t19626 * t19630 * t19620 + t196
     #33 * t19630 * t19622 - t19633 * t19618 * t19628)
        t19640 = t4 * t19639
        t19669 = t19633 ** 2
        t19670 = t19624 ** 2
        t19671 = t19620 ** 2
        t19680 = (t4 * (t5641 * (t19538 + t19539 + t19540) / 0.2E1 + t19
     #547 / 0.2E1) * t5650 - t4 * (t19547 / 0.2E1 + t12238 * (t19552 + t
     #19553 + t19554) / 0.2E1) * t8523) * t94 + (t18219 * t5665 - t19566
     #) * t94 / 0.2E1 + (t19566 - t18369 * t12260) * t94 / 0.2E1 + (t528
     #3 * t19257 - t19578) * t94 / 0.2E1 + (t19578 - t11550 * t19430) * 
     #t94 / 0.2E1 + (t15052 * (t15029 * t15038 + t15042 * t15030 + t1504
     #0 * t15034) * t15062 - t19089) * t183 / 0.2E1 + t19094 + (t4 * (t1
     #5051 * (t19596 + t19597 + t19598) / 0.2E1 + t19108 / 0.2E1) * t853
     #6 - t19117) * t183 + (t14226 * ((t19607 - t8484) * t236 / 0.2E1 + 
     #t8486 / 0.2E1) - t19132) * t183 / 0.2E1 + t19137 + (t19640 * (t196
     #17 * t19633 + t19630 * t19624 + t19628 * t19620) * ((t19253 - t185
     #22) * t94 / 0.2E1 + (t18522 - t19426) * t94 / 0.2E1) - t8527) * t2
     #36 / 0.2E1 + t8530 + (t19640 * (t19626 * t19633 + t19618 * t19624 
     #+ t19622 * t19620) * ((t19607 - t18522) * t183 / 0.2E1 + t18524 / 
     #0.2E1) - t8540) * t236 / 0.2E1 + t8543 + (t4 * (t19639 * (t19669 +
     # t19670 + t19671) / 0.2E1 + t8548 / 0.2E1) * t18648 - t8552) * t23
     #6
        t19681 = t19680 * t8515
        t19689 = t822 * t19152
        t19693 = t5988 ** 2
        t19694 = t6001 ** 2
        t19695 = t5999 ** 2
        t19698 = t8799 ** 2
        t19699 = t8812 ** 2
        t19700 = t8810 ** 2
        t19702 = t8821 * (t19698 + t19699 + t19700)
        t19707 = t12521 ** 2
        t19708 = t12534 ** 2
        t19709 = t12532 ** 2
        t19721 = t18112 * t8843
        t19733 = t8225 * t19139
        t19751 = t17300 ** 2
        t19752 = t17292 ** 2
        t19753 = t17296 ** 2
        t19762 = u(i,t1379,t18383,n)
        t19772 = rx(i,t185,t18383,0,0)
        t19773 = rx(i,t185,t18383,1,1)
        t19775 = rx(i,t185,t18383,2,2)
        t19777 = rx(i,t185,t18383,1,2)
        t19779 = rx(i,t185,t18383,2,1)
        t19781 = rx(i,t185,t18383,1,0)
        t19783 = rx(i,t185,t18383,0,2)
        t19785 = rx(i,t185,t18383,0,1)
        t19788 = rx(i,t185,t18383,2,0)
        t19794 = 0.1E1 / (t19772 * t19773 * t19775 - t19772 * t19777 * t
     #19779 + t19781 * t19779 * t19783 - t19781 * t19785 * t19775 + t197
     #88 * t19785 * t19777 - t19788 * t19773 * t19783)
        t19795 = t4 * t19794
        t19824 = t19788 ** 2
        t19825 = t19779 ** 2
        t19826 = t19775 ** 2
        t19835 = (t4 * (t6010 * (t19693 + t19694 + t19695) / 0.2E1 + t19
     #702 / 0.2E1) * t6019 - t4 * (t19702 / 0.2E1 + t12543 * (t19707 + t
     #19708 + t19709) / 0.2E1) * t8828) * t94 + (t18225 * t6034 - t19721
     #) * t94 / 0.2E1 + (t19721 - t18374 * t12565) * t94 / 0.2E1 + (t567
     #8 * t19269 - t19733) * t94 / 0.2E1 + (t19733 - t11843 * t19442) * 
     #t94 / 0.2E1 + t19103 + (t19100 - t17314 * (t17291 * t17300 + t1730
     #4 * t17292 + t17302 * t17296) * t17324) * t183 / 0.2E1 + (t19126 -
     # t4 * (t19122 / 0.2E1 + t17313 * (t19751 + t19752 + t19753) / 0.2E
     #1) * t8841) * t183 + t19144 + (t19141 - t16342 * ((t19762 - t8789)
     # * t236 / 0.2E1 + t8791 / 0.2E1)) * t183 / 0.2E1 + (t19795 * (t197
     #72 * t19788 + t19785 * t19779 + t19783 * t19775) * ((t19265 - t185
     #25) * t94 / 0.2E1 + (t18525 - t19438) * t94 / 0.2E1) - t8832) * t2
     #36 / 0.2E1 + t8835 + (t19795 * (t19781 * t19788 + t19773 * t19779 
     #+ t19777 * t19775) * (t18527 / 0.2E1 + (t18525 - t19762) * t183 / 
     #0.2E1) - t8845) * t236 / 0.2E1 + t8848 + (t4 * (t19794 * (t19824 +
     # t19825 + t19826) / 0.2E1 + t8853 / 0.2E1) * t18662 - t8857) * t23
     #6
        t19836 = t19835 * t8820
        t19871 = (t4887 * t6245 - t5105 * t9020) * t94 + (t4147 * t6271 
     #- t19169) * t94 / 0.2E1 + (t19169 - t4798 * t12766) * t94 / 0.2E1 
     #+ (t452 * ((t19339 - t4971) * t236 / 0.2E1 + t4973 / 0.2E1) - t193
     #47) * t94 / 0.2E1 + (t19347 - t4807 * ((t19512 - t8179) * t236 / 0
     #.2E1 + t8181 / 0.2E1)) * t94 / 0.2E1 + (t4817 * t15288 - t19525) *
     # t183 / 0.2E1 + (t19525 - t4826 * t17550) * t183 / 0.2E1 + (t5171 
     #* t9040 - t5180 * t9042) * t183 + (t3800 * ((t19681 - t8556) * t23
     #6 / 0.2E1 + t8558 / 0.2E1) - t19689) * t183 / 0.2E1 + (t19689 - t4
     #050 * ((t19836 - t8861) * t236 / 0.2E1 + t8863 / 0.2E1)) * t183 / 
     #0.2E1 + (t4866 * ((t19339 - t19148) * t94 / 0.2E1 + (t19148 - t195
     #12) * t94 / 0.2E1) - t9024) * t236 / 0.2E1 + t9029 + (t4876 * ((t1
     #9681 - t19148) * t183 / 0.2E1 + (t19148 - t19836) * t183 / 0.2E1) 
     #- t9046) * t236 / 0.2E1 + t9051 + (t5265 * t19150 - t9063) * t236
        t19872 = t19871 * t774
        t19882 = t18079 * t7119
        t19896 = t18725 / 0.2E1 + t2243 / 0.2E1
        t19898 = t4866 * t19896
        t19912 = t18079 * t7173
        t19930 = t4876 * t19896
        t19943 = (t19029 * t2454 - t19038 * t7171) * t94 + (t18073 * t22
     #70 - t19882) * t94 / 0.2E1 + (t19882 - t18087 * t10644) * t94 / 0.
     #2E1 + (t1626 * (t18827 / 0.2E1 + t2225 / 0.2E1) - t19898) * t94 / 
     #0.2E1 + (t19898 - t7586 * (t18846 / 0.2E1 + t6855 / 0.2E1)) * t94 
     #/ 0.2E1 + (t18101 * t14187 - t19912) * t183 / 0.2E1 + (t19912 - t1
     #8112 * t16459) * t183 / 0.2E1 + (t19116 * t7114 - t19125 * t7117) 
     #* t183 + (t7939 * (t18915 / 0.2E1 + t7258 / 0.2E1) - t19930) * t18
     #3 / 0.2E1 + (t19930 - t8239 * (t18929 / 0.2E1 + t7277 / 0.2E1)) * 
     #t183 / 0.2E1 + t18986 / 0.2E1 + t10000 + t18874 / 0.2E1 + t10001 +
     # t18889
        t19949 = dz * ((t19943 * t5227 - t10003) * t236 / 0.2E1 + t10005
     # / 0.2E1)
        t19953 = dz * (t19150 - t5272)
        t19958 = dz * (t9773 + t9774 - t9775) / 0.2E1
        t19959 = dz * t5472
        t19961 = t136 * t19959 / 0.2E1
        t19967 = t1429 * (t6898 - dz * (t6900 - t6905) / 0.12E2) / 0.12E
     #2
        t19970 = dz * (t10005 / 0.2E1 + t10058 / 0.2E1)
        t19972 = t2076 * t19970 / 0.4E1
        t19974 = dz * (t5272 - t5470)
        t19976 = t136 * t19974 / 0.12E2
        t19977 = t959 + t136 * t18720 - t18736 + t2076 * t19014 / 0.2E1 
     #- t136 * t19153 / 0.2E1 + t19161 + t2917 * t19872 / 0.6E1 - t2076 
     #* t19949 / 0.4E1 + t136 * t19953 / 0.12E2 - t2 - t6837 - t19958 - 
     #t7300 - t19961 - t19967 - t9070 - t19972 - t19976
        t19981 = 0.8E1 * t866
        t19982 = 0.8E1 * t867
        t19983 = 0.8E1 * t868
        t19993 = sqrt(0.8E1 * t861 + 0.8E1 * t862 + 0.8E1 * t863 + t1998
     #1 + t19982 + t19983 - 0.2E1 * dz * ((t5258 + t5259 + t5260 - t861 
     #- t862 - t863) * t236 / 0.2E1 - (t866 + t867 + t868 - t875 - t876 
     #- t877) * t236 / 0.2E1))
        t19994 = 0.1E1 / t19993
        t19999 = t6487 * t9202 * t18323
        t20002 = t873 * t9205 * t18328 / 0.2E1
        t20005 = t873 * t9209 * t18333 / 0.6E1
        t20007 = t9202 * t18336 / 0.24E2
        t20020 = t9202 * t19959 / 0.2E1
        t20022 = t9216 * t19970 / 0.4E1
        t20024 = t9202 * t19974 / 0.12E2
        t20025 = t959 + t9202 * t18720 - t18736 + t9216 * t19014 / 0.2E1
     # - t9202 * t19153 / 0.2E1 + t19161 + t9221 * t19872 / 0.6E1 - t921
     #6 * t19949 / 0.4E1 + t9202 * t19953 / 0.12E2 - t2 - t9228 - t19958
     # - t9230 - t20020 - t19967 - t9234 - t20022 - t20024
        t20028 = 0.2E1 * t18339 * t20025 * t19994
        t20030 = (t6487 * t136 * t18323 + t873 * t159 * t18328 / 0.2E1 +
     # t873 * t893 * t18333 / 0.6E1 - t136 * t18336 / 0.24E2 + 0.2E1 * t
     #18339 * t19977 * t19994 - t19999 - t20002 - t20005 + t20007 - t200
     #28) * t133
        t20036 = t6487 * (t269 - dz * t6507 / 0.24E2)
        t20038 = dz * t6517 / 0.24E2
        t20054 = t4 * (t9727 + t17964 / 0.2E1 - dz * ((t17959 - t9726) *
     # t236 / 0.2E1 - (t17964 - t5426 * t5431) * t236 / 0.2E1) / 0.8E1)
        t20065 = (t2470 - t7185) * t94
        t20082 = t13442 + t13443 - t13444 + t1046 / 0.4E1 + t1179 / 0.4E
     #1 - t18024 / 0.12E2 - dz * ((t18005 + t18006 - t18007 - t2060 - t6
     #838 + t6849) * t236 / 0.2E1 - (t18010 + t18011 - t18025 - t2470 / 
     #0.2E1 - t7185 / 0.2E1 + t1227 * (((t2468 - t2470) * t94 - t20065) 
     #* t94 / 0.2E1 + (t20065 - (t7185 - t10724) * t94) * t94 / 0.2E1) /
     # 0.6E1) * t236 / 0.2E1) / 0.8E1
        t20087 = t4 * (t9726 / 0.2E1 + t17964 / 0.2E1)
        t20089 = t2905 / 0.4E1 + t7434 / 0.4E1 + t6258 / 0.4E1 + t9031 /
     # 0.4E1
        t20098 = t6398 / 0.4E1 + t9170 / 0.4E1 + (t9950 - t10056) * t94 
     #/ 0.4E1 + (t10056 - t13345) * t94 / 0.4E1
        t20104 = dz * (t1176 / 0.2E1 - t7191 / 0.2E1)
        t20108 = t20054 * t9202 * t20082
        t20111 = t20087 * t9216 * t20089 / 0.2E1
        t20114 = t20087 * t9221 * t20098 / 0.6E1
        t20116 = t9202 * t20104 / 0.24E2
        t20118 = (t20054 * t136 * t20082 + t20087 * t2076 * t20089 / 0.2
     #E1 + t20087 * t2917 * t20098 / 0.6E1 - t136 * t20104 / 0.24E2 - t2
     #0108 - t20111 - t20114 + t20116) * t133
        t20131 = (t1759 - t5433) * t94
        t20149 = t20054 * (t13543 + t13544 - t13548 + t499 / 0.4E1 + t82
     #1 / 0.4E1 - t18122 / 0.12E2 - dz * ((t18103 + t18104 - t18105 - t1
     #3570 - t13571 + t13572) * t236 / 0.2E1 - (t18108 + t18109 - t18123
     # - t1759 / 0.2E1 - t5433 / 0.2E1 + t1227 * (((t1757 - t1759) * t94
     # - t20131) * t94 / 0.2E1 + (t20131 - (t5433 - t8342) * t94) * t94 
     #/ 0.2E1) / 0.6E1) * t236 / 0.2E1) / 0.8E1)
        t20153 = dz * (t790 / 0.2E1 - t5439 / 0.2E1) / 0.24E2
        t20169 = t4 * (t15518 + t18145 / 0.2E1 - dz * ((t18140 - t15517)
     # * t236 / 0.2E1 - (t18145 - t5426 * t5444) * t236 / 0.2E1) / 0.8E1
     #)
        t20180 = (t7132 - t7135) * t183
        t20197 = t9279 + t9280 - t9284 + t1201 / 0.4E1 + t1203 / 0.4E1 -
     # t18205 / 0.12E2 - dz * ((t18186 + t18187 - t18188 - t9306 - t9307
     # + t9308) * t236 / 0.2E1 - (t18191 + t18192 - t18206 - t7132 / 0.2
     #E1 - t7135 / 0.2E1 + t1330 * (((t14266 - t7132) * t183 - t20180) *
     # t183 / 0.2E1 + (t20180 - (t7135 - t16549) * t183) * t183 / 0.2E1)
     # / 0.6E1) * t236 / 0.2E1) / 0.8E1
        t20202 = t4 * (t15517 / 0.2E1 + t18145 / 0.2E1)
        t20204 = t4128 / 0.4E1 + t4392 / 0.4E1 + t9053 / 0.4E1 + t9055 /
     # 0.4E1
        t20213 = t9570 / 0.4E1 + t9631 / 0.4E1 + (t15686 - t10056) * t18
     #3 / 0.4E1 + (t10056 - t17892) * t183 / 0.4E1
        t20219 = dz * (t1198 / 0.2E1 - t7141 / 0.2E1)
        t20223 = t20169 * t9202 * t20197
        t20226 = t20202 * t9216 * t20204 / 0.2E1
        t20229 = t20202 * t9221 * t20213 / 0.6E1
        t20231 = t9202 * t20219 / 0.24E2
        t20233 = (t20169 * t136 * t20197 + t20202 * t2076 * t20204 / 0.2
     #E1 + t20202 * t2917 * t20213 / 0.6E1 - t136 * t20219 / 0.24E2 - t2
     #0223 - t20226 - t20229 + t20231) * t133
        t20246 = (t5446 - t5448) * t183
        t20264 = t20169 * (t9662 + t9663 - t9667 + t851 / 0.4E1 + t853 /
     # 0.4E1 - t18303 / 0.12E2 - dz * ((t18284 + t18285 - t18286 - t9689
     # - t9690 + t9691) * t236 / 0.2E1 - (t18289 + t18290 - t18304 - t54
     #46 / 0.2E1 - t5448 / 0.2E1 + t1330 * (((t8684 - t5446) * t183 - t2
     #0246) * t183 / 0.2E1 + (t20246 - (t5448 - t8989) * t183) * t183 / 
     #0.2E1) / 0.6E1) * t236 / 0.2E1) / 0.8E1)
        t20268 = dz * (t844 / 0.2E1 - t5454 / 0.2E1) / 0.24E2
        t20275 = t964 - dz * t6904 / 0.24E2
        t20280 = t161 * t5469 * t236
        t20285 = t895 * t10057 * t236
        t20288 = dz * t6917
        t20291 = cc * t6498
        t20292 = k - 3
        t20293 = u(i,t180,t20292,n)
        t20295 = (t5382 - t20293) * t236
        t20303 = u(i,j,t20292,n)
        t20305 = (t1716 - t20303) * t236
        t19210 = (t1721 - (t272 / 0.2E1 - t20305 / 0.2E1) * t236) * t236
        t20312 = t837 * t19210
        t20315 = u(i,t185,t20292,n)
        t20317 = (t5394 - t20315) * t236
        t20338 = rx(i,j,t20292,0,0)
        t20339 = rx(i,j,t20292,1,1)
        t20341 = rx(i,j,t20292,2,2)
        t20343 = rx(i,j,t20292,1,2)
        t20345 = rx(i,j,t20292,2,1)
        t20347 = rx(i,j,t20292,1,0)
        t20349 = rx(i,j,t20292,0,2)
        t20351 = rx(i,j,t20292,0,1)
        t20354 = rx(i,j,t20292,2,0)
        t20360 = 0.1E1 / (t20338 * t20339 * t20341 - t20338 * t20343 * t
     #20345 + t20347 * t20345 * t20349 - t20347 * t20351 * t20341 + t203
     #54 * t20351 * t20343 - t20354 * t20339 * t20349)
        t20361 = t20354 ** 2
        t20362 = t20345 ** 2
        t20363 = t20341 ** 2
        t20365 = t20360 * (t20361 + t20362 + t20363)
        t20368 = t4 * (t5460 / 0.2E1 + t20365 / 0.2E1)
        t20371 = (t5464 - t20368 * t20305) * t236
        t20395 = (t5392 - t5402) * t183
        t20406 = t5347 + t5000 + t5322 + t5403 + t4993 + t5336 + t5393 +
     # t828 + t5440 - t1429 * ((t3811 * (t6686 - (t722 / 0.2E1 - t20295 
     #/ 0.2E1) * t236) * t236 - t20312) * t183 / 0.2E1 + (t20312 - t4061
     # * (t6701 - (t745 / 0.2E1 - t20317 / 0.2E1) * t236) * t236) * t183
     # / 0.2E1) / 0.6E1 - t1429 * ((t6514 - t5463 * (t6511 - (t1718 - t2
     #0305) * t236) * t236) * t236 + (t6520 - (t5466 - t20371) * t236) *
     # t236) / 0.24E2 + t5455 - t1227 * ((t4981 * t18115 - t5303 * t1811
     #9) * t94 + ((t4984 - t5306) * t94 - (t5306 - t8215) * t94) * t94) 
     #/ 0.24E2 + t5356 - t1330 * (((t8640 - t5392) * t183 - t20395) * t1
     #83 / 0.2E1 + (t20395 - (t5402 - t8945) * t183) * t183 / 0.2E1) / 0
     #.6E1
        t20408 = t5366 / 0.2E1
        t20418 = t4 * (t5361 / 0.2E1 + t20408 - dy * ((t8621 - t5361) * 
     #t183 / 0.2E1 - (t5366 - t5375) * t183 / 0.2E1) / 0.8E1)
        t20430 = t4 * (t20408 + t5375 / 0.2E1 - dy * ((t5361 - t5366) * 
     #t183 / 0.2E1 - (t5375 - t8926) * t183 / 0.2E1) / 0.8E1)
        t20439 = t4720 * t6381
        t20467 = t4 * t20360
        t20473 = (t20293 - t20303) * t183
        t20475 = (t20303 - t20315) * t183
        t19350 = t20467 * (t20347 * t20354 + t20339 * t20345 + t20343 * 
     #t20341)
        t20481 = (t5452 - t19350 * (t20473 / 0.2E1 + t20475 / 0.2E1)) * 
     #t236
        t20498 = t4 * (t6489 + t5460 / 0.2E1 - dz * (t6481 / 0.2E1 - (t5
     #460 - t20365) * t236 / 0.2E1) / 0.8E1)
        t20505 = (t5346 - t5355) * t183
        t20530 = t4978 / 0.2E1
        t20540 = t4 * (t4679 / 0.2E1 + t20530 - dx * ((t4670 - t4679) * 
     #t94 / 0.2E1 - (t4978 - t5300) * t94 / 0.2E1) / 0.8E1)
        t20552 = t4 * (t20530 + t5300 / 0.2E1 - dx * ((t4679 - t4978) * 
     #t94 / 0.2E1 - (t5300 - t8209) * t94 / 0.2E1) / 0.8E1)
        t20560 = u(t5,j,t20292,n)
        t20562 = (t20560 - t20303) * t94
        t20563 = u(t96,j,t20292,n)
        t20565 = (t20303 - t20563) * t94
        t19396 = t20467 * (t20338 * t20354 + t20351 * t20345 + t20349 * 
     #t20341)
        t20571 = (t5437 - t19396 * (t20562 / 0.2E1 + t20565 / 0.2E1)) * 
     #t236
        t20585 = t4720 * t6154
        t20614 = (t1442 - t20560) * t236
        t20624 = t809 * t19210
        t20628 = (t5327 - t20563) * t236
        t20645 = (t4999 - t5335) * t94
        t20659 = (t4992 - t5321) * t94
        t20670 = (t20418 * t851 - t20430 * t853) * t183 - t1227 * ((t495
     #6 * t13351 - t20439) * t183 / 0.2E1 + (t20439 - t4965 * t15206) * 
     #t183 / 0.2E1) / 0.6E1 - t1227 * (t6815 / 0.2E1 + (t6813 - t5040 * 
     #((t1757 / 0.2E1 - t5433 / 0.2E1) * t94 - (t1759 / 0.2E1 - t8342 / 
     #0.2E1) * t94) * t94) * t236 / 0.2E1) / 0.6E1 - t1429 * (t6471 / 0.
     #2E1 + (t6469 - (t5454 - t20481) * t236) * t236 / 0.2E1) / 0.6E1 + 
     #(t6500 - t20498 * t1718) * t236 - t1330 * (((t8615 - t5346) * t183
     # - t20505) * t183 / 0.2E1 + (t20505 - (t5355 - t8920) * t183) * t1
     #83 / 0.2E1) / 0.6E1 - t1330 * ((t5369 * t18296 - t5378 * t18300) *
     # t183 + ((t8627 - t5381) * t183 - (t5381 - t8932) * t183) * t183) 
     #/ 0.24E2 + (t20540 * t499 - t20552 * t821) * t94 - t1429 * (t6603 
     #/ 0.2E1 + (t6601 - (t5439 - t20571) * t236) * t236 / 0.2E1) / 0.6E
     #1 - t1330 * ((t4462 * t1923 - t20585) * t94 / 0.2E1 + (t20585 - t4
     #925 * t9970) * t94 / 0.2E1) / 0.6E1 - t1330 * (t6457 / 0.2E1 + (t6
     #455 - t5053 * ((t8684 / 0.2E1 - t5448 / 0.2E1) * t183 - (t5446 / 0
     #.2E1 - t8989 / 0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 - t1
     #429 * ((t492 * (t1703 - (t255 / 0.2E1 - t20614 / 0.2E1) * t236) * 
     #t236 - t20624) * t94 / 0.2E1 + (t20624 - t4947 * (t6739 - (t602 / 
     #0.2E1 - t20628 / 0.2E1) * t236) * t236) * t94 / 0.2E1) / 0.6E1 - t
     #1227 * (((t4740 - t4999) * t94 - t20645) * t94 / 0.2E1 + (t20645 -
     # (t5335 - t8244) * t94) * t94 / 0.2E1) / 0.6E1 - t1227 * (((t4715 
     #- t4992) * t94 - t20659) * t94 / 0.2E1 + (t20659 - (t5321 - t8230)
     # * t94) * t94 / 0.2E1) / 0.6E1 + t860
        t20672 = (t20406 + t20670) * t813
        t20675 = ut(i,j,t20292,n)
        t20677 = (t2247 - t20675) * t236
        t20681 = (t6903 - (t2249 - t20677) * t236) * t236
        t20688 = dz * (t9774 + t2249 / 0.2E1 - t1429 * (t6905 / 0.2E1 + 
     #t20681 / 0.2E1) / 0.6E1) / 0.2E1
        t20692 = (t10026 - t10031) * t183
        t20723 = ut(t5,j,t20292,n)
        t20726 = ut(t96,j,t20292,n)
        t20734 = (t7189 - t19396 * ((t20723 - t20675) * t94 / 0.2E1 + (t
     #20675 - t20726) * t94 / 0.2E1)) * t236
        t20743 = ut(i,t180,t20292,n)
        t20746 = ut(i,t185,t20292,n)
        t20754 = (t7139 - t19350 * ((t20743 - t20675) * t183 / 0.2E1 + (
     #t20675 - t20746) * t183 / 0.2E1)) * t236
        t20771 = t4720 * t6696
        t20787 = t10027 + t10052 + t9914 + t10020 + t10013 - t1330 * (((
     #t15671 - t10026) * t183 - t20692) * t183 / 0.2E1 + (t20692 - (t100
     #31 - t17877) * t183) * t183 / 0.2E1) / 0.6E1 + (t20418 * t1201 - t
     #20430 * t1203) * t183 - t1330 * (t7234 / 0.2E1 + (t7232 - t5053 * 
     #((t14266 / 0.2E1 - t7135 / 0.2E1) * t183 - (t7132 / 0.2E1 - t16549
     # / 0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 - t1429 * (t7195
     # / 0.2E1 + (t7193 - (t7191 - t20734) * t236) * t236 / 0.2E1) / 0.6
     #E1 + t1186 - t1429 * (t7145 / 0.2E1 + (t7143 - (t7141 - t20754) * 
     #t236) * t236 / 0.2E1) / 0.6E1 + (t7029 - t20498 * t2249) * t236 - 
     #t1330 * ((t4462 * t2475 - t20771) * t94 / 0.2E1 + (t20771 - t4925 
     #* t10325) * t94 / 0.2E1) / 0.6E1 + (t20540 * t1046 - t20552 * t117
     #9) * t94 + t1210
        t20791 = (t10044 - t10051) * t183
        t20807 = t4720 * t6612
        t20820 = (t2229 - t20723) * t236
        t19725 = (t2252 - (t964 / 0.2E1 - t20677 / 0.2E1) * t236) * t236
        t20834 = t809 * t19725
        t20838 = (t6859 - t20726) * t236
        t20855 = (t9913 - t10019) * t94
        t20871 = (t6914 - t20368 * t20677) * t236
        t20909 = (t7130 - t20743) * t236
        t20919 = t837 * t19725
        t20923 = (t7133 - t20746) * t236
        t20953 = (t9895 - t10012) * t94
        t20964 = -t1330 * (((t15681 - t10044) * t183 - t20791) * t183 / 
     #0.2E1 + (t20791 - (t10051 - t17887) * t183) * t183 / 0.2E1) / 0.6E
     #1 - t1227 * ((t4956 * t13568 - t20807) * t183 / 0.2E1 + (t20807 - 
     #t4965 * t15650) * t183 / 0.2E1) / 0.6E1 - t1429 * ((t492 * (t2234 
     #- (t951 / 0.2E1 - t20820 / 0.2E1) * t236) * t236 - t20834) * t94 /
     # 0.2E1 + (t20834 - t4947 * (t6864 - (t1105 / 0.2E1 - t20838 / 0.2E
     #1) * t236) * t236) * t94 / 0.2E1) / 0.6E1 - t1227 * (((t9906 - t99
     #13) * t94 - t20855) * t94 / 0.2E1 + (t20855 - (t10019 - t13308) * 
     #t94) * t94 / 0.2E1) / 0.6E1 - t1429 * ((t6906 - t5463 * t20681) * 
     #t236 + (t6918 - (t6916 - t20871) * t236) * t236) / 0.24E2 - t1227 
     #* (t7107 / 0.2E1 + (t7105 - t5040 * ((t2468 / 0.2E1 - t7185 / 0.2E
     #1) * t94 - (t2470 / 0.2E1 - t10724 / 0.2E1) * t94) * t94) * t236 /
     # 0.2E1) / 0.6E1 + t10054 - t1227 * ((t4981 * t18017 - t5303 * t180
     #21) * t94 + ((t9884 - t10008) * t94 - (t10008 - t13297) * t94) * t
     #94) / 0.24E2 + t10053 - t1429 * ((t3811 * (t7266 - (t1144 / 0.2E1 
     #- t20909 / 0.2E1) * t236) * t236 - t20919) * t183 / 0.2E1 + (t2091
     #9 - t4061 * (t7285 - (t1159 / 0.2E1 - t20923 / 0.2E1) * t236) * t2
     #36) * t183 / 0.2E1) / 0.6E1 - t1330 * ((t5369 * t18198 - t5378 * t
     #18202) * t183 + ((t15675 - t10036) * t183 - (t10036 - t17881) * t1
     #83) * t183) / 0.24E2 - t1227 * (((t9890 - t9895) * t94 - t20953) *
     # t94 / 0.2E1 + (t20953 - (t10012 - t13301) * t94) * t94 / 0.2E1) /
     # 0.6E1 + t9896 + t10032 + t10045
        t20966 = (t20787 + t20964) * t813
        t20969 = t1488 ** 2
        t20970 = t1501 ** 2
        t20971 = t1499 ** 2
        t20973 = t1510 * (t20969 + t20970 + t20971)
        t20974 = t5404 ** 2
        t20975 = t5417 ** 2
        t20976 = t5415 ** 2
        t20978 = t5426 * (t20974 + t20975 + t20976)
        t20981 = t4 * (t20973 / 0.2E1 + t20978 / 0.2E1)
        t20982 = t20981 * t1759
        t20983 = t8313 ** 2
        t20984 = t8326 ** 2
        t20985 = t8324 ** 2
        t20987 = t8335 * (t20983 + t20984 + t20985)
        t20990 = t4 * (t20978 / 0.2E1 + t20987 / 0.2E1)
        t20991 = t20990 * t5433
        t19907 = t1552 * (t1488 * t1497 + t1501 * t1489 + t1499 * t1493)
        t20999 = t19907 * t1564
        t19911 = t5427 * (t5404 * t5413 + t5417 * t5405 + t5415 * t5409)
        t21005 = t19911 * t5450
        t21008 = (t20999 - t21005) * t94 / 0.2E1
        t19918 = t8336 * (t8313 * t8322 + t8326 * t8314 + t8324 * t8318)
        t21014 = t19918 * t8359
        t21017 = (t21005 - t21014) * t94 / 0.2E1
        t21019 = t1444 / 0.2E1 + t20614 / 0.2E1
        t21021 = t1639 * t21019
        t21023 = t1718 / 0.2E1 + t20305 / 0.2E1
        t21025 = t5040 * t21023
        t21028 = (t21021 - t21025) * t94 / 0.2E1
        t21030 = t5329 / 0.2E1 + t20628 / 0.2E1
        t21032 = t7767 * t21030
        t21035 = (t21025 - t21032) * t94 / 0.2E1
        t19932 = t8665 * (t8642 * t8651 + t8655 * t8643 + t8653 * t8647)
        t21041 = t19932 * t8673
        t21043 = t19911 * t5435
        t21046 = (t21041 - t21043) * t183 / 0.2E1
        t19938 = t8970 * (t8947 * t8956 + t8960 * t8948 + t8958 * t8952)
        t21052 = t19938 * t8978
        t21055 = (t21043 - t21052) * t183 / 0.2E1
        t21056 = t8651 ** 2
        t21057 = t8643 ** 2
        t21058 = t8647 ** 2
        t21060 = t8664 * (t21056 + t21057 + t21058)
        t21061 = t5413 ** 2
        t21062 = t5405 ** 2
        t21063 = t5409 ** 2
        t21065 = t5426 * (t21061 + t21062 + t21063)
        t21068 = t4 * (t21060 / 0.2E1 + t21065 / 0.2E1)
        t21069 = t21068 * t5446
        t21070 = t8956 ** 2
        t21071 = t8948 ** 2
        t21072 = t8952 ** 2
        t21074 = t8969 * (t21070 + t21071 + t21072)
        t21077 = t4 * (t21065 / 0.2E1 + t21074 / 0.2E1)
        t21078 = t21077 * t5448
        t21082 = t5384 / 0.2E1 + t20295 / 0.2E1
        t21084 = t8091 * t21082
        t21086 = t5053 * t21023
        t21089 = (t21084 - t21086) * t183 / 0.2E1
        t21091 = t5396 / 0.2E1 + t20317 / 0.2E1
        t21093 = t8384 * t21091
        t21096 = (t21086 - t21093) * t183 / 0.2E1
        t21099 = (t20982 - t20991) * t94 + t21008 + t21017 + t21028 + t2
     #1035 + t21046 + t21055 + (t21069 - t21078) * t183 + t21089 + t2109
     #6 + t5440 + t20571 / 0.2E1 + t5455 + t20481 / 0.2E1 + t20371
        t21100 = t21099 * t5425
        t21102 = (t5468 - t21100) * t236
        t21104 = t5470 / 0.2E1 + t21102 / 0.2E1
        t21105 = dz * t21104
        t21113 = t1429 * (t6903 - dz * (t6905 - t20681) / 0.12E2) / 0.12
     #E2
        t21121 = t4720 * t9057
        t21130 = t4809 ** 2
        t21131 = t4822 ** 2
        t21132 = t4820 ** 2
        t21150 = u(t64,j,t20292,n)
        t21167 = t19907 * t1761
        t21180 = t5808 ** 2
        t21181 = t5800 ** 2
        t21182 = t5804 ** 2
        t21185 = t1497 ** 2
        t21186 = t1489 ** 2
        t21187 = t1493 ** 2
        t21189 = t1510 * (t21185 + t21186 + t21187)
        t21194 = t6177 ** 2
        t21195 = t6169 ** 2
        t21196 = t6173 ** 2
        t21205 = u(t5,t180,t20292,n)
        t21209 = t1983 / 0.2E1 + (t1557 - t21205) * t236 / 0.2E1
        t21213 = t1367 * t21019
        t21217 = u(t5,t185,t20292,n)
        t21221 = t2002 / 0.2E1 + (t1560 - t21217) * t236 / 0.2E1
        t21227 = rx(t5,j,t20292,0,0)
        t21228 = rx(t5,j,t20292,1,1)
        t21230 = rx(t5,j,t20292,2,2)
        t21232 = rx(t5,j,t20292,1,2)
        t21234 = rx(t5,j,t20292,2,1)
        t21236 = rx(t5,j,t20292,1,0)
        t21238 = rx(t5,j,t20292,0,2)
        t21240 = rx(t5,j,t20292,0,1)
        t21243 = rx(t5,j,t20292,2,0)
        t21249 = 0.1E1 / (t21227 * t21228 * t21230 - t21227 * t21232 * t
     #21234 + t21236 * t21234 * t21238 - t21236 * t21240 * t21230 + t212
     #43 * t21240 * t21232 - t21243 * t21228 * t21238)
        t21250 = t4 * t21249
        t21279 = t21243 ** 2
        t21280 = t21234 ** 2
        t21281 = t21230 ** 2
        t20047 = t5822 * (t5799 * t5808 + t5812 * t5800 + t5810 * t5804)
        t20052 = t6191 * (t6168 * t6177 + t6181 * t6169 + t6179 * t6173)
        t21290 = (t4 * (t4831 * (t21130 + t21131 + t21132) / 0.2E1 + t20
     #973 / 0.2E1) * t1757 - t20982) * t94 + (t4832 * (t4809 * t4818 + t
     #4822 * t4810 + t4820 * t4814) * t4855 - t20999) * t94 / 0.2E1 + t2
     #1008 + (t4612 * (t1690 / 0.2E1 + (t1688 - t21150) * t236 / 0.2E1) 
     #- t21021) * t94 / 0.2E1 + t21028 + (t20047 * t5832 - t21167) * t18
     #3 / 0.2E1 + (t21167 - t20052 * t6201) * t183 / 0.2E1 + (t4 * (t582
     #1 * (t21180 + t21181 + t21182) / 0.2E1 + t21189 / 0.2E1) * t1559 -
     # t4 * (t21189 / 0.2E1 + t6190 * (t21194 + t21195 + t21196) / 0.2E1
     #) * t1562) * t183 + (t5514 * t21209 - t21213) * t183 / 0.2E1 + (t2
     #1213 - t5855 * t21221) * t183 / 0.2E1 + t5062 + (t1763 - t21250 * 
     #(t21227 * t21243 + t21240 * t21234 + t21238 * t21230) * ((t21150 -
     # t20560) * t94 / 0.2E1 + t20562 / 0.2E1)) * t236 / 0.2E1 + t5063 +
     # (t1566 - t21250 * (t21236 * t21243 + t21228 * t21234 + t21232 * t
     #21230) * ((t21205 - t20560) * t183 / 0.2E1 + (t20560 - t21217) * t
     #183 / 0.2E1)) * t236 / 0.2E1 + (t1519 - t4 * (t1515 / 0.2E1 + t212
     #49 * (t21279 + t21280 + t21281) / 0.2E1) * t20614) * t236
        t21291 = t21290 * t1509
        t21299 = t809 * t21104
        t21303 = t12035 ** 2
        t21304 = t12048 ** 2
        t21305 = t12046 ** 2
        t21323 = u(t6542,j,t20292,n)
        t21340 = t19918 * t8344
        t21353 = t12373 ** 2
        t21354 = t12365 ** 2
        t21355 = t12369 ** 2
        t21358 = t8322 ** 2
        t21359 = t8314 ** 2
        t21360 = t8318 ** 2
        t21362 = t8335 * (t21358 + t21359 + t21360)
        t21367 = t12678 ** 2
        t21368 = t12670 ** 2
        t21369 = t12674 ** 2
        t21378 = u(t96,t180,t20292,n)
        t21382 = t8293 / 0.2E1 + (t8291 - t21378) * t236 / 0.2E1
        t21386 = t7787 * t21030
        t21390 = u(t96,t185,t20292,n)
        t21394 = t8305 / 0.2E1 + (t8303 - t21390) * t236 / 0.2E1
        t21400 = rx(t96,j,t20292,0,0)
        t21401 = rx(t96,j,t20292,1,1)
        t21403 = rx(t96,j,t20292,2,2)
        t21405 = rx(t96,j,t20292,1,2)
        t21407 = rx(t96,j,t20292,2,1)
        t21409 = rx(t96,j,t20292,1,0)
        t21411 = rx(t96,j,t20292,0,2)
        t21413 = rx(t96,j,t20292,0,1)
        t21416 = rx(t96,j,t20292,2,0)
        t21422 = 0.1E1 / (t21400 * t21401 * t21403 - t21400 * t21405 * t
     #21407 + t21409 * t21407 * t21411 - t21409 * t21413 * t21403 + t214
     #16 * t21413 * t21405 - t21416 * t21401 * t21411)
        t21423 = t4 * t21422
        t21452 = t21416 ** 2
        t21453 = t21407 ** 2
        t21454 = t21403 ** 2
        t20183 = t12387 * (t12364 * t12373 + t12377 * t12365 + t12375 * 
     #t12369)
        t20188 = t12692 * (t12669 * t12678 + t12682 * t12670 + t12680 * 
     #t12674)
        t21463 = (t20991 - t4 * (t20987 / 0.2E1 + t12057 * (t21303 + t21
     #304 + t21305) / 0.2E1) * t8342) * t94 + t21017 + (t21014 - t12058 
     #* (t12035 * t12044 + t12048 * t12036 + t12046 * t12040) * t12081) 
     #* t94 / 0.2E1 + t21035 + (t21032 - t11394 * (t8238 / 0.2E1 + (t823
     #6 - t21323) * t236 / 0.2E1)) * t94 / 0.2E1 + (t20183 * t12395 - t2
     #1340) * t183 / 0.2E1 + (t21340 - t20188 * t12700) * t183 / 0.2E1 +
     # (t4 * (t12386 * (t21353 + t21354 + t21355) / 0.2E1 + t21362 / 0.2
     #E1) * t8355 - t4 * (t21362 / 0.2E1 + t12691 * (t21367 + t21368 + t
     #21369) / 0.2E1) * t8357) * t183 + (t11704 * t21382 - t21386) * t18
     #3 / 0.2E1 + (t21386 - t11981 * t21394) * t183 / 0.2E1 + t8349 + (t
     #8346 - t21423 * (t21400 * t21416 + t21413 * t21407 + t21411 * t214
     #03) * (t20565 / 0.2E1 + (t20563 - t21323) * t94 / 0.2E1)) * t236 /
     # 0.2E1 + t8364 + (t8361 - t21423 * (t21416 * t21409 + t21401 * t21
     #407 + t21405 * t21403) * ((t21378 - t20563) * t183 / 0.2E1 + (t205
     #63 - t21390) * t183 / 0.2E1)) * t236 / 0.2E1 + (t8373 - t4 * (t836
     #9 / 0.2E1 + t21422 * (t21452 + t21453 + t21454) / 0.2E1) * t20628)
     # * t236
        t21464 = t21463 * t8334
        t21477 = t4720 * t9033
        t21490 = t5799 ** 2
        t21491 = t5812 ** 2
        t21492 = t5810 ** 2
        t21495 = t8642 ** 2
        t21496 = t8655 ** 2
        t21497 = t8653 ** 2
        t21499 = t8664 * (t21495 + t21496 + t21497)
        t21504 = t12364 ** 2
        t21505 = t12377 ** 2
        t21506 = t12375 ** 2
        t21518 = t19932 * t8686
        t21530 = t8071 * t21082
        t21548 = t15218 ** 2
        t21549 = t15210 ** 2
        t21550 = t15214 ** 2
        t21559 = u(i,t1331,t20292,n)
        t21569 = rx(i,t180,t20292,0,0)
        t21570 = rx(i,t180,t20292,1,1)
        t21572 = rx(i,t180,t20292,2,2)
        t21574 = rx(i,t180,t20292,1,2)
        t21576 = rx(i,t180,t20292,2,1)
        t21578 = rx(i,t180,t20292,1,0)
        t21580 = rx(i,t180,t20292,0,2)
        t21582 = rx(i,t180,t20292,0,1)
        t21585 = rx(i,t180,t20292,2,0)
        t21591 = 0.1E1 / (t21569 * t21570 * t21572 - t21569 * t21574 * t
     #21576 + t21578 * t21576 * t21580 - t21578 * t21582 * t21572 + t215
     #85 * t21582 * t21574 - t21585 * t21570 * t21580)
        t21592 = t4 * t21591
        t21621 = t21585 ** 2
        t21622 = t21576 ** 2
        t21623 = t21572 ** 2
        t21632 = (t4 * (t5821 * (t21490 + t21491 + t21492) / 0.2E1 + t21
     #499 / 0.2E1) * t5830 - t4 * (t21499 / 0.2E1 + t12386 * (t21504 + t
     #21505 + t21506) / 0.2E1) * t8671) * t94 + (t20047 * t5845 - t21518
     #) * t94 / 0.2E1 + (t21518 - t20183 * t12408) * t94 / 0.2E1 + (t550
     #3 * t21209 - t21530) * t94 / 0.2E1 + (t21530 - t11699 * t21382) * 
     #t94 / 0.2E1 + (t15232 * (t15209 * t15218 + t15222 * t15210 + t1522
     #0 * t15214) * t15242 - t21041) * t183 / 0.2E1 + t21046 + (t4 * (t1
     #5231 * (t21548 + t21549 + t21550) / 0.2E1 + t21060 / 0.2E1) * t868
     #4 - t21069) * t183 + (t14370 * (t8634 / 0.2E1 + (t8632 - t21559) *
     # t236 / 0.2E1) - t21084) * t183 / 0.2E1 + t21089 + t8678 + (t8675 
     #- t21592 * (t21569 * t21585 + t21582 * t21576 + t21580 * t21572) *
     # ((t21205 - t20293) * t94 / 0.2E1 + (t20293 - t21378) * t94 / 0.2E
     #1)) * t236 / 0.2E1 + t8691 + (t8688 - t21592 * (t21585 * t21578 + 
     #t21570 * t21576 + t21574 * t21572) * ((t21559 - t20293) * t183 / 0
     #.2E1 + t20473 / 0.2E1)) * t236 / 0.2E1 + (t8700 - t4 * (t8696 / 0.
     #2E1 + t21591 * (t21621 + t21622 + t21623) / 0.2E1) * t20295) * t23
     #6
        t21633 = t21632 * t8663
        t21641 = t837 * t21104
        t21645 = t6168 ** 2
        t21646 = t6181 ** 2
        t21647 = t6179 ** 2
        t21650 = t8947 ** 2
        t21651 = t8960 ** 2
        t21652 = t8958 ** 2
        t21654 = t8969 * (t21650 + t21651 + t21652)
        t21659 = t12669 ** 2
        t21660 = t12682 ** 2
        t21661 = t12680 ** 2
        t21673 = t19938 * t8991
        t21685 = t8368 * t21091
        t21703 = t17480 ** 2
        t21704 = t17472 ** 2
        t21705 = t17476 ** 2
        t21714 = u(i,t1379,t20292,n)
        t21724 = rx(i,t185,t20292,0,0)
        t21725 = rx(i,t185,t20292,1,1)
        t21727 = rx(i,t185,t20292,2,2)
        t21729 = rx(i,t185,t20292,1,2)
        t21731 = rx(i,t185,t20292,2,1)
        t21733 = rx(i,t185,t20292,1,0)
        t21735 = rx(i,t185,t20292,0,2)
        t21737 = rx(i,t185,t20292,0,1)
        t21740 = rx(i,t185,t20292,2,0)
        t21746 = 0.1E1 / (t21724 * t21725 * t21727 - t21724 * t21729 * t
     #21731 + t21733 * t21731 * t21735 - t21733 * t21737 * t21727 + t217
     #40 * t21737 * t21729 - t21740 * t21725 * t21735)
        t21747 = t4 * t21746
        t21776 = t21740 ** 2
        t21777 = t21731 ** 2
        t21778 = t21727 ** 2
        t21787 = (t4 * (t6190 * (t21645 + t21646 + t21647) / 0.2E1 + t21
     #654 / 0.2E1) * t6199 - t4 * (t21654 / 0.2E1 + t12691 * (t21659 + t
     #21660 + t21661) / 0.2E1) * t8976) * t94 + (t20052 * t6214 - t21673
     #) * t94 / 0.2E1 + (t21673 - t20188 * t12713) * t94 / 0.2E1 + (t584
     #7 * t21221 - t21685) * t94 / 0.2E1 + (t21685 - t11976 * t21394) * 
     #t94 / 0.2E1 + t21055 + (t21052 - t17494 * (t17471 * t17480 + t1748
     #4 * t17472 + t17482 * t17476) * t17504) * t183 / 0.2E1 + (t21078 -
     # t4 * (t21074 / 0.2E1 + t17493 * (t21703 + t21704 + t21705) / 0.2E
     #1) * t8989) * t183 + t21096 + (t21093 - t16484 * (t8939 / 0.2E1 + 
     #(t8937 - t21714) * t236 / 0.2E1)) * t183 / 0.2E1 + t8983 + (t8980 
     #- t21747 * (t21724 * t21740 + t21737 * t21731 + t21735 * t21727) *
     # ((t21217 - t20315) * t94 / 0.2E1 + (t20315 - t21390) * t94 / 0.2E
     #1)) * t236 / 0.2E1 + t8996 + (t8993 - t21747 * (t21740 * t21733 + 
     #t21731 * t21725 + t21729 * t21727) * (t20475 / 0.2E1 + (t20315 - t
     #21714) * t183 / 0.2E1)) * t236 / 0.2E1 + (t9005 - t4 * (t9001 / 0.
     #2E1 + t21746 * (t21776 + t21777 + t21778) / 0.2E1) * t20317) * t23
     #6
        t21788 = t21787 * t8968
        t21823 = (t4981 * t6258 - t5303 * t9031) * t94 + (t4462 * t6284 
     #- t21121) * t94 / 0.2E1 + (t21121 - t4925 * t12779) * t94 / 0.2E1 
     #+ (t492 * (t5067 / 0.2E1 + (t5065 - t21291) * t236 / 0.2E1) - t212
     #99) * t94 / 0.2E1 + (t21299 - t4947 * (t8379 / 0.2E1 + (t8377 - t2
     #1464) * t236 / 0.2E1)) * t94 / 0.2E1 + (t4956 * t15301 - t21477) *
     # t183 / 0.2E1 + (t21477 - t4965 * t17563) * t183 / 0.2E1 + (t5369 
     #* t9053 - t5378 * t9055) * t183 + (t3811 * (t8706 / 0.2E1 + (t8704
     # - t21633) * t236 / 0.2E1) - t21641) * t183 / 0.2E1 + (t21641 - t4
     #061 * (t9011 / 0.2E1 + (t9009 - t21788) * t236 / 0.2E1)) * t183 / 
     #0.2E1 + t9038 + (t9035 - t5040 * ((t21291 - t21100) * t94 / 0.2E1 
     #+ (t21100 - t21464) * t94 / 0.2E1)) * t236 / 0.2E1 + t9062 + (t905
     #9 - t5053 * ((t21633 - t21100) * t183 / 0.2E1 + (t21100 - t21788) 
     #* t183 / 0.2E1)) * t236 / 0.2E1 + (t9064 - t5463 * t21102) * t236
        t21824 = t21823 * t813
        t21834 = t19911 * t7137
        t21848 = t2249 / 0.2E1 + t20677 / 0.2E1
        t21850 = t5040 * t21848
        t21864 = t19911 * t7187
        t21882 = t5053 * t21848
        t21895 = (t20981 * t2470 - t20990 * t7185) * t94 + (t19907 * t22
     #88 - t21834) * t94 / 0.2E1 + (t21834 - t19918 * t10662) * t94 / 0.
     #2E1 + (t1639 * (t2231 / 0.2E1 + t20820 / 0.2E1) - t21850) * t94 / 
     #0.2E1 + (t21850 - t7767 * (t6861 / 0.2E1 + t20838 / 0.2E1)) * t94 
     #/ 0.2E1 + (t19932 * t14203 - t21864) * t183 / 0.2E1 + (t21864 - t1
     #9938 * t16475) * t183 / 0.2E1 + (t21068 * t7132 - t21077 * t7135) 
     #* t183 + (t8091 * (t7263 / 0.2E1 + t20909 / 0.2E1) - t21882) * t18
     #3 / 0.2E1 + (t21882 - t8384 * (t7282 / 0.2E1 + t20923 / 0.2E1)) * 
     #t183 / 0.2E1 + t10053 + t20734 / 0.2E1 + t10054 + t20754 / 0.2E1 +
     # t20871
        t21901 = dz * (t10058 / 0.2E1 + (t10056 - t21895 * t5425) * t236
     # / 0.2E1)
        t21905 = dz * (t5470 - t21102)
        t21908 = t2 + t6837 - t19958 + t7300 - t19961 + t19967 + t9070 -
     # t19972 + t19976 - t962 - t136 * t20672 - t20688 - t2076 * t20966 
     #/ 0.2E1 - t136 * t21105 / 0.2E1 - t21113 - t2917 * t21824 / 0.6E1 
     #- t2076 * t21901 / 0.4E1 - t136 * t21905 / 0.12E2
        t21921 = sqrt(t19981 + t19982 + t19983 + 0.8E1 * t875 + 0.8E1 * 
     #t876 + 0.8E1 * t877 - 0.2E1 * dz * ((t861 + t862 + t863 - t866 - t
     #867 - t868) * t236 / 0.2E1 - (t875 + t876 + t877 - t5456 - t5457 -
     # t5458) * t236 / 0.2E1))
        t21922 = 0.1E1 / t21921
        t21927 = t6499 * t9202 * t20275
        t21930 = t882 * t9205 * t20280 / 0.2E1
        t21933 = t882 * t9209 * t20285 / 0.6E1
        t21935 = t9202 * t20288 / 0.24E2
        t21947 = t2 + t9228 - t19958 + t9230 - t20020 + t19967 + t9234 -
     # t20022 + t20024 - t962 - t9202 * t20672 - t20688 - t9216 * t20966
     # / 0.2E1 - t9202 * t21105 / 0.2E1 - t21113 - t9221 * t21824 / 0.6E
     #1 - t9216 * t21901 / 0.4E1 - t9202 * t21905 / 0.12E2
        t21950 = 0.2E1 * t20291 * t21947 * t21922
        t21952 = (t6499 * t136 * t20275 + t882 * t159 * t20280 / 0.2E1 +
     # t882 * t893 * t20285 / 0.6E1 - t136 * t20288 / 0.24E2 + 0.2E1 * t
     #20291 * t21908 * t21922 - t21927 - t21930 - t21933 + t21935 - t219
     #50) * t133
        t21958 = t6499 * (t272 - dz * t6512 / 0.24E2)
        t21960 = dz * t6519 / 0.24E2
        t21965 = t18068 * t161 / 0.6E1 + (t18131 + t18058 + t18061 - t18
     #135 + t18064 - t18066 - t18068 * t9201) * t161 / 0.2E1 + t18249 * 
     #t161 / 0.6E1 + (t18312 + t18239 + t18242 - t18316 + t18245 - t1824
     #7 - t18249 * t9201) * t161 / 0.2E1 + t20030 * t161 / 0.6E1 + (t200
     #36 + t19999 + t20002 - t20038 + t20005 - t20007 + t20028 - t20030 
     #* t9201) * t161 / 0.2E1 - t20118 * t161 / 0.6E1 - (t20149 + t20108
     # + t20111 - t20153 + t20114 - t20116 - t20118 * t9201) * t161 / 0.
     #2E1 - t20233 * t161 / 0.6E1 - (t20264 + t20223 + t20226 - t20268 +
     # t20229 - t20231 - t20233 * t9201) * t161 / 0.2E1 - t21952 * t161 
     #/ 0.6E1 - (t21958 + t21927 + t21930 - t21960 + t21933 - t21935 + t
     #21950 - t21952 * t9201) * t161 / 0.2E1
        t21999 = t9244 * dt / 0.2E1 + (t9250 + t9204 + t9208 - t9252 + t
     #9212 - t9214 + t9242) * dt - t9244 * t9202 + t9653 * dt / 0.2E1 + 
     #(t9715 + t9643 + t9646 - t9719 + t9649 - t9651) * dt - t9653 * t92
     #02 + t10080 * dt / 0.2E1 + (t10142 + t10070 + t10073 - t10146 + t1
     #0076 - t10078) * dt - t10080 * t9202 - t12946 * dt / 0.2E1 - (t129
     #52 + t12921 + t12924 - t12954 + t12927 - t12929 + t12944) * dt + t
     #12946 * t9202 - t13152 * dt / 0.2E1 - (t13183 + t13142 + t13145 - 
     #t13187 + t13148 - t13150) * dt + t13152 * t9202 - t13369 * dt / 0.
     #2E1 - (t13400 + t13359 + t13362 - t13404 + t13365 - t13367) * dt +
     # t13369 * t9202
        t22032 = t13524 * dt / 0.2E1 + (t13596 + t13514 + t13517 - t1360
     #0 + t13520 - t13522) * dt - t13524 * t9202 + t15502 * dt / 0.2E1 +
     # (t15508 + t15471 + t15474 - t15510 + t15477 - t15479 + t15500) * 
     #dt - t15502 * t9202 + t15710 * dt / 0.2E1 + (t15763 + t15700 + t15
     #703 - t15767 + t15706 - t15708) * dt - t15710 * t9202 - t15847 * d
     #t / 0.2E1 - (t15878 + t15837 + t15840 - t15882 + t15843 - t15845) 
     #* dt + t15847 * t9202 - t17734 * dt / 0.2E1 - (t17740 + t17709 + t
     #17712 - t17742 + t17715 - t17717 + t17732) * dt + t17734 * t9202 -
     # t17916 * dt / 0.2E1 - (t17947 + t17906 + t17909 - t17951 + t17912
     # - t17914) * dt + t17916 * t9202
        t22065 = t18068 * dt / 0.2E1 + (t18131 + t18058 + t18061 - t1813
     #5 + t18064 - t18066) * dt - t18068 * t9202 + t18249 * dt / 0.2E1 +
     # (t18312 + t18239 + t18242 - t18316 + t18245 - t18247) * dt - t182
     #49 * t9202 + t20030 * dt / 0.2E1 + (t20036 + t19999 + t20002 - t20
     #038 + t20005 - t20007 + t20028) * dt - t20030 * t9202 - t20118 * d
     #t / 0.2E1 - (t20149 + t20108 + t20111 - t20153 + t20114 - t20116) 
     #* dt + t20118 * t9202 - t20233 * dt / 0.2E1 - (t20264 + t20223 + t
     #20226 - t20268 + t20229 - t20231) * dt + t20233 * t9202 - t21952 *
     # dt / 0.2E1 - (t21958 + t21927 + t21930 - t21960 + t21933 - t21935
     # + t21950) * dt + t21952 * t9202

        unew(i,j,k) = t1 + dt * t2 + t13409 * t56 * t94 + t17956 * t5
     #6 * t183 + t21965 * t56 * t236

        utnew(i,j,k) = t2 + t21999 * t56 * t94 + t220
     #32 * t56 * t183 + t22065 * t56 * t236

c        blah = array(int(t1 + dt * t2 + t13409 * t56 * t94 + t17956 * t5
c     #6 * t183 + t21965 * t56 * t236),int(t2 + t21999 * t56 * t94 + t220
c     #32 * t56 * t183 + t22065 * t56 * t236))

        return
      end
