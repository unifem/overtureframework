      subroutine duStepWaveGen3d4ccOLD( 
     *   nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,
     *   n1a,n1b,n2a,n2b,n3a,n3b,
     *   u,ut,unew,utnew,rx,
     *   dx,dy,dz,dt,cc,beta,
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
      real dx,dy,dz,dt,cc,beta
c
c.. generated code to follow
c
        real t1
        real t100
        real t10001
        real t10003
        real t10005
        real t10007
        real t10008
        real t1001
        real t10010
        real t10012
        real t10014
        real t10015
        real t10016
        real t10017
        real t10018
        real t10019
        real t1002
        real t10020
        real t10021
        real t10022
        real t10024
        real t10026
        real t10028
        real t10029
        real t10031
        real t10033
        real t10035
        real t10036
        real t10038
        real t1004
        real t10040
        real t10042
        real t10043
        real t10045
        real t10047
        real t10048
        real t10049
        real t1005
        real t10050
        real t10052
        real t10054
        real t10056
        real t10058
        real t10060
        real t10061
        real t10063
        real t10065
        real t10067
        real t10068
        real t10069
        real t1007
        real t10070
        real t10071
        real t10072
        real t10073
        real t10074
        real t10076
        real t10082
        real t10086
        real t10089
        real t1009
        real t10092
        real t10094
        real t10096
        real t10103
        real t10105
        real t10106
        real t10109
        real t1011
        real t10110
        real t10116
        real t10127
        real t10128
        real t10129
        real t1013
        real t10132
        real t10133
        real t10134
        real t10135
        real t10136
        real t10140
        real t10142
        real t10146
        real t10149
        real t1015
        real t10150
        real t10158
        real t1016
        real t10162
        real t10169
        real t1017
        real t10174
        real t10177
        real t10179
        real t10182
        real t10185
        real t10189
        real t1019
        real t102
        real t1020
        integer t10214
        real t10215
        real t10216
        real t10218
        real t1022
        real t10220
        real t10222
        real t10224
        real t10226
        real t10228
        real t10231
        real t10236
        real t10237
        real t10238
        real t10239
        real t1024
        real t10240
        real t10242
        real t10245
        real t10250
        real t10254
        real t1026
        real t10267
        real t1028
        real t10286
        real t10288
        real t1029
        real t10298
        real t10301
        real t1031
        real t10325
        real t1033
        real t10337
        real t10341
        real t10345
        real t10347
        real t1035
        real t10359
        real t1037
        real t1039
        real t10391
        real t104
        real t1041
        real t10411
        real t1042
        real t10421
        real t10433
        real t1044
        real t10440
        real t10451
        real t10453
        real t1046
        real t10467
        real t10470
        real t10472
        real t1048
        real t10486
        real t10492
        real t10494
        real t1050
        real t10500
        real t10513
        real t10515
        real t10516
        real t10518
        real t1052
        real t10524
        real t10529
        real t1053
        real t10533
        real t10536
        real t10540
        real t10548
        real t1055
        real t10553
        real t10558
        real t10560
        real t10562
        real t1057
        real t1059
        real t10592
        real t10595
        real t10598
        real t106
        real t10600
        real t10604
        real t1061
        real t10611
        real t10612
        real t10614
        real t10618
        real t10620
        real t1063
        real t10630
        real t10633
        real t10635
        real t10639
        real t10641
        real t1065
        real t1066
        real t10661
        real t10666
        real t10678
        real t1068
        real t10680
        real t10686
        real t10690
        real t10693
        real t10695
        real t1070
        real t10701
        real t10710
        real t10713
        real t10717
        real t1072
        real t10721
        real t10725
        real t10728
        real t10731
        real t10735
        real t10739
        real t1074
        real t10740
        real t10748
        real t10750
        real t1076
        real t10764
        real t10767
        real t10769
        real t1077
        real t10772
        real t10777
        real t1078
        real t10782
        real t10787
        real t1079
        real t10792
        real t10796
        real t108
        real t10800
        real t10808
        real t1081
        real t10812
        real t1082
        real t10822
        real t10827
        real t1083
        real t10837
        real t1084
        real t10841
        real t10846
        real t1086
        real t10869
        real t1087
        real t10884
        real t10889
        real t1089
        real t10897
        real t10898
        real t1090
        real t10901
        real t10909
        real t10919
        real t1092
        real t10924
        real t1094
        real t10946
        real t10949
        real t10954
        real t10956
        real t1096
        real t10962
        real t10966
        real t10969
        real t10971
        real t10977
        real t1098
        real t10982
        real t10987
        real t10988
        real t1099
        real t10993
        real t10997
        real t10999
        real t11
        real t110
        real t1100
        real t11001
        real t11016
        real t1102
        real t11021
        real t1103
        real t11035
        real t11038
        real t11044
        real t11046
        real t11048
        real t1105
        real t11050
        real t11053
        real t11055
        real t11057
        real t11060
        real t11061
        real t11062
        real t11063
        real t11065
        real t11066
        real t11067
        real t11068
        real t1107
        real t11070
        real t11073
        real t11074
        real t11075
        real t11076
        real t11077
        real t11079
        real t11082
        real t11083
        real t1109
        real t11091
        real t11097
        real t11100
        real t11106
        real t11109
        real t1111
        real t11111
        real t11113
        real t11115
        real t11118
        real t1112
        real t11120
        real t11122
        real t11125
        real t11131
        real t11133
        real t11136
        real t1114
        real t11142
        real t11145
        real t11146
        real t11147
        real t11148
        real t11150
        real t11151
        real t11152
        real t11153
        real t11155
        real t11158
        real t11159
        real t1116
        real t11160
        real t11161
        real t11162
        real t11164
        real t11166
        real t11167
        real t11168
        real t11171
        real t11172
        real t11174
        real t11176
        real t11177
        real t11178
        real t1118
        real t11185
        real t11189
        real t11190
        real t11192
        real t11194
        real t11196
        real t11198
        real t1120
        real t11200
        real t11202
        real t11205
        real t11211
        real t11212
        real t11213
        real t11214
        real t11219
        real t1122
        real t11223
        real t11228
        real t1124
        real t11242
        real t11245
        real t1125
        real t11255
        real t11256
        real t11258
        real t11260
        real t11262
        real t11264
        real t11266
        real t11268
        real t1127
        real t11271
        real t11277
        real t11278
        real t1129
        real t11292
        real t11293
        real t11294
        real t113
        real t11307
        real t1131
        real t11310
        real t11314
        real t11320
        real t11321
        real t11323
        real t11325
        real t11327
        real t11329
        real t1133
        real t11331
        real t11333
        real t11336
        real t1134
        real t11342
        real t11343
        real t1135
        real t11351
        real t11355
        real t11359
        real t1136
        real t11360
        real t11362
        real t11364
        real t11366
        real t11368
        real t11370
        real t11371
        real t11372
        real t11375
        real t11378
        real t1138
        real t11381
        real t11382
        real t1139
        real t11390
        real t11403
        real t11407
        real t1141
        real t11412
        real t11418
        real t1142
        real t11424
        real t11425
        real t11426
        real t11429
        real t11430
        real t11431
        real t11433
        real t11438
        real t11439
        real t1144
        real t11440
        real t11449
        real t11450
        real t11453
        real t11454
        real t11456
        real t11458
        real t1146
        real t11460
        real t11462
        real t11464
        real t11466
        real t11469
        real t11475
        real t11476
        real t11477
        real t11478
        real t1148
        real t11487
        real t11492
        real t1150
        real t11506
        real t11509
        real t11519
        real t1152
        real t11520
        real t11522
        real t11524
        real t11526
        real t11528
        real t1153
        real t11530
        real t11532
        real t11535
        real t1154
        real t11541
        real t11542
        real t11547
        real t1155
        real t11556
        real t11557
        real t11558
        real t1156
        real t11563
        real t11568
        real t1157
        real t11571
        real t11574
        real t11575
        real t11578
        real t11584
        real t11585
        real t11587
        real t11589
        real t1159
        real t11591
        real t11593
        real t11595
        real t11597
        real t11600
        real t11606
        real t11607
        real t1161
        real t11615
        real t11619
        real t11623
        real t11624
        real t11626
        real t11628
        real t1163
        real t11630
        real t11632
        real t11634
        real t11636
        real t11639
        real t11645
        real t11646
        real t1165
        real t11654
        real t1166
        real t11667
        real t11671
        real t1168
        real t11682
        real t11687
        real t11688
        real t11689
        real t11690
        real t11693
        real t11694
        real t11695
        real t11697
        real t1170
        real t11702
        real t11703
        real t11704
        real t11710
        real t11713
        real t11714
        real t11717
        real t1172
        real t11722
        real t11724
        real t11725
        real t11727
        real t11729
        real t11731
        real t11733
        real t11735
        real t11737
        real t1174
        real t11740
        real t11746
        real t11747
        real t11748
        real t11749
        real t11758
        real t1176
        real t1177
        real t11778
        real t1179
        real t11795
        real t118
        real t11808
        real t11809
        real t1181
        real t11810
        real t11813
        real t11814
        real t11815
        real t11817
        real t11822
        real t11823
        real t11824
        real t1183
        real t11833
        real t11835
        real t11837
        real t11841
        real t11845
        real t11849
        real t1185
        real t11852
        real t11855
        real t11856
        real t11858
        real t1186
        real t11860
        real t11861
        real t11862
        real t11864
        real t11866
        real t11868
        real t11870
        real t11871
        real t11877
        real t11878
        real t1188
        real t119
        real t1190
        real t11901
        real t11907
        real t11908
        real t11909
        real t11918
        real t11919
        real t1192
        real t11922
        real t11923
        real t11925
        real t11927
        real t11929
        real t11931
        real t11933
        real t11935
        real t11938
        real t1194
        real t11944
        real t11945
        real t11946
        real t11947
        real t11956
        real t1196
        real t11973
        real t11976
        real t1198
        real t11987
        real t1199
        real t11993
        real t11994
        real t11999
        real t120
        real t12006
        real t12007
        real t12008
        real t1201
        real t12011
        real t12012
        real t12013
        real t12015
        real t1202
        real t12020
        real t12021
        real t12022
        real t1203
        real t12031
        real t12035
        real t12039
        real t12043
        real t12047
        real t1205
        real t12053
        real t12054
        real t12056
        real t12058
        real t12060
        real t12062
        real t12064
        real t12066
        real t12069
        real t1207
        real t12075
        real t12076
        real t1209
        real t12099
        real t121
        real t1210
        real t12105
        real t12106
        real t12107
        real t1211
        real t12116
        real t12117
        real t1212
        real t12134
        real t1214
        real t1215
        real t12151
        real t12152
        real t12153
        real t1216
        real t1217
        real t12172
        real t12173
        real t12175
        real t12177
        real t12179
        real t12181
        real t12183
        real t12185
        real t12188
        real t1219
        real t12194
        real t12195
        real t122
        real t12203
        real t12209
        real t12210
        real t12211
        real t1222
        real t12224
        real t12228
        real t1223
        real t12234
        real t12235
        real t12237
        real t12239
        real t12241
        real t12243
        real t12245
        real t12247
        real t12250
        real t12256
        real t12257
        real t1226
        real t12265
        real t1227
        real t12278
        real t1228
        real t12284
        real t12285
        real t12286
        integer t1229
        real t12295
        real t12296
        real t12299
        real t1230
        real t12300
        real t12301
        real t1231
        real t12320
        real t12321
        real t12323
        real t12325
        real t12327
        real t12329
        real t1233
        real t12331
        real t12333
        real t12336
        real t12342
        real t12343
        real t1235
        real t12351
        real t12357
        real t12358
        real t12359
        real t1237
        real t12372
        real t12376
        real t12382
        real t12383
        real t12385
        real t12387
        real t12389
        real t1239
        real t12391
        real t12393
        real t12395
        real t12398
        real t124
        real t12404
        real t12405
        real t1241
        real t12413
        real t12426
        real t1243
        real t12432
        real t12433
        real t12434
        real t1244
        real t12443
        real t12444
        real t12448
        real t12452
        real t12456
        real t12457
        real t12458
        real t1246
        real t12477
        real t12478
        real t12480
        real t12482
        real t12484
        real t12486
        real t12488
        real t12490
        real t12493
        real t12499
        real t12500
        real t12508
        real t1251
        real t12514
        real t12515
        real t12516
        real t1252
        real t12529
        real t1253
        real t12533
        real t12539
        real t12540
        real t12542
        real t12544
        real t12546
        real t12548
        real t12550
        real t12552
        real t12555
        real t12561
        real t12562
        real t12570
        real t1258
        real t12583
        real t12589
        real t1259
        real t12590
        real t12591
        real t126
        real t12600
        real t12601
        real t12604
        real t12605
        real t12606
        real t1261
        real t1262
        real t12625
        real t12626
        real t12628
        real t12630
        real t12632
        real t12634
        real t12636
        real t12638
        real t1264
        real t12641
        real t12647
        real t12648
        real t12656
        real t1266
        real t12662
        real t12663
        real t12664
        real t12677
        real t1268
        real t12681
        real t12687
        real t12688
        real t12690
        real t12692
        real t12694
        real t12696
        real t12698
        real t1270
        real t12700
        real t12703
        real t12709
        real t12710
        real t12718
        real t12731
        real t12737
        real t12738
        real t12739
        real t1274
        real t12748
        real t12749
        real t1275
        real t12753
        real t12766
        integer t1277
        real t1278
        real t12784
        real t12788
        real t1279
        real t12797
        real t12807
        real t12809
        real t1281
        real t12819
        real t12821
        real t1283
        real t12836
        real t12839
        real t12843
        real t12847
        real t1285
        real t12851
        real t12854
        real t12858
        real t12869
        real t1287
        real t12885
        real t12889
        real t1289
        real t12898
        real t12908
        real t1291
        real t12914
        real t12918
        real t12921
        real t12934
        real t12935
        real t1294
        real t12940
        real t12943
        real t12946
        real t12948
        real t12960
        real t12963
        real t12965
        real t12971
        real t12973
        real t12989
        real t1299
        real t13
        real t1300
        real t13000
        real t1301
        real t13017
        real t13022
        real t13024
        real t13030
        real t13036
        real t13041
        real t13046
        real t13048
        real t13052
        real t13057
        real t13059
        real t1306
        real t13066
        real t1307
        real t13070
        real t13075
        real t13085
        real t13086
        real t1309
        real t13091
        real t13097
        real t131
        real t1310
        real t13102
        real t13107
        real t13109
        real t13113
        real t13118
        real t1312
        real t13120
        real t13127
        real t13131
        real t13136
        real t1314
        real t13146
        real t13147
        real t1315
        real t13151
        real t13157
        real t1316
        real t13161
        real t13164
        real t13167
        real t13169
        real t13171
        real t1318
        real t13184
        real t13188
        real t132
        real t13202
        real t13206
        real t13212
        real t13222
        real t13230
        real t13233
        real t13250
        real t13255
        real t13257
        real t13258
        real t13262
        real t13263
        real t13267
        real t1327
        real t13274
        real t13279
        integer t1328
        real t1329
        real t13293
        real t13297
        real t133
        real t13302
        real t1331
        real t13310
        real t13311
        real t13316
        real t13320
        real t13327
        real t13332
        real t13346
        integer t1335
        real t13350
        real t13355
        real t1336
        real t13363
        real t13364
        real t13368
        real t13374
        real t13378
        real t1338
        real t13381
        real t13384
        real t13386
        real t13388
        real t134
        real t13401
        real t13419
        real t13423
        real t13428
        real t13431
        real t13435
        real t13436
        real t13444
        real t13450
        real t13452
        real t13456
        real t13459
        real t1346
        real t13461
        real t13462
        real t13463
        real t13469
        real t1347
        real t1348
        real t13480
        real t13481
        real t13482
        real t13485
        real t13486
        real t13488
        real t13490
        real t13492
        real t13494
        real t13496
        real t13499
        real t135
        real t13500
        real t13507
        real t1351
        real t13512
        real t13514
        real t1352
        real t13523
        real t13529
        real t13533
        real t13536
        real t13539
        real t1354
        real t13541
        real t13543
        real t13551
        real t13553
        real t13557
        real t13560
        real t13562
        real t13563
        real t13566
        real t13567
        real t1357
        real t13573
        real t13584
        real t13585
        real t13586
        real t13589
        real t13590
        real t13591
        real t13592
        real t13593
        real t13597
        real t13599
        real t136
        real t13603
        real t13606
        real t13607
        real t1361
        real t13615
        real t13619
        real t13626
        real t13631
        real t13636
        real t13639
        real t1364
        real t13642
        integer t13643
        real t13644
        real t13646
        real t13654
        real t13655
        real t13657
        real t13659
        real t1366
        real t13661
        real t13663
        real t13665
        real t13667
        real t13670
        real t13676
        real t13677
        real t13678
        real t13679
        real t13681
        real t13684
        real t13687
        real t13698
        real t137
        real t1370
        real t13710
        real t1372
        real t13720
        real t13732
        real t13737
        real t13747
        real t13759
        real t13766
        real t13789
        real t13793
        real t13796
        real t13798
        real t13801
        real t13803
        real t13808
        real t13814
        real t13819
        real t13830
        real t13834
        real t13845
        real t13850
        real t13852
        real t13853
        real t13855
        real t1386
        real t13861
        real t1388
        real t13882
        real t139
        real t13905
        real t13907
        real t13908
        real t13910
        real t13916
        real t1392
        real t1393
        real t13933
        real t13938
        real t1394
        real t13952
        real t13953
        real t13956
        real t13959
        real t13975
        real t140
        real t14004
        real t14014
        real t14018
        real t1404
        real t14045
        real t14048
        real t14051
        real t14053
        real t14057
        real t14064
        real t14065
        real t14067
        real t1407
        real t14071
        real t14073
        real t14087
        real t1409
        real t14092
        real t14099
        real t1412
        real t14123
        real t14127
        real t1413
        real t14131
        real t14139
        real t14143
        real t1415
        real t14152
        real t14154
        real t14168
        real t14171
        real t14173
        real t1418
        real t14196
        real t142
        real t14213
        real t14219
        real t1422
        real t14223
        real t14227
        real t14233
        real t1424
        real t14249
        real t14250
        real t14279
        real t1429
        real t1430
        real t14303
        real t14307
        real t14309
        real t14314
        real t1432
        real t14320
        real t14325
        real t14333
        real t14335
        real t1434
        real t14345
        real t14348
        real t14350
        real t1436
        real t14373
        real t1438
        real t14387
        real t14391
        real t144
        real t1440
        real t14404
        real t14412
        real t14415
        real t14418
        real t14419
        real t1442
        real t14420
        real t14421
        real t14423
        real t14424
        real t14425
        real t14426
        real t14428
        real t14431
        real t14432
        real t14433
        real t14434
        real t14435
        real t14437
        real t14440
        real t14441
        real t14445
        real t14447
        real t14449
        real t1445
        real t14451
        real t14454
        real t14456
        real t14458
        real t14461
        real t14467
        real t14473
        real t14476
        real t14482
        real t14485
        real t14493
        real t14495
        real t14498
        real t145
        real t1450
        real t14504
        real t14507
        real t14509
        real t1451
        real t14511
        real t14513
        real t14516
        real t14518
        real t1452
        real t14520
        real t14523
        real t14524
        real t14525
        real t14526
        real t14528
        real t14529
        real t14530
        real t14531
        real t14533
        real t14536
        real t14537
        real t14538
        real t14539
        real t14540
        real t14542
        real t14545
        real t14546
        real t14549
        real t14550
        real t14552
        real t14554
        real t14555
        real t14563
        real t14568
        real t14569
        real t1457
        real t14570
        real t14579
        real t1458
        real t14598
        real t14599
        real t1460
        real t14601
        real t14603
        real t14605
        real t14607
        real t14609
        real t14611
        real t14614
        real t1462
        real t14620
        real t14621
        real t14635
        real t14636
        real t14637
        real t1464
        real t14650
        real t14653
        real t1466
        real t14670
        real t14686
        real t14690
        real t14697
        real t147
        real t1470
        real t14703
        real t14704
        real t14705
        real t14708
        real t14709
        real t14710
        real t14712
        real t14717
        real t14718
        real t14719
        real t14728
        real t14729
        real t1473
        real t14737
        real t1474
        real t14741
        real t14742
        real t14743
        real t14752
        real t1476
        real t14771
        real t14772
        real t14774
        real t14776
        real t14778
        real t1478
        real t14780
        real t14782
        real t14784
        real t14787
        real t14793
        real t14794
        real t1480
        real t14808
        real t14809
        real t14810
        real t1482
        real t14823
        real t14826
        real t1484
        real t14843
        real t14859
        real t1486
        real t14863
        real t14870
        real t14876
        real t14877
        real t14878
        real t14881
        real t14882
        real t14883
        real t14885
        real t1489
        real t14890
        real t14891
        real t14892
        real t149
        real t14901
        real t14902
        real t14915
        real t1493
        real t14938
        real t14939
        real t1494
        real t14940
        real t14943
        real t14944
        real t14945
        real t14947
        real t1495
        real t14952
        real t14953
        real t14954
        real t1496
        real t14966
        real t14978
        real t14987
        real t14988
        real t14990
        real t14992
        real t14994
        real t14996
        real t14998
        real t15
        real t150
        real t15000
        real t15003
        real t15009
        real t1501
        real t15010
        real t1502
        real t15026
        real t15027
        real t15028
        real t1504
        real t15041
        real t15051
        real t15052
        real t15054
        real t15056
        real t15058
        real t1506
        real t15060
        real t15062
        real t15064
        real t15067
        real t15073
        real t15074
        real t1508
        real t15084
        real t1510
        real t15103
        real t15104
        real t15105
        real t15114
        real t15115
        real t15118
        real t15119
        real t15120
        real t15122
        real t15123
        real t15124
        real t15125
        real t15127
        real t15132
        real t15133
        real t15134
        real t15146
        real t15158
        real t15164
        real t15167
        real t15168
        real t15170
        real t15172
        real t15174
        real t15176
        real t15178
        real t15180
        real t15183
        real t15189
        real t15190
        real t15206
        real t15207
        real t15208
        real t15211
        real t1522
        real t15221
        real t15231
        real t15232
        real t15234
        real t15236
        real t15238
        real t1524
        real t15240
        real t15242
        real t15244
        real t15247
        real t15253
        real t15254
        real t15264
        real t15283
        real t15284
        real t15285
        real t1529
        real t15294
        real t15295
        real t153
        real t15310
        real t15314
        real t15323
        real t1533
        real t15336
        real t1534
        real t1535
        real t15353
        real t15355
        real t15358
        real t15368
        real t1537
        real t15370
        real t15384
        real t1540
        real t15400
        real t15404
        real t15405
        real t15409
        real t1541
        real t15413
        real t15424
        real t1543
        real t15439
        real t15445
        real t15449
        real t15454
        real t15455
        real t15457
        real t1546
        real t15463
        real t15466
        real t15468
        real t1547
        real t15470
        real t15472
        real t15473
        real t15477
        real t15478
        real t15479
        real t1548
        real t15489
        real t15490
        real t15495
        real t15498
        real t1550
        real t15501
        real t15503
        real t15516
        real t15518
        real t15520
        real t15521
        real t15524
        real t15526
        real t1553
        real t15532
        real t15534
        real t15539
        real t1554
        real t15541
        real t15542
        real t15546
        real t15554
        real t15555
        real t15559
        real t1556
        real t15566
        real t15577
        real t15578
        real t15579
        real t15582
        real t15583
        real t15587
        real t15589
        real t15593
        real t15596
        real t15597
        real t15604
        real t15609
        real t15611
        real t15622
        real t15634
        real t1564
        real t15646
        real t15650
        real t15656
        real t1566
        real t15660
        real t15661
        real t15671
        real t15683
        real t15695
        real t15699
        real t1570
        real t15705
        real t15709
        real t15710
        real t15714
        real t1572
        real t15720
        real t15724
        real t15727
        real t15730
        real t15732
        real t15734
        real t15741
        real t15744
        real t15748
        real t15749
        real t1575
        real t15759
        real t15760
        real t15761
        real t15764
        real t15765
        real t15769
        real t15771
        real t15775
        real t15778
        real t15779
        real t15787
        real t15791
        real t158
        real t1580
        real t15807
        real t1581
        real t15818
        real t15835
        real t1584
        real t15840
        real t15842
        real t15851
        real t15857
        real t1586
        real t15861
        real t15864
        real t15867
        real t15869
        real t15871
        real t15884
        real t1589
        real t15895
        real t15899
        real t159
        real t15902
        real t15906
        real t15907
        real t15913
        real t15914
        real t15918
        real t15921
        real t15923
        real t15926
        real t15929
        real t1593
        integer t15930
        real t15931
        real t15933
        real t15941
        real t15943
        real t15950
        real t15953
        real t15955
        real t1596
        real t15976
        real t15977
        real t15979
        real t1598
        real t15981
        real t15983
        real t15985
        real t15987
        real t15989
        real t15992
        real t15998
        real t15999
        real t16000
        real t16001
        real t16003
        real t16006
        real t16009
        real t1602
        real t16038
        real t1604
        real t16042
        real t16048
        real t16050
        real t16056
        real t16068
        real t16072
        real t16077
        real t16080
        real t16090
        real t161
        real t16102
        real t16111
        real t16126
        real t16140
        real t16187
        real t16189
        integer t1619
        real t16190
        real t16192
        real t16198
        real t1620
        real t16203
        real t16208
        real t1621
        real t16219
        real t1623
        real t16238
        real t1625
        real t16253
        real t16263
        real t16267
        real t1627
        real t16282
        real t1629
        real t16292
        real t16304
        real t1631
        real t16311
        real t16322
        real t16325
        real t16328
        real t1633
        real t16330
        real t16334
        real t16341
        real t16346
        real t16348
        real t16352
        real t16354
        real t1636
        real t16366
        real t16369
        real t16375
        real t16379
        real t16383
        real t16389
        real t164
        real t16411
        real t16413
        real t1642
        real t16427
        real t1643
        real t16430
        real t16432
        real t1644
        real t1645
        real t16451
        real t16459
        real t16464
        real t16467
        real t1647
        real t16475
        real t16493
        real t165
        real t1651
        real t16510
        real t16517
        real t1653
        real t16539
        real t16556
        real t16561
        real t1657
        real t16574
        real t1659
        real t166
        real t16607
        real t16611
        real t16615
        real t16623
        real t16627
        real t1663
        real t16637
        real t16647
        real t1665
        real t16651
        real t16668
        real t16679
        real t1668
        real t16682
        real t16685
        real t16686
        real t16687
        real t16689
        real t16690
        real t16691
        real t16692
        real t16694
        real t16697
        real t16698
        real t16699
        real t16700
        real t16701
        real t16703
        real t16706
        real t16707
        real t16711
        real t16713
        real t16715
        real t16717
        real t16720
        real t16722
        real t16724
        real t16727
        real t16733
        real t16739
        real t16742
        real t16748
        real t16751
        real t16759
        real t16761
        real t16764
        real t16770
        real t16773
        real t16775
        real t16777
        real t16779
        real t1678
        real t16782
        real t16784
        real t16786
        real t16789
        real t16790
        real t16791
        real t16792
        real t16794
        real t16795
        real t16796
        real t16797
        real t16799
        real t168
        real t16802
        real t16803
        real t16804
        real t16805
        real t16806
        real t16808
        real t16811
        real t16812
        real t16815
        real t16816
        real t16818
        real t1682
        real t16820
        real t16821
        real t16829
        real t16834
        real t16835
        real t16836
        real t16845
        real t1685
        real t16864
        real t16865
        real t16867
        real t16869
        real t1687
        real t16871
        real t16873
        real t16875
        real t16877
        real t16880
        real t16886
        real t16887
        real t169
        real t1690
        real t16901
        real t16902
        real t16903
        real t16916
        real t16919
        real t16936
        real t1694
        real t1695
        real t16952
        real t16956
        real t1696
        real t16963
        real t16969
        real t16970
        real t16971
        real t16974
        real t16975
        real t16976
        real t16978
        real t16983
        real t16984
        real t16985
        real t16994
        real t16995
        real t17
        real t17003
        real t17007
        real t17008
        real t17009
        real t1701
        real t17018
        real t1702
        real t17037
        real t17038
        real t1704
        real t17040
        real t17042
        real t17044
        real t17046
        real t17048
        real t17050
        real t17053
        real t17059
        real t17060
        real t17074
        real t17075
        real t17076
        real t1708
        real t17089
        real t17092
        real t171
        real t17109
        real t17125
        real t17129
        real t1713
        real t17136
        real t1714
        real t17142
        real t17143
        real t17144
        real t17147
        real t17148
        real t17149
        real t1715
        real t17151
        real t17156
        real t17157
        real t17158
        real t1716
        real t17167
        real t17168
        real t1718
        real t17181
        real t172
        real t17204
        real t17205
        real t17206
        real t17209
        real t1721
        real t17210
        real t17211
        real t17213
        real t17218
        real t17219
        real t17220
        real t17232
        real t1724
        real t17244
        real t17253
        real t17254
        real t17256
        real t17258
        real t17260
        real t17262
        real t17264
        real t17266
        real t17269
        real t1727
        real t17275
        real t17276
        real t1728
        real t17292
        real t17293
        real t17294
        real t17307
        real t1731
        real t17317
        real t17318
        real t17320
        real t17322
        real t17324
        real t17326
        real t17328
        real t17330
        real t17333
        real t17339
        real t1734
        real t17340
        real t17350
        real t17369
        real t17370
        real t17371
        real t17380
        real t17381
        real t17384
        real t17385
        real t17386
        real t17389
        real t17390
        real t17391
        real t17393
        real t17398
        real t17399
        real t174
        real t1740
        real t17400
        real t17412
        real t1742
        real t17424
        real t17433
        real t17434
        real t17436
        real t17438
        real t1744
        real t17440
        real t17442
        real t17444
        real t17446
        real t17449
        real t17455
        real t17456
        real t17457
        real t1746
        real t17466
        real t17472
        real t17473
        real t17474
        real t1748
        real t17487
        real t17497
        real t17498
        real t175
        real t17500
        real t17502
        real t17504
        real t17506
        real t17508
        real t17510
        real t17513
        real t17519
        real t1752
        real t17520
        real t17530
        real t17549
        real t17550
        real t17551
        real t17560
        real t17561
        real t17576
        real t17580
        real t17589
        real t1760
        real t17602
        real t17619
        real t1762
        real t17621
        real t17633
        real t17635
        real t1764
        real t17649
        real t1766
        real t17665
        real t17669
        real t17678
        real t1768
        real t17688
        real t17689
        real t17704
        real t17710
        real t17714
        real t17717
        real t17730
        real t17731
        real t17736
        real t17739
        real t17742
        real t17744
        real t1775
        real t17756
        real t17759
        real t17761
        real t17767
        real t17769
        real t17785
        real t17796
        real t178
        real t1780
        real t17808
        real t17813
        real t17818
        real t1782
        real t17820
        real t17831
        real t17843
        real t17855
        real t17859
        real t17865
        real t17869
        real t1787
        real t17870
        real t17880
        real t17892
        real t179
        real t17904
        real t17908
        real t1791
        real t17914
        real t17918
        real t17919
        real t1792
        real t17923
        real t17929
        real t1793
        real t17933
        real t17936
        real t17939
        real t17941
        real t17943
        real t1795
        real t17956
        real t17974
        real t17978
        real t1798
        real t17983
        real t17986
        real t1799
        real t17991
        real t17999
        integer t180
        real t18005
        real t18007
        real t1801
        real t18011
        real t18014
        real t18021
        real t18032
        real t18033
        real t18034
        real t18037
        real t18038
        real t1804
        real t18042
        real t18044
        real t18048
        real t1805
        real t18051
        real t18052
        real t18059
        real t1806
        real t18064
        real t18066
        real t18075
        real t1808
        real t18081
        real t18085
        real t18088
        real t18091
        real t18093
        real t18095
        real t181
        real t18102
        real t18103
        real t18105
        real t18108
        real t18109
        real t1811
        real t18112
        real t18116
        real t18119
        real t1812
        real t18130
        real t18131
        real t18132
        real t18133
        real t18135
        real t18136
        real t1814
        real t18140
        real t18142
        real t18143
        real t18146
        real t18149
        real t18150
        real t18158
        real t18162
        real t18167
        real t18172
        real t18180
        real t18186
        real t18188
        real t18192
        real t18195
        real t18202
        real t18213
        real t18214
        real t18215
        real t18218
        real t18219
        real t1822
        real t18223
        real t18225
        real t18229
        real t18232
        real t18233
        real t18240
        real t18245
        real t18247
        real t18249
        real t18254
        real t18256
        real t18262
        real t18266
        real t18269
        real t1827
        real t18272
        real t18274
        real t18276
        real t18284
        real t18286
        real t1829
        real t18290
        real t18293
        real t183
        real t1830
        real t18300
        real t18311
        real t18312
        real t18313
        real t18316
        real t18317
        real t1832
        real t18321
        real t18323
        real t18327
        real t1833
        real t18330
        real t18331
        real t18339
        real t18343
        real t18350
        real t18355
        real t18360
        real t18363
        real t18366
        real t1838
        real t18388
        real t18398
        real t184
        integer t18400
        real t18401
        real t18403
        real t18406
        real t18411
        real t18412
        real t18414
        real t18416
        real t18418
        real t1842
        real t18420
        real t18422
        real t18424
        real t18427
        real t18433
        real t18434
        real t18435
        real t18436
        real t18438
        real t18441
        real t18444
        real t18452
        real t18454
        real t1846
        real t18468
        real t18471
        real t18473
        real t1848
        integer t185
        real t18500
        real t18502
        real t18512
        real t18515
        real t18517
        real t18531
        real t18537
        real t18539
        real t18545
        real t18562
        real t1858
        real t18584
        real t18596
        real t186
        real t1860
        real t18616
        real t1862
        real t18628
        real t18638
        real t1864
        real t18650
        real t18657
        real t1866
        real t18671
        real t18685
        real t18697
        real t1870
        real t18707
        real t18719
        real t18728
        real t18730
        real t18736
        real t18745
        real t18748
        real t18751
        real t18753
        real t18757
        real t18764
        real t1878
        real t18784
        real t188
        real t1880
        real t18800
        real t18812
        real t18814
        real t1882
        real t18828
        real t18831
        real t18833
        real t1884
        real t1886
        real t18873
        real t18885
        real t18901
        real t18913
        real t18916
        real t18924
        real t18958
        real t1896
        real t18973
        real t18985
        real t18995
        real t18999
        real t19
        real t190
        real t19018
        real t19029
        real t19040
        real t19043
        real t19046
        real t19047
        real t19048
        real t19049
        real t19051
        real t19052
        real t19053
        real t19054
        real t19056
        real t19059
        real t1906
        real t19060
        real t19061
        real t19062
        real t19063
        real t19065
        real t19068
        real t19069
        real t19077
        real t19083
        real t19086
        real t19092
        real t19095
        real t19097
        real t19099
        real t19101
        real t19103
        real t19106
        real t19108
        real t19110
        real t19113
        real t19119
        real t19121
        real t19124
        real t19130
        real t19133
        real t19134
        real t19135
        real t19136
        real t19138
        real t19139
        real t19140
        real t19141
        real t19143
        real t19146
        real t19147
        real t19148
        real t19149
        real t19150
        real t19152
        real t19155
        real t19156
        real t19160
        real t19162
        real t19164
        real t19167
        real t19169
        real t19171
        real t19174
        real t19177
        real t19178
        real t1918
        real t19180
        real t19182
        real t19183
        real t19191
        real t19199
        real t192
        real t19208
        real t19209
        real t19210
        real t1922
        real t19228
        real t1924
        real t19243
        real t19245
        real t19258
        real t19259
        real t19260
        real t19263
        real t19264
        real t19265
        real t19267
        real t19272
        real t19273
        real t19274
        real t19283
        real t19287
        real t19291
        real t19295
        real t19299
        real t193
        real t1930
        real t19301
        real t19305
        real t19306
        real t19308
        real t19310
        real t19312
        real t19314
        real t19316
        real t19318
        real t19321
        real t19327
        real t19328
        real t19357
        real t19358
        real t19359
        real t19368
        real t19369
        real t19377
        real t19381
        real t19382
        real t19383
        real t1940
        real t19401
        real t19418
        real t19431
        real t19432
        real t19433
        real t19436
        real t19437
        real t19438
        real t1944
        real t19440
        real t19445
        real t19446
        real t19447
        real t19456
        real t19460
        real t19464
        real t19468
        real t1947
        real t19472
        real t19478
        real t19479
        real t19481
        real t19483
        real t19485
        real t19487
        real t19489
        real t1949
        real t19491
        real t19494
        real t19496
        real t19500
        real t19501
        real t19530
        real t19531
        real t19532
        real t19541
        real t19542
        real t1955
        real t19555
        real t19568
        real t19569
        real t19570
        real t19573
        real t19574
        real t19575
        real t19577
        real t19582
        real t19583
        real t19584
        real t19596
        real t196
        real t19608
        real t19626
        real t19627
        real t19628
        real t19637
        real t19647
        real t19648
        real t19649
        real t19650
        real t19652
        real t19654
        real t19656
        real t19658
        real t19660
        real t19663
        real t19669
        real t1967
        real t19670
        real t19699
        real t197
        real t19700
        real t19701
        real t19710
        real t19711
        real t19719
        real t19723
        real t19724
        real t19725
        real t19728
        real t19729
        real t1973
        real t19730
        real t19732
        real t19737
        real t19738
        real t19739
        real t19751
        real t19763
        real t19781
        real t19782
        real t19783
        real t19792
        real t198
        real t1980
        real t19802
        real t19803
        real t19805
        real t19807
        real t19809
        real t19811
        real t19813
        real t19815
        real t19818
        real t19824
        real t19825
        real t1984
        real t19854
        real t19855
        real t19856
        real t19865
        real t19866
        real t1990
        real t19901
        real t19903
        real t19906
        real t19914
        real t19928
        real t19930
        real t19941
        real t19944
        real t19946
        real t19952
        real t19962
        real t19966
        real t19972
        real t19975
        real t19981
        real t19985
        real t19990
        real t19991
        real t19993
        real t19999
        real t2
        real t200
        real t20002
        real t20004
        real t20006
        real t20008
        real t20009
        real t20013
        real t20014
        real t20015
        real t2002
        real t20025
        real t20026
        real t20031
        real t20034
        real t20037
        real t20039
        real t20052
        real t20054
        real t20056
        real t20057
        real t20060
        real t20062
        real t20068
        real t20070
        real t20081
        real t20086
        real t20087
        real t20097
        real t201
        real t20114
        real t20119
        real t2012
        real t20121
        real t20130
        real t20136
        real t20140
        real t20143
        real t20146
        real t20148
        real t20150
        real t20163
        real t20181
        real t20185
        real t20201
        real t20212
        real t20217
        real t20222
        real t20229
        real t20234
        real t20236
        real t2024
        real t20245
        real t20251
        real t20255
        real t20258
        real t20261
        real t20263
        real t20265
        real t20278
        real t2028
        real t20296
        real t203
        real t20300
        real t20307
        real t20312
        real t20317
        real t20320
        real t20323
        real t20327
        real t2033
        real t2035
        integer t20351
        real t20352
        real t20354
        real t20362
        real t20364
        real t20371
        real t20374
        real t20376
        real t20391
        real t20401
        real t2041
        real t20413
        real t20420
        real t20431
        real t20432
        real t20434
        real t20436
        real t20438
        real t20440
        real t20442
        real t20444
        real t20447
        real t2045
        real t20453
        real t20454
        real t20459
        real t20461
        real t20462
        real t20464
        real t20470
        real t2049
        real t20498
        real t205
        real t2051
        real t20514
        real t20529
        real t20545
        real t20557
        real t2056
        real t20565
        real t20566
        real t20567
        real t20569
        real t20572
        real t20575
        real t20584
        real t2059
        real t20594
        real t20598
        real t20617
        real t20619
        real t2062
        real t20625
        real t2063
        real t20635
        real t20645
        real t2065
        real t20657
        real t20669
        real t2069
        real t207
        real t2070
        real t20702
        real t20705
        real t20708
        real t20710
        real t20714
        real t20721
        real t20727
        real t2073
        real t20756
        real t20758
        real t2077
        real t20772
        real t20775
        real t20777
        real t2078
        real t2079
        real t20796
        real t2080
        real t20807
        real t2081
        real t20834
        real t20837
        real t2084
        real t20845
        real t20854
        real t20858
        real t20870
        real t2088
        real t20880
        real t20884
        real t209
        real t20905
        real t2092
        real t20938
        real t20955
        real t2096
        real t20986
        real t2099
        real t20997
        real t210
        real t2100
        real t21000
        real t21003
        real t21004
        real t21005
        real t21007
        real t21008
        real t21009
        real t21010
        real t21012
        real t21015
        real t21016
        real t21017
        real t21018
        real t21019
        real t21021
        real t21024
        real t21025
        real t2103
        real t21033
        real t21039
        real t21042
        real t21048
        real t21051
        real t21053
        real t21055
        real t21057
        real t21059
        real t21062
        real t21064
        real t21066
        real t21069
        real t2107
        real t21075
        real t21077
        real t21080
        real t21086
        real t21089
        real t21090
        real t21091
        real t21092
        real t21094
        real t21095
        real t21096
        real t21097
        real t21099
        real t211
        real t21102
        real t21103
        real t21104
        real t21105
        real t21106
        real t21108
        real t2111
        real t21111
        real t21112
        real t21116
        real t21118
        real t21120
        real t21123
        real t21125
        real t21127
        real t21130
        real t21133
        real t21134
        real t21136
        real t21138
        real t21139
        real t21147
        real t21155
        real t21164
        real t21165
        real t21166
        real t21184
        real t21201
        real t2121
        real t21214
        real t21215
        real t21216
        real t21219
        real t21220
        real t21221
        real t21223
        real t21228
        real t21229
        real t21230
        real t21239
        real t2124
        real t21243
        real t21247
        real t21251
        real t21255
        real t2126
        real t21261
        real t21262
        real t21264
        real t21266
        real t21268
        real t2127
        real t21270
        real t21272
        real t21274
        real t21277
        real t21283
        real t21284
        real t2129
        real t21313
        real t21314
        real t21315
        real t21324
        real t21325
        real t21333
        real t21337
        real t21338
        real t21339
        real t2135
        real t21357
        real t21374
        real t21387
        real t21388
        real t21389
        real t2139
        real t21392
        real t21393
        real t21394
        real t21396
        real t214
        real t21401
        real t21402
        real t21403
        real t21412
        real t21416
        real t2142
        real t21420
        real t21424
        real t21428
        real t21434
        real t21435
        real t21437
        real t21439
        real t2144
        real t21441
        real t21443
        real t21445
        real t21447
        real t2145
        real t21450
        real t21456
        real t21457
        real t2147
        real t21486
        real t21487
        real t21488
        real t21497
        real t21498
        real t215
        real t21511
        real t21524
        real t21525
        real t21526
        real t21529
        real t2153
        real t21530
        real t21531
        real t21533
        real t21538
        real t21539
        real t21540
        real t21552
        real t21564
        real t21582
        real t21583
        real t21584
        real t21593
        real t216
        real t21603
        real t21604
        real t21606
        real t21608
        real t21610
        real t21612
        real t21614
        real t21616
        real t21619
        real t2162
        real t21625
        real t21626
        real t2164
        real t21655
        real t21656
        real t21657
        real t21666
        real t21667
        real t21675
        real t21679
        real t21680
        real t21681
        real t21684
        real t21685
        real t21686
        real t21688
        real t21693
        real t21694
        real t21695
        real t2170
        real t21707
        real t21719
        real t21737
        real t21738
        real t21739
        real t21748
        real t21758
        real t21759
        real t21761
        real t21763
        real t21765
        real t21767
        real t21769
        real t21771
        real t21774
        real t21780
        real t21781
        real t218
        real t2180
        real t21810
        real t21811
        real t21812
        real t21821
        real t21822
        real t2184
        real t21857
        real t21859
        real t21869
        real t2187
        real t21883
        real t21885
        real t2189
        real t21899
        real t219
        real t21917
        real t21930
        real t21936
        real t21940
        real t21943
        real t2195
        real t21956
        real t21957
        real t21962
        real t21965
        real t21968
        real t21970
        real t21982
        real t21985
        real t21987
        real t21993
        real t21995
        real t22
        real t22000
        real t22034
        real t2206
        real t22067
        real t2207
        real t2209
        real t221
        real t2210
        real t22100
        real t2212
        real t2218
        real t2222
        real t2225
        real t2226
        real t2228
        real t2229
        real t223
        real t2231
        real t2237
        real t2240
        real t2244
        real t2247
        real t225
        real t2252
        real t2261
        real t2264
        real t2266
        real t2269
        real t227
        real t2273
        real t2277
        real t228
        real t2282
        real t2283
        real t2288
        real t2297
        real t2307
        real t2309
        real t231
        real t2314
        real t232
        real t2320
        real t2325
        integer t233
        real t2333
        real t2335
        real t2339
        real t234
        real t2341
        real t2350
        real t2353
        real t2355
        real t2358
        real t236
        real t2362
        real t2365
        real t2367
        real t237
        real t2371
        real t2373
        integer t238
        real t2387
        real t239
        real t2392
        real t2396
        real t2399
        real t2400
        real t2408
        real t241
        real t2412
        real t2422
        real t2427
        real t243
        real t2430
        real t2436
        real t2440
        real t2442
        real t2447
        real t245
        real t2453
        real t2458
        real t2466
        real t2468
        real t2474
        real t248
        real t2481
        real t2484
        real t2486
        real t249
        real t2492
        real t250
        real t2512
        real t2516
        real t2519
        real t252
        real t2520
        real t2522
        real t253
        real t2536
        real t2540
        real t2543
        real t2544
        real t2546
        real t255
        real t2552
        real t2557
        real t2563
        real t2567
        real t2568
        real t257
        real t2571
        real t2574
        real t2576
        real t2579
        real t2582
        real t2583
        real t2585
        real t259
        real t2591
        real t2596
        real t2605
        real t2606
        real t261
        real t2610
        real t2613
        real t2615
        real t2618
        real t262
        real t2622
        real t2624
        real t2629
        real t2632
        real t2634
        real t2635
        real t2638
        real t2639
        real t2641
        real t2643
        real t2645
        real t2647
        real t2649
        real t265
        real t2651
        real t2652
        real t2654
        real t2659
        real t266
        real t2660
        real t2661
        real t2666
        real t2667
        real t2669
        real t267
        real t2671
        real t2673
        real t2676
        real t2677
        real t2678
        real t2680
        real t2682
        real t2684
        real t2686
        real t2688
        real t269
        real t2690
        real t2693
        real t2698
        real t2699
        real t27
        real t270
        real t2700
        real t2706
        real t2708
        real t2711
        real t2712
        real t2713
        real t2714
        real t2716
        real t2717
        real t2718
        real t2719
        real t272
        real t2721
        real t2724
        real t2725
        real t2726
        real t2727
        real t2728
        real t2730
        real t2733
        real t2734
        real t274
        real t2741
        real t2743
        real t2744
        real t2746
        real t2748
        real t2750
        real t2756
        real t2758
        real t2759
        real t276
        real t2764
        real t2765
        real t2766
        real t2767
        real t2769
        real t2771
        real t2773
        real t2776
        real t2777
        real t2778
        real t278
        real t2780
        real t2782
        real t2784
        real t2786
        real t2788
        real t279
        real t2790
        real t2793
        real t2798
        real t2799
        real t28
        real t280
        real t2800
        real t2806
        real t2808
        real t281
        real t2810
        real t2813
        real t2814
        real t2815
        real t2817
        real t2819
        real t2821
        real t2823
        real t2825
        real t2827
        real t283
        real t2830
        real t2835
        real t2836
        real t2837
        real t2843
        real t2845
        real t2848
        real t285
        real t2854
        real t2856
        real t2858
        real t2860
        real t2862
        real t2865
        real t287
        real t2871
        real t2873
        real t2875
        real t2877
        real t2880
        real t2881
        real t2882
        real t2883
        real t2885
        real t2886
        real t2887
        real t2888
        real t289
        real t2890
        real t2893
        real t2894
        real t2895
        real t2896
        real t2897
        real t2899
        real t29
        real t2902
        real t2903
        real t2906
        real t2907
        real t2909
        real t291
        real t2910
        real t2912
        real t2913
        real t2921
        real t2922
        real t2923
        real t2925
        real t2928
        real t2929
        real t293
        real t2931
        real t2933
        real t2935
        real t2937
        real t2939
        real t2941
        real t2944
        real t2950
        real t2951
        real t2952
        real t2953
        real t2956
        real t2957
        real t2958
        real t296
        real t2960
        real t2965
        real t2966
        real t2967
        real t2969
        real t2972
        real t2973
        real t2976
        real t2979
        real t2981
        real t2989
        real t2991
        real t2996
        real t2998
        real t30
        real t3000
        real t3001
        real t3006
        real t3009
        real t301
        real t3014
        real t302
        real t3020
        real t3021
        real t3026
        real t303
        real t3030
        real t3032
        real t3033
        real t3034
        real t3035
        real t3037
        real t3038
        real t3039
        real t3041
        real t3043
        real t3045
        real t3047
        real t3050
        real t3056
        real t3057
        real t306
        real t3071
        real t3072
        real t3073
        real t3086
        real t3089
        real t309
        real t3093
        real t3095
        real t3099
        real t31
        real t3100
        real t3101
        real t3102
        real t3104
        real t3106
        real t3108
        real t311
        real t3110
        real t3112
        real t3115
        real t3121
        real t3122
        real t313
        real t3130
        real t3132
        real t3136
        real t3140
        real t3141
        real t3143
        real t3145
        real t3147
        real t3149
        real t315
        real t3151
        real t3153
        real t3156
        real t3162
        real t3163
        real t317
        real t3171
        real t3173
        real t3186
        real t319
        real t3190
        real t3201
        real t3207
        real t3208
        real t3209
        real t321
        real t3212
        real t3213
        real t3214
        real t3216
        real t322
        real t3221
        real t3222
        real t3223
        real t323
        real t3232
        real t3233
        real t3236
        real t3237
        real t3239
        real t324
        real t3241
        real t3243
        real t3245
        real t3247
        real t3249
        real t3252
        real t3258
        real t3259
        real t326
        real t3260
        real t3261
        real t3264
        real t3265
        real t3266
        real t3268
        real t3273
        real t3274
        real t3275
        real t3277
        real t3278
        real t328
        real t3280
        real t3281
        real t3284
        real t3289
        real t3297
        real t3299
        real t33
        real t330
        real t3304
        real t3306
        real t3308
        real t3309
        real t3314
        real t3315
        real t3317
        real t332
        real t3321
        real t3326
        real t3329
        real t3333
        real t3338
        real t334
        real t3340
        real t3341
        real t3342
        real t3343
        real t3345
        real t3347
        real t3349
        real t3351
        real t3353
        real t3355
        real t3358
        real t336
        real t3364
        real t3365
        real t3377
        real t3379
        real t3380
        real t3381
        real t339
        real t3394
        real t3397
        real t34
        real t3401
        real t3402
        real t3407
        real t3408
        real t3410
        real t3412
        real t3414
        real t3416
        real t3418
        real t3420
        real t3423
        real t3429
        real t3430
        real t3433
        real t3438
        real t344
        real t3440
        real t3443
        real t3444
        real t3448
        real t3449
        real t345
        real t3451
        real t3453
        real t3455
        real t3456
        real t3457
        real t3459
        real t346
        real t3461
        real t3464
        real t3470
        real t3471
        real t3479
        real t348
        real t3481
        real t3487
        real t3494
        real t3498
        real t35
        real t3509
        real t3511
        real t3515
        real t3516
        real t3517
        real t352
        real t3520
        real t3521
        real t3522
        real t3524
        real t3529
        real t3530
        real t3531
        real t354
        real t3540
        real t3541
        real t3543
        real t3548
        real t3549
        real t3550
        real t3552
        real t3554
        real t3555
        real t3556
        real t3558
        real t356
        real t3560
        real t3562
        real t3564
        real t3565
        real t3568
        real t3571
        real t3573
        real t3574
        real t3575
        real t3576
        real t3577
        real t3578
        real t358
        real t3580
        real t3582
        real t3584
        real t3586
        real t3588
        real t3590
        real t3593
        real t3598
        real t3599
        real t36
        real t360
        real t3600
        real t3606
        real t3608
        real t361
        real t3610
        real t3612
        real t3615
        real t3616
        real t3617
        real t3619
        real t362
        real t3621
        real t3623
        real t3625
        real t3627
        real t3629
        real t363
        real t3632
        real t3636
        real t3637
        real t3638
        real t3639
        real t364
        real t3645
        real t3647
        real t3649
        real t3651
        real t3652
        real t3658
        real t366
        real t3660
        real t3662
        real t3665
        real t367
        real t3671
        real t3673
        real t3676
        real t3677
        real t3678
        real t3679
        real t368
        real t3681
        real t3682
        real t3683
        real t3684
        real t3686
        real t3688
        real t3689
        real t369
        real t3690
        real t3691
        real t3692
        real t3693
        real t3695
        real t3698
        real t3699
        real t3702
        real t3703
        real t3705
        real t3706
        real t3707
        real t3708
        real t371
        real t3710
        real t3713
        real t3714
        real t3716
        real t3718
        real t3719
        real t3720
        real t3722
        real t3723
        real t3729
        real t3731
        real t3732
        real t3733
        real t3734
        real t3735
        real t3736
        real t3738
        real t374
        real t3740
        real t3742
        real t3744
        real t3746
        real t3748
        real t375
        real t3751
        real t3756
        real t3757
        real t3758
        real t3759
        real t376
        real t3764
        real t3766
        real t3768
        real t377
        real t3770
        real t3773
        real t3774
        real t3775
        real t3777
        real t3779
        real t378
        real t3781
        real t3783
        real t3785
        real t3787
        real t3790
        real t3793
        real t3795
        real t3796
        real t3797
        real t38
        real t380
        real t3803
        real t3805
        real t3806
        real t3807
        real t3810
        real t3816
        real t3817
        real t3818
        real t3820
        real t3823
        real t3829
        real t383
        real t3831
        real t3834
        real t3835
        real t3836
        real t3837
        real t3839
        real t384
        real t3840
        real t3841
        real t3842
        real t3844
        real t3847
        real t3848
        real t3849
        real t3850
        real t3851
        real t3853
        real t3856
        real t3857
        real t386
        real t3860
        real t3861
        real t3863
        real t3865
        real t3867
        real t3871
        real t3872
        real t3874
        real t3876
        real t3878
        real t388
        real t3880
        real t3882
        real t3883
        real t3884
        real t3887
        real t3892
        real t3893
        real t3894
        real t3895
        real t3896
        real t3898
        real t3899
        real t3901
        real t3902
        real t3904
        real t3905
        real t391
        real t3910
        real t3912
        real t3914
        real t3916
        real t3918
        real t3919
        real t3924
        real t3926
        real t3927
        real t3929
        real t393
        real t3931
        real t3933
        real t3935
        real t3936
        real t3937
        real t3938
        real t394
        real t3940
        real t3941
        real t3942
        real t3944
        real t3946
        real t3948
        real t3950
        real t3953
        real t3958
        real t3959
        real t396
        real t3960
        real t3962
        real t3964
        real t3966
        real t3968
        real t397
        real t3970
        real t3972
        real t3973
        real t3974
        real t3975
        real t3976
        real t3978
        real t398
        real t3981
        real t3982
        real t3984
        real t3988
        real t3989
        real t3991
        real t3992
        real t3994
        real t3996
        real t3998
        real t4
        real t40
        real t400
        real t4000
        real t4001
        real t4002
        real t4003
        real t4005
        real t4006
        real t4007
        real t4009
        real t4011
        real t4013
        real t4015
        real t4018
        real t4023
        real t4024
        real t4025
        real t4031
        real t4033
        real t4035
        real t4037
        real t4039
        real t4040
        real t4041
        real t4042
        real t4043
        real t4044
        real t4046
        real t4048
        real t4050
        real t4052
        real t4054
        real t4056
        real t4057
        real t406
        real t4062
        real t4063
        real t4064
        real t4067
        real t4070
        real t4072
        real t4074
        real t4076
        real t4077
        real t408
        real t4083
        real t4085
        real t4087
        real t4089
        real t409
        real t4091
        real t4092
        real t4098
        real t4100
        real t4102
        real t4104
        real t4105
        real t4106
        real t4107
        real t4108
        real t411
        real t4110
        real t4111
        real t4112
        real t4113
        real t4115
        real t4118
        real t4119
        real t4120
        real t4121
        real t4122
        real t4124
        real t4127
        real t4128
        real t4130
        real t4131
        real t4132
        real t4133
        real t4134
        real t4135
        real t4136
        real t4138
        real t414
        real t4140
        real t4142
        real t4144
        real t4146
        real t4147
        real t4148
        real t4151
        real t4153
        real t4156
        real t4157
        real t4158
        real t4159
        real t416
        real t4160
        real t4162
        real t4165
        real t4166
        real t4168
        real t4169
        real t417
        real t4174
        real t4176
        real t4178
        real t4180
        real t4182
        real t4183
        real t4188
        real t419
        real t4190
        real t4191
        real t4193
        real t4195
        real t4197
        real t4199
        real t42
        real t4200
        real t4201
        real t4202
        real t4204
        real t4206
        real t4208
        real t421
        real t4210
        real t4212
        real t4214
        real t4217
        real t4222
        real t4223
        real t4224
        real t4228
        real t423
        real t4230
        real t4232
        real t4234
        real t4236
        real t4237
        real t4238
        real t4239
        real t4240
        real t4242
        real t4245
        real t4246
        real t4248
        real t425
        real t4252
        real t4253
        real t4255
        real t4256
        real t4258
        real t426
        real t4260
        real t4262
        real t4264
        real t4265
        real t4266
        real t4267
        real t4269
        real t427
        real t4271
        real t4273
        real t4275
        real t4277
        real t4279
        real t428
        real t4282
        real t4287
        real t4288
        real t4289
        real t4295
        real t4297
        real t4299
        real t430
        real t4301
        real t4303
        real t4304
        real t4305
        real t4306
        real t4308
        real t4310
        real t4311
        real t4312
        real t4314
        real t4316
        real t4318
        real t432
        real t4320
        real t4321
        real t4326
        real t4327
        real t4328
        real t4334
        real t4336
        real t4338
        real t434
        real t4340
        real t4341
        real t4347
        real t4349
        real t4351
        real t4353
        real t4355
        real t4356
        real t436
        real t4362
        real t4364
        real t4366
        real t4368
        real t4369
        real t4370
        real t4371
        real t4372
        real t4373
        real t4374
        real t4375
        real t4376
        real t4377
        real t4379
        real t438
        real t4382
        real t4383
        real t4384
        real t4385
        real t4386
        real t4388
        real t4391
        real t4392
        real t4394
        real t4395
        real t4396
        real t4397
        real t4398
        real t44
        real t440
        real t4400
        real t4402
        real t4405
        real t4406
        real t4407
        real t4409
        real t4411
        real t4413
        real t4415
        real t4417
        real t4419
        real t4422
        real t4428
        real t4429
        real t443
        real t4430
        real t4431
        real t4434
        real t4435
        real t4436
        real t4438
        real t4443
        real t4444
        real t4445
        real t4447
        real t4450
        real t4451
        real t4454
        real t4464
        real t4468
        real t4472
        real t448
        real t4481
        real t4483
        real t4484
        real t4489
        real t449
        real t4497
        real t4499
        real t450
        real t4504
        real t4506
        real t4508
        real t4509
        real t4517
        real t452
        real t4530
        real t4531
        real t4532
        real t4535
        real t4536
        real t4537
        real t4539
        real t4544
        real t4545
        real t4546
        real t4555
        real t4559
        real t456
        real t4563
        real t4567
        real t4571
        real t4574
        real t4577
        real t4578
        real t458
        real t4580
        real t4582
        real t4583
        real t4584
        real t4586
        real t4588
        real t4590
        real t4593
        real t4599
        real t46
        real t460
        real t4600
        real t4618
        real t462
        real t4623
        real t4629
        real t4630
        real t4631
        real t464
        real t4640
        real t4641
        real t4644
        real t4645
        real t4647
        real t4649
        real t4651
        real t4653
        real t4655
        real t4657
        real t466
        real t4660
        real t4666
        real t4667
        real t4668
        real t4669
        real t467
        real t4670
        real t4672
        real t4673
        real t4674
        real t4676
        real t468
        real t4681
        real t4682
        real t4683
        real t4684
        real t4685
        real t4688
        real t4689
        real t469
        real t4692
        real t4694
        real t471
        real t4710
        real t4719
        real t4721
        real t4722
        real t4726
        real t4727
        real t473
        real t4734
        real t4735
        real t4737
        real t4742
        real t4743
        real t4744
        real t4746
        real t4747
        real t475
        real t4755
        real t4768
        real t4769
        real t477
        real t4770
        real t4773
        real t4774
        real t4775
        real t4777
        real t4782
        real t4783
        real t4784
        real t479
        real t4793
        real t4797
        real t48
        real t4801
        real t4804
        real t4805
        real t4809
        real t481
        real t4813
        real t4815
        real t4816
        real t4818
        real t4820
        real t4822
        real t4823
        real t4824
        real t4826
        real t4828
        real t4831
        real t4832
        real t4837
        real t4838
        real t484
        real t4861
        real t4867
        real t4868
        real t4869
        real t4872
        real t4878
        real t4879
        real t4882
        real t4886
        real t4887
        real t4888
        real t489
        real t4890
        real t4893
        real t4894
        real t4896
        real t490
        real t4902
        real t4904
        real t4905
        real t4907
        real t4909
        real t491
        real t4911
        real t4912
        real t4918
        real t492
        real t4920
        real t4923
        real t4929
        real t4931
        real t4932
        real t4933
        real t4934
        real t4935
        real t4937
        real t4938
        real t4939
        real t4940
        real t4942
        real t4945
        real t4946
        real t4947
        real t4948
        real t4949
        real t4951
        real t4953
        real t4954
        real t4955
        real t4959
        real t4961
        real t4962
        real t4963
        real t4966
        real t4968
        real t497
        real t4970
        real t4971
        real t4973
        real t4974
        real t4975
        real t4976
        real t4977
        real t4979
        real t4980
        real t4981
        real t4982
        real t4984
        real t4987
        real t4988
        real t499
        real t4990
        real t4996
        real t4998
        real t4999
        integer t5
        real t5001
        real t5003
        real t5005
        real t5006
        real t501
        real t5012
        real t5014
        real t5017
        real t5023
        real t5026
        real t5027
        real t5028
        real t5029
        real t503
        real t5031
        real t5032
        real t5033
        real t5034
        real t5036
        real t5039
        real t5040
        real t5041
        real t5042
        real t5043
        real t5045
        real t5046
        real t5048
        real t5049
        real t505
        real t5053
        real t5055
        real t5057
        real t5059
        real t506
        real t5060
        real t5062
        real t5064
        real t5067
        real t5068
        real t5069
        real t507
        real t5070
        real t5071
        real t5073
        real t5075
        real t5077
        real t5081
        real t5082
        real t5084
        real t5086
        real t5088
        real t5090
        real t5092
        real t5094
        real t5097
        real t51
        real t5102
        real t5103
        real t5104
        real t5105
        real t5106
        real t5108
        real t5111
        real t5112
        real t5114
        real t5115
        real t512
        real t5121
        real t5123
        real t5125
        real t5127
        real t5129
        real t5130
        real t5135
        real t5137
        real t5139
        real t514
        real t5141
        real t5143
        real t5144
        real t5150
        real t5152
        real t5154
        real t5155
        real t516
        real t5161
        real t5163
        real t5164
        real t5165
        real t5166
        real t5167
        real t5169
        real t5170
        real t5171
        real t5172
        real t5174
        real t5177
        real t5178
        real t5179
        real t518
        real t5180
        real t5181
        real t5183
        real t5186
        real t5187
        real t5189
        real t5190
        real t5192
        real t5194
        real t5196
        real t5198
        real t520
        real t5200
        real t5201
        real t5202
        real t5204
        real t5206
        real t5208
        real t521
        real t5210
        real t5211
        real t5212
        real t5213
        real t5215
        real t5217
        real t5219
        real t522
        real t5221
        real t5223
        real t5225
        real t5228
        real t523
        real t5233
        real t5234
        real t5235
        real t5239
        real t5241
        real t5243
        real t5244
        real t5245
        real t5247
        real t5248
        real t5252
        real t5254
        real t5256
        real t5258
        real t5260
        real t5262
        real t5263
        real t5264
        real t5265
        real t5266
        real t5268
        real t5271
        real t5272
        real t5273
        real t5274
        real t5275
        real t5276
        real t5277
        real t5278
        real t5279
        real t5280
        real t5282
        real t5284
        real t5286
        real t5288
        real t5289
        real t529
        real t5290
        real t5292
        real t5295
        real t5297
        real t5300
        real t5301
        real t5302
        real t5303
        real t5304
        real t5306
        real t5309
        real t531
        real t5310
        real t5312
        real t5313
        real t5319
        real t5321
        real t5323
        real t5325
        real t5327
        real t5328
        real t533
        real t5333
        real t5335
        real t5337
        real t5339
        real t5341
        real t5342
        real t5348
        real t535
        real t5350
        real t5352
        real t5353
        real t5359
        real t5361
        real t5362
        real t5363
        real t5364
        real t5365
        real t5367
        real t5368
        real t5369
        real t537
        real t5370
        real t5372
        real t5375
        real t5376
        real t5377
        real t5378
        real t5379
        real t538
        real t5381
        real t5384
        real t5385
        real t5387
        real t5388
        real t539
        real t5390
        real t5392
        real t5394
        real t5396
        real t5398
        real t5399
        real t540
        real t5400
        real t5402
        real t5404
        real t5406
        real t5408
        real t5409
        real t541
        real t5410
        real t5411
        real t5413
        real t5415
        real t5417
        real t5419
        real t5421
        real t5423
        real t5426
        real t543
        real t5431
        real t5432
        real t5433
        real t5437
        real t5439
        real t544
        real t5441
        real t5443
        real t5445
        real t5446
        real t545
        real t5450
        real t5452
        real t5454
        real t5456
        real t5458
        real t546
        real t5460
        real t5461
        real t5462
        real t5463
        real t5464
        real t5466
        real t5469
        real t5470
        real t5472
        real t5473
        real t5474
        real t5475
        real t5476
        real t5478
        real t548
        real t5480
        real t5483
        real t5487
        real t5489
        real t5493
        real t5500
        real t5504
        real t5509
        real t551
        real t5512
        real t5513
        real t5514
        real t5517
        real t5518
        real t5519
        real t552
        real t5520
        real t5521
        real t5526
        real t5527
        real t5528
        real t553
        real t5530
        real t5533
        real t5534
        real t554
        real t5540
        real t5545
        real t5548
        real t555
        real t5552
        real t5557
        real t5560
        real t5561
        real t5562
        real t5564
        real t5566
        real t5568
        real t557
        real t5570
        real t5572
        real t5574
        real t5577
        real t5583
        real t5584
        real t5592
        real t5594
        real t56
        real t560
        real t5600
        real t5601
        real t5602
        real t561
        real t5615
        real t5619
        real t5625
        real t5626
        real t5628
        real t563
        real t5630
        real t5632
        real t5634
        real t5636
        real t5638
        real t564
        real t5641
        real t5647
        real t5648
        real t565
        real t5656
        real t5658
        real t5661
        real t5671
        real t5676
        real t5677
        real t5678
        real t5679
        real t568
        real t5684
        real t5688
        real t5689
        real t569
        real t5691
        real t5692
        real t5693
        real t5694
        real t5697
        real t5698
        real t5699
        real t57
        real t5701
        real t5706
        real t5707
        real t5708
        real t571
        real t5710
        real t5713
        real t5714
        real t572
        real t5720
        real t5725
        real t5728
        real t573
        real t5732
        real t5737
        real t574
        real t5740
        real t5741
        real t5742
        real t5744
        real t5746
        real t5748
        real t575
        real t5750
        real t5752
        real t5754
        real t5757
        real t5763
        real t5764
        real t5772
        real t5774
        real t5780
        real t5781
        real t5782
        real t579
        real t5795
        real t5799
        real t58
        real t580
        real t5805
        real t5806
        real t5808
        real t5810
        real t5812
        real t5814
        real t5816
        real t5818
        real t582
        real t5821
        real t5827
        real t5828
        real t583
        real t5830
        real t5836
        real t5838
        real t5846
        real t585
        real t5851
        real t5853
        real t5857
        real t5858
        real t5859
        real t5861
        real t5868
        real t5869
        real t587
        real t5873
        real t5877
        real t5881
        real t5882
        real t5883
        real t5886
        real t5887
        real t5888
        real t589
        real t5890
        real t5895
        real t5896
        real t5897
        real t5899
        real t59
        real t590
        real t5902
        real t5903
        real t5909
        real t591
        real t5914
        real t5917
        real t592
        real t5921
        real t5926
        real t5929
        real t5930
        real t5931
        real t5933
        real t5935
        real t5937
        real t5939
        real t5941
        real t5943
        real t5946
        real t5952
        real t5953
        real t596
        real t5961
        real t5963
        real t5969
        real t597
        real t5970
        real t5971
        real t5984
        real t5988
        real t599
        real t5994
        real t5995
        real t5997
        real t5999
        real t6
        real t60
        real t600
        real t6001
        real t6003
        real t6005
        real t6007
        real t6010
        real t6016
        real t6017
        real t602
        real t6025
        real t6027
        real t604
        real t6040
        real t6046
        real t6047
        real t6048
        real t6057
        real t6058
        real t606
        real t6061
        real t6062
        real t6063
        real t6066
        real t6067
        real t6068
        real t6070
        real t6075
        real t6076
        real t6077
        real t6079
        real t608
        real t6082
        real t6083
        real t6089
        real t609
        real t6094
        real t6097
        real t610
        real t6101
        real t6106
        real t6109
        real t611
        real t6110
        real t6111
        real t6113
        real t6115
        real t6117
        real t6119
        real t6121
        real t6123
        real t6126
        real t613
        real t6132
        real t6133
        real t6141
        real t6143
        real t6149
        real t615
        real t6150
        real t6151
        real t6164
        real t6168
        real t617
        real t6174
        real t6175
        real t6177
        real t6179
        real t6181
        real t6183
        real t6185
        real t6187
        real t619
        real t6190
        real t6196
        real t6197
        real t62
        real t6205
        real t6207
        real t621
        real t6214
        real t6220
        real t6226
        real t6227
        real t6228
        real t623
        real t6230
        real t6236
        real t6237
        real t6238
        real t6242
        real t6245
        real t6251
        real t6252
        real t6257
        real t6258
        real t626
        real t6262
        real t6264
        real t6268
        real t6274
        real t6277
        real t6281
        real t629
        real t6290
        real t63
        real t6300
        real t6302
        real t6305
        real t631
        real t6313
        real t6315
        real t632
        real t633
        real t6330
        real t6333
        real t6337
        real t6341
        real t6345
        real t6348
        real t6351
        real t6352
        real t6357
        real t6363
        real t6366
        real t637
        real t6379
        real t6383
        real t639
        real t6392
        real t6396
        integer t64
        real t6402
        real t6406
        real t6409
        real t641
        real t6413
        real t6417
        real t6419
        real t6421
        real t6423
        real t6425
        real t643
        real t6430
        real t6431
        real t6435
        real t6440
        real t6441
        real t6442
        real t6443
        real t6445
        real t645
        real t6452
        real t6453
        real t6454
        real t6457
        integer t6458
        real t6459
        real t6460
        real t6462
        real t6464
        real t6466
        real t6468
        real t647
        real t6470
        real t6472
        real t6475
        real t6480
        real t6481
        real t6482
        real t6483
        real t6484
        real t6485
        real t6486
        real t6489
        real t649
        real t6493
        real t6494
        real t6495
        real t6497
        real t6499
        real t65
        real t650
        real t6501
        real t6503
        real t6505
        real t6507
        real t651
        real t6510
        real t6512
        real t6516
        real t6517
        real t6518
        real t652
        real t6520
        real t6521
        real t6523
        real t6525
        real t6526
        real t6527
        real t6529
        real t6531
        real t6533
        real t6538
        real t6539
        real t654
        real t6541
        real t6542
        real t6543
        real t6544
        real t6546
        real t6547
        real t6548
        real t6549
        real t6552
        real t6553
        real t6554
        real t6555
        real t6559
        real t656
        real t6563
        real t6566
        real t6570
        real t6571
        real t6573
        real t6575
        real t6578
        real t658
        real t6581
        real t6585
        real t6587
        real t6594
        real t6597
        real t66
        real t660
        real t6601
        real t6604
        real t6606
        real t6609
        real t6612
        real t6616
        real t6618
        real t662
        real t6623
        real t6625
        real t6628
        real t6632
        real t6634
        real t6637
        real t664
        real t6641
        real t6643
        real t6644
        real t6646
        real t6649
        real t6653
        real t6655
        real t6662
        real t6665
        real t6669
        real t667
        real t6671
        real t6677
        real t6679
        real t6680
        real t6681
        real t6682
        real t6684
        real t6685
        real t6686
        real t6687
        real t669
        real t6690
        real t6691
        real t6692
        real t6693
        real t6694
        real t6699
        real t6701
        real t6702
        real t6703
        real t6704
        real t6709
        real t6710
        real t6712
        real t6713
        real t6714
        real t672
        real t6721
        real t6723
        real t6724
        real t6725
        real t6727
        real t6729
        real t673
        real t6732
        real t6735
        real t6738
        real t674
        real t6742
        real t6745
        real t6747
        real t6749
        real t6752
        real t6756
        real t6758
        real t6766
        real t6768
        real t6770
        real t6772
        real t6774
        real t6776
        real t6778
        real t678
        real t6780
        real t6786
        real t6788
        real t6790
        real t6792
        real t6794
        real t6799
        real t68
        real t680
        real t6800
        real t6804
        real t6809
        real t6810
        real t6811
        real t6812
        real t6814
        real t682
        real t6821
        real t6822
        real t6823
        real t6828
        real t6831
        real t6835
        real t6837
        real t684
        real t6842
        real t6845
        real t6846
        real t6847
        real t6848
        real t6850
        real t6852
        real t6853
        real t6854
        real t6857
        real t6858
        real t686
        real t6861
        real t6863
        real t6865
        real t6866
        real t6867
        real t6868
        real t6869
        real t687
        real t6870
        real t6871
        real t6872
        real t6873
        real t6876
        real t6878
        real t6879
        real t688
        real t6880
        real t6881
        real t6882
        real t6883
        real t6884
        real t6885
        real t689
        real t6890
        real t6891
        real t6893
        real t6894
        real t6896
        real t6898
        real t690
        real t6900
        real t6902
        real t6904
        real t6906
        real t6908
        real t6911
        real t6913
        real t6915
        real t6916
        real t692
        real t6920
        real t6923
        real t6927
        real t6929
        real t693
        real t6930
        real t6932
        real t6935
        real t6939
        real t694
        real t6941
        real t6946
        real t6948
        real t695
        real t6951
        real t6952
        real t6954
        real t6957
        real t6961
        real t6964
        real t6966
        real t6967
        real t6969
        real t697
        real t6972
        real t6973
        real t6975
        real t6978
        real t6982
        real t6984
        real t6985
        real t6989
        real t6991
        real t6994
        real t6995
        real t6997
        real t6998
        real t7
        real t70
        real t700
        real t7000
        real t7004
        real t7007
        real t7009
        real t701
        real t7010
        real t7012
        real t7015
        real t7016
        real t7018
        real t702
        real t7021
        real t7025
        real t7027
        real t703
        real t7031
        real t7032
        real t7034
        real t7037
        real t7038
        real t704
        real t7040
        real t7043
        real t7047
        real t7049
        real t7052
        real t7054
        real t7057
        real t7059
        real t706
        real t7061
        real t7063
        real t7065
        real t7067
        real t7069
        real t7071
        real t7072
        real t7074
        real t7076
        real t7078
        real t708
        real t7080
        real t7082
        real t7084
        real t7087
        real t7089
        real t709
        real t7091
        real t7093
        real t7094
        real t7095
        real t7096
        real t7098
        real t7099
        real t710
        real t7100
        real t7101
        real t7104
        real t7106
        real t7107
        real t7108
        real t7109
        real t7111
        real t7112
        real t7113
        real t712
        real t7120
        real t7122
        real t7124
        real t7126
        real t7128
        real t7130
        real t7131
        real t7132
        real t7134
        real t7136
        real t7138
        real t7140
        real t7142
        real t7144
        real t7146
        real t7147
        real t7148
        real t715
        real t7153
        real t7154
        real t7157
        real t7159
        real t716
        real t7160
        real t7162
        real t7163
        real t7164
        real t717
        real t7171
        real t7174
        real t7176
        real t7179
        real t7183
        real t7185
        real t719
        real t7191
        real t7193
        real t7195
        real t7197
        real t7199
        real t72
        real t720
        real t7201
        real t7203
        real t7205
        real t7207
        real t7209
        real t7211
        real t7213
        real t7215
        real t7217
        real t7218
        real t7219
        real t722
        real t7225
        real t7228
        real t7230
        real t7232
        real t7235
        real t7237
        real t7239
        real t724
        real t7242
        real t7246
        real t7248
        real t7254
        real t7256
        real t7258
        real t726
        real t7260
        real t7261
        real t7262
        real t7264
        real t7266
        real t7268
        real t7270
        real t7272
        real t7274
        real t7276
        real t7278
        real t7282
        real t7283
        real t7284
        real t7288
        real t7290
        real t7292
        real t7294
        real t7296
        real t7298
        real t730
        real t7300
        real t7305
        real t7308
        real t731
        real t7310
        real t7311
        real t7312
        real t7314
        real t7316
        real t7317
        real t7318
        real t732
        real t7320
        real t7322
        real t7323
        real t7325
        real t7327
        real t7329
        real t7330
        real t7331
        real t7332
        real t7333
        real t7335
        real t7336
        real t7337
        real t7338
        real t734
        real t7340
        real t7343
        real t7344
        real t7345
        real t7346
        real t7347
        real t7349
        real t735
        real t7352
        real t7353
        real t7355
        real t7361
        real t7364
        real t7367
        real t7369
        real t7370
        real t7376
        real t7378
        real t7379
        real t7380
        real t7381
        real t7383
        real t7385
        real t7387
        real t7388
        real t739
        real t7390
        real t7392
        real t7394
        real t7395
        real t7397
        real t74
        real t740
        real t7401
        real t7403
        real t7405
        real t7406
        real t7412
        real t7414
        real t7415
        real t7416
        real t7417
        real t7418
        real t742
        real t7420
        real t7421
        real t7422
        real t7423
        real t7425
        real t7428
        real t7429
        real t743
        real t7430
        real t7431
        real t7432
        real t7434
        real t7437
        real t7438
        real t7440
        real t7441
        real t7442
        real t7443
        real t7444
        real t7446
        real t7447
        real t7449
        real t745
        real t7455
        real t7456
        real t7459
        real t7460
        real t7462
        real t7464
        real t7466
        real t7468
        real t747
        real t7470
        real t7472
        real t7475
        real t7479
        real t7480
        real t7481
        real t7482
        real t7483
        real t7484
        real t7486
        real t7489
        real t749
        real t7490
        real t7492
        real t7493
        real t7496
        real t7498
        real t7500
        real t7502
        real t7504
        real t7506
        real t7507
        real t7508
        real t751
        real t7512
        real t7514
        real t7515
        real t7517
        real t7519
        real t752
        real t7520
        real t7521
        real t7523
        real t7524
        real t7525
        real t7526
        real t7528
        real t753
        real t7530
        real t7532
        real t7534
        real t7536
        real t7538
        real t754
        real t7541
        real t7546
        real t7547
        real t7548
        real t7554
        real t7556
        real t7558
        real t756
        real t7560
        real t7561
        real t7562
        real t7563
        real t7564
        real t7566
        real t7569
        real t7570
        real t7572
        real t7577
        real t7579
        real t758
        real t7580
        real t7582
        real t7584
        real t7586
        real t7588
        real t7589
        real t7590
        real t7591
        real t7593
        real t7595
        real t7596
        real t7597
        real t7599
        real t76
        real t760
        real t7601
        real t7603
        real t7606
        real t7609
        real t7611
        real t7612
        real t7613
        real t7619
        real t762
        real t7621
        real t7623
        real t7625
        real t7628
        real t7629
        real t7630
        real t7632
        real t7634
        real t7636
        real t7638
        real t764
        real t7640
        real t7642
        real t7645
        real t7650
        real t7651
        real t7652
        real t7658
        real t766
        real t7660
        real t7662
        real t7665
        real t7666
        real t7671
        real t7673
        real t7675
        real t7677
        real t7678
        real t7680
        real t7685
        real t7686
        real t7688
        real t769
        real t7690
        real t7693
        real t7694
        real t7695
        real t7696
        real t7698
        real t7699
        real t7700
        real t7701
        real t7702
        real t7703
        real t7706
        real t7707
        real t7708
        real t7709
        real t771
        real t7710
        real t7712
        real t7715
        real t7716
        real t7719
        real t7720
        real t7722
        real t7723
        real t7724
        real t7726
        real t7728
        real t7730
        real t7732
        real t7734
        real t7736
        real t7739
        real t774
        real t7744
        real t7745
        real t7746
        real t7747
        real t7748
        real t775
        real t7750
        real t7753
        real t7754
        real t7756
        real t7757
        real t776
        real t7762
        real t7764
        real t7766
        real t7768
        real t7770
        real t7771
        real t7776
        real t7777
        real t7778
        real t7779
        real t7781
        real t7783
        real t7785
        real t7787
        real t7788
        real t7789
        real t7790
        real t7792
        real t7794
        real t7796
        real t7797
        real t7798
        real t78
        real t780
        real t7800
        real t7802
        real t7805
        real t7810
        real t7811
        real t7812
        real t7818
        real t782
        real t7820
        real t7822
        real t7824
        real t7825
        real t7826
        real t7827
        real t7828
        real t7830
        real t7833
        real t7834
        real t7836
        real t784
        real t7841
        real t7843
        real t7844
        real t7846
        real t7848
        real t7850
        real t7852
        real t7853
        real t7854
        real t7855
        real t7857
        real t7859
        real t786
        real t7861
        real t7863
        real t7865
        real t7867
        real t7870
        real t7875
        real t7876
        real t7877
        real t788
        real t7883
        real t7884
        real t7885
        real t7887
        real t7889
        real t7892
        real t7893
        real t7894
        real t7896
        real t7898
        real t790
        real t7900
        real t7902
        real t7904
        real t7906
        real t7907
        real t7909
        real t791
        real t7914
        real t7915
        real t7916
        real t792
        real t7922
        real t7924
        real t7926
        real t7929
        real t793
        real t7935
        real t7937
        real t7938
        real t7939
        real t7941
        real t7944
        real t7949
        real t795
        real t7950
        real t7952
        real t7954
        real t7957
        real t7958
        real t7959
        real t7960
        real t7962
        real t7963
        real t7964
        real t7965
        real t7967
        real t797
        real t7970
        real t7971
        real t7972
        real t7973
        real t7974
        real t7976
        real t7979
        real t7980
        real t7983
        real t7984
        real t7986
        real t7988
        real t799
        real t7990
        real t7993
        real t7994
        real t7995
        real t7997
        real t7999
        real t8001
        real t8003
        real t8005
        real t8007
        real t801
        real t8010
        real t8015
        real t8016
        real t8017
        real t8018
        real t8019
        real t8021
        real t8023
        real t8024
        real t8025
        real t8027
        real t8028
        real t803
        real t8034
        real t8036
        real t8038
        real t8040
        real t8042
        real t8043
        real t8045
        real t8048
        real t805
        real t8050
        real t8052
        real t8054
        real t8056
        real t8057
        real t8063
        real t8065
        real t8068
        real t8074
        real t8077
        real t8078
        real t8079
        real t808
        real t8080
        real t8081
        real t8082
        real t8083
        real t8084
        real t8085
        real t8087
        real t809
        real t8090
        real t8091
        real t8092
        real t8093
        real t8094
        real t8096
        real t8099
        real t81
        real t8100
        real t8101
        real t8103
        real t8105
        real t8107
        real t8109
        real t8111
        real t8114
        real t8115
        real t8117
        real t8119
        real t8121
        real t8124
        real t8125
        real t8126
        real t8128
        real t813
        real t8130
        real t8132
        real t8134
        real t8136
        real t8138
        real t814
        real t8141
        real t8146
        real t8147
        real t8148
        real t815
        real t8154
        real t8156
        real t8158
        real t8160
        real t8161
        real t8167
        real t8168
        real t8169
        real t8171
        real t8173
        real t8175
        real t8176
        real t8177
        real t8178
        real t8179
        real t8181
        real t8184
        real t8185
        real t8187
        real t8188
        real t8189
        real t819
        real t8191
        real t8192
        real t8193
        real t8195
        real t8197
        real t8198
        real t8199
        real t8201
        real t8203
        real t8205
        real t8208
        real t821
        real t8213
        real t8214
        real t8215
        real t8216
        real t8217
        real t8219
        real t822
        real t8222
        real t8223
        real t8225
        real t8226
        real t823
        real t8232
        real t8234
        real t8235
        real t8236
        real t8238
        real t8240
        real t8241
        real t8246
        real t8248
        real t8249
        real t825
        real t8250
        real t8252
        real t8254
        real t8255
        real t8261
        real t8263
        real t8266
        real t827
        real t8272
        real t8275
        real t8276
        real t8277
        real t8278
        real t828
        real t8280
        real t8281
        real t8282
        real t8283
        real t8285
        real t8288
        real t8289
        real t8290
        real t8291
        real t8292
        real t8294
        real t8297
        real t8298
        real t8301
        real t8303
        real t8305
        real t8307
        real t8309
        real t8312
        real t8313
        real t8315
        real t8317
        real t8318
        real t8319
        real t832
        real t8322
        real t8323
        real t8324
        real t8326
        real t8328
        real t8330
        real t8332
        real t8334
        real t8336
        real t8339
        real t834
        real t8340
        real t8344
        real t8345
        real t8346
        real t8352
        real t8354
        real t8356
        real t8358
        real t8359
        real t836
        real t8365
        real t8367
        real t8369
        real t837
        real t8371
        real t8373
        real t8374
        real t8375
        real t8376
        real t8377
        real t8378
        real t8379
        real t838
        real t8382
        real t8383
        real t8385
        real t8386
        real t8387
        real t8389
        real t8391
        real t8393
        real t8394
        real t8396
        real t8398
        real t840
        real t8400
        real t8402
        real t8404
        real t8407
        real t8409
        real t8411
        real t8413
        real t8416
        real t8417
        real t8418
        real t842
        real t8421
        real t8422
        real t8423
        real t8425
        real t8428
        real t8429
        real t8433
        real t8436
        real t8438
        real t844
        real t8441
        real t8442
        real t8443
        real t8445
        real t8447
        real t8449
        real t845
        real t8451
        real t8453
        real t8455
        real t8458
        real t8463
        real t8464
        real t8465
        real t8471
        real t8473
        real t8475
        real t8477
        real t8478
        real t8479
        real t8480
        real t8481
        real t8483
        real t8486
        real t8487
        real t8489
        real t849
        real t8494
        real t8496
        real t8498
        real t8500
        real t8502
        real t8503
        real t8504
        real t8505
        real t8507
        real t8509
        real t851
        real t8511
        real t8513
        real t8515
        real t8517
        real t8520
        real t8525
        real t8526
        real t8527
        real t853
        real t8533
        real t8535
        real t8537
        real t8539
        real t8540
        real t8546
        real t8548
        real t855
        real t8550
        real t8552
        real t8553
        real t8554
        real t8555
        real t8556
        real t8558
        real t8561
        real t8562
        real t8564
        real t8565
        real t8566
        real t8568
        real t8569
        real t857
        real t8570
        real t8571
        real t8573
        real t8576
        real t8577
        real t8581
        real t8584
        real t8586
        real t8589
        real t859
        real t8590
        real t8591
        real t8593
        real t8595
        real t8597
        real t8599
        real t86
        real t860
        real t8601
        real t8603
        real t8606
        real t861
        real t8611
        real t8612
        real t8613
        real t8619
        real t862
        real t8621
        real t8623
        real t8625
        real t8626
        real t8627
        real t8628
        real t8629
        real t863
        real t8631
        real t8634
        real t8635
        real t8637
        real t8642
        real t8644
        real t8646
        real t8648
        real t865
        real t8650
        real t8651
        real t8652
        real t8653
        real t8655
        real t8657
        real t8659
        real t866
        real t8661
        real t8663
        real t8665
        real t8668
        real t867
        real t8673
        real t8674
        real t8675
        real t868
        real t8681
        real t8683
        real t8685
        real t8687
        real t8688
        real t8694
        real t8696
        real t8698
        real t87
        real t870
        real t8700
        real t8701
        real t8702
        real t8703
        real t8704
        real t8706
        real t8709
        real t8710
        real t8712
        real t8713
        real t8714
        real t8716
        real t8718
        real t8720
        real t8722
        real t8725
        real t8726
        real t8727
        real t8728
        real t873
        real t8730
        real t8733
        real t8734
        real t8738
        real t874
        real t8741
        real t8743
        real t8746
        real t8747
        real t8748
        real t875
        real t8750
        real t8752
        real t8754
        real t8756
        real t8758
        real t876
        real t8760
        real t8763
        real t8768
        real t8769
        real t877
        real t8770
        real t8776
        real t8778
        real t8780
        real t8782
        real t8783
        real t8784
        real t8785
        real t8786
        real t8788
        real t879
        real t8791
        real t8792
        real t8794
        real t8799
        real t88
        real t8801
        real t8803
        real t8805
        real t8807
        real t8808
        real t8809
        real t8810
        real t8812
        real t8814
        real t8816
        real t8818
        real t882
        real t8820
        real t8822
        real t8825
        real t883
        real t8830
        real t8831
        real t8832
        real t8838
        real t8840
        real t8842
        real t8844
        real t8845
        real t885
        real t8851
        real t8853
        real t8855
        real t8857
        real t8858
        real t8859
        real t886
        real t8860
        real t8861
        real t8863
        real t8866
        real t8867
        real t8869
        real t887
        real t8870
        real t8871
        real t8873
        real t8874
        real t8875
        real t8876
        real t8878
        real t888
        real t8881
        real t8882
        real t8886
        real t8889
        real t8891
        real t8894
        real t8895
        real t8896
        real t8898
        real t89
        real t890
        real t8900
        real t8902
        real t8904
        real t8906
        real t8908
        real t8911
        real t8916
        real t8917
        real t8918
        real t8924
        real t8926
        real t8928
        real t893
        real t8930
        real t8931
        real t8932
        real t8933
        real t8934
        real t8936
        real t8939
        real t8940
        real t8942
        real t8947
        real t8949
        real t895
        real t8951
        real t8953
        real t8955
        real t8956
        real t8957
        real t8958
        real t896
        real t8960
        real t8962
        real t8964
        real t8966
        real t8968
        real t897
        real t8970
        real t8973
        real t8978
        real t8979
        real t8980
        real t8986
        real t8988
        real t899
        real t8990
        real t8992
        real t8993
        real t8999
        real t9
        real t90
        real t900
        real t9001
        real t9003
        real t9005
        real t9006
        real t9007
        real t9008
        real t9009
        real t9011
        real t9014
        real t9015
        real t9017
        real t9018
        real t9019
        real t902
        real t9021
        real t9023
        real t9025
        real t9028
        real t903
        real t9030
        real t9032
        real t9034
        real t9036
        real t9039
        real t9041
        real t9043
        real t9045
        real t9048
        real t905
        real t9050
        real t9052
        real t9054
        real t9056
        real t9058
        real t9061
        real t9063
        real t9065
        real t9067
        real t9069
        real t907
        real t9072
        real t9073
        real t9074
        real t9077
        real t9079
        real t9081
        real t9082
        real t9083
        real t9085
        real t9087
        real t9089
        real t909
        real t9091
        real t9093
        real t9094
        real t9096
        real t9098
        real t910
        real t9100
        real t9101
        real t9102
        real t9103
        real t9105
        real t9106
        real t9108
        real t9109
        real t9111
        real t9113
        real t9115
        real t9117
        real t9119
        real t912
        real t9120
        real t9121
        real t9123
        real t9124
        real t9126
        real t9128
        real t913
        real t9130
        real t9132
        real t9133
        real t9135
        real t9137
        real t9139
        real t9141
        real t9142
        real t9144
        real t9146
        real t9148
        real t9149
        real t915
        real t9151
        real t9153
        real t9155
        real t9157
        real t9159
        real t9161
        real t9162
        real t9164
        real t9166
        real t9168
        real t917
        real t9170
        real t9172
        real t9173
        real t9174
        real t9175
        real t9177
        real t9178
        real t9179
        real t9180
        real t9181
        real t9184
        real t9186
        real t9188
        real t919
        real t9190
        real t9191
        real t9195
        real t9196
        real t9197
        real t92
        real t9207
        real t9208
        real t921
        real t9212
        real t9213
        real t9215
        real t9216
        real t9219
        real t922
        real t9220
        real t9223
        real t9225
        real t9226
        real t9228
        real t923
        real t9233
        real t9240
        real t9242
        real t9244
        real t9246
        real t9248
        real t925
        real t9250
        real t9251
        real t9254
        real t9256
        real t926
        real t9262
        real t9264
        real t9269
        real t9271
        real t9272
        real t9276
        real t928
        real t9284
        real t9289
        real t9291
        real t9292
        real t9295
        real t9296
        real t930
        real t9302
        real t9313
        real t9314
        real t9315
        real t9318
        real t9319
        real t932
        real t9320
        real t9321
        real t9322
        real t9326
        real t9328
        real t9332
        real t9335
        real t9336
        real t934
        real t9343
        real t9348
        real t9349
        real t935
        real t9351
        real t9355
        real t9357
        real t9359
        real t936
        real t9365
        real t9367
        real t9369
        real t9372
        real t9374
        real t9376
        real t9377
        real t938
        real t9381
        real t9383
        real t9386
        real t9388
        real t9389
        real t939
        real t9393
        real t9395
        real t9397
        real t94
        real t9401
        real t9406
        real t9408
        real t941
        real t9410
        real t9417
        real t9421
        real t9426
        real t943
        real t9436
        real t9437
        real t9441
        real t9443
        real t9449
        real t945
        real t9451
        real t9453
        real t9456
        real t9458
        real t946
        real t9460
        real t9461
        real t9465
        real t9467
        real t9470
        real t9472
        real t9473
        real t9477
        real t9479
        real t948
        real t9481
        real t9485
        real t949
        real t9490
        real t9492
        real t9494
        real t9501
        real t9505
        real t951
        real t9510
        real t9520
        real t9521
        real t9524
        real t9526
        real t9528
        real t953
        real t9530
        real t9532
        real t9533
        real t9535
        real t9537
        real t9538
        real t9539
        real t9540
        real t9542
        real t9544
        real t9546
        real t9548
        real t955
        real t9550
        real t9551
        real t9553
        real t9555
        real t9557
        real t9559
        real t9560
        real t9562
        real t9564
        real t9566
        real t9568
        real t9569
        real t957
        real t9571
        real t9573
        real t9575
        real t9576
        real t9577
        real t9578
        real t958
        real t9580
        real t9581
        real t9582
        real t9583
        real t9584
        real t9585
        real t9587
        real t9589
        real t959
        real t9591
        real t9593
        real t9594
        real t9596
        real t9598
        real t9599
        integer t96
        real t9600
        real t9601
        real t9603
        real t9605
        real t9607
        real t9609
        real t961
        real t9611
        real t9612
        real t9614
        real t9616
        real t9618
        real t962
        real t9620
        real t9621
        real t9623
        real t9625
        real t9627
        real t9629
        real t9630
        real t9632
        real t9634
        real t9636
        real t9637
        real t9638
        real t9639
        real t964
        real t9641
        real t9642
        real t9643
        real t9644
        real t9645
        real t9647
        real t9653
        real t9657
        real t9658
        real t966
        real t9661
        real t9662
        real t9665
        real t9667
        real t9669
        real t9676
        real t9678
        real t9679
        real t968
        real t9682
        real t9683
        real t9689
        real t97
        real t970
        real t9700
        real t9701
        real t9702
        real t9705
        real t9706
        real t9707
        real t9708
        real t9709
        real t971
        real t9713
        real t9715
        real t9719
        real t9722
        real t9723
        real t973
        real t9731
        real t9735
        real t9740
        real t9742
        real t9743
        real t9746
        real t9747
        real t975
        real t9754
        real t9755
        real t9760
        real t9762
        real t9763
        real t9766
        real t9767
        real t977
        real t9770
        real t9773
        real t9779
        real t9783
        real t9784
        real t9785
        real t9786
        real t9789
        real t979
        real t9790
        real t9791
        real t9792
        real t9793
        real t9797
        real t9799
        real t98
        real t9803
        real t9806
        real t9807
        real t981
        real t9814
        real t9819
        real t9821
        real t9826
        real t9828
        real t983
        real t9832
        real t9834
        real t9837
        real t9839
        real t9840
        real t9846
        real t9848
        real t985
        real t9850
        real t9853
        real t9855
        real t9857
        real t9858
        real t986
        real t9862
        real t9876
        real t988
        real t9880
        real t9882
        real t9885
        real t9887
        real t9893
        real t9894
        real t9898
        real t990
        real t9900
        real t9904
        real t9906
        real t9909
        real t9911
        real t9912
        real t9918
        real t992
        real t9920
        real t9922
        real t9925
        real t9927
        real t9929
        real t9930
        real t9934
        real t994
        real t9948
        real t9952
        real t9957
        real t996
        real t9965
        real t9966
        real t9969
        real t997
        real t9971
        real t9973
        real t9975
        real t9976
        real t9978
        real t998
        real t9980
        real t9982
        real t9983
        real t9985
        real t9987
        real t9989
        real t999
        real t9990
        real t9992
        real t9994
        real t9995
        real t9996
        real t9997
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
        t1227 = beta * t135
        t1228 = dz ** 2
        t1229 = k + 2
        t1230 = rx(t5,j,t1229,0,0)
        t1231 = rx(t5,j,t1229,1,1)
        t1233 = rx(t5,j,t1229,2,2)
        t1235 = rx(t5,j,t1229,1,2)
        t1237 = rx(t5,j,t1229,2,1)
        t1239 = rx(t5,j,t1229,1,0)
        t1241 = rx(t5,j,t1229,0,2)
        t1243 = rx(t5,j,t1229,0,1)
        t1246 = rx(t5,j,t1229,2,0)
        t1251 = t1230 * t1231 * t1233 - t1230 * t1235 * t1237 + t1239 * 
     #t1237 * t1241 - t1239 * t1243 * t1233 + t1246 * t1243 * t1235 - t1
     #246 * t1231 * t1241
        t1252 = 0.1E1 / t1251
        t1253 = t4 * t1252
        t1258 = u(t64,j,t1229,n)
        t1259 = u(t5,j,t1229,n)
        t1261 = (t1258 - t1259) * t94
        t1262 = u(i,j,t1229,n)
        t1264 = (t1259 - t1262) * t94
        t1266 = t1261 / 0.2E1 + t1264 / 0.2E1
        t1155 = t1253 * (t1230 * t1246 + t1243 * t1237 + t1241 * t1233)
        t1268 = t1155 * t1266
        t1270 = (t1268 - t462) * t236
        t1274 = (t466 - t505) * t236
        t1277 = k - 2
        t1278 = rx(t5,j,t1277,0,0)
        t1279 = rx(t5,j,t1277,1,1)
        t1281 = rx(t5,j,t1277,2,2)
        t1283 = rx(t5,j,t1277,1,2)
        t1285 = rx(t5,j,t1277,2,1)
        t1287 = rx(t5,j,t1277,1,0)
        t1289 = rx(t5,j,t1277,0,2)
        t1291 = rx(t5,j,t1277,0,1)
        t1294 = rx(t5,j,t1277,2,0)
        t1299 = t1278 * t1279 * t1281 - t1278 * t1283 * t1285 + t1287 * 
     #t1285 * t1289 - t1287 * t1291 * t1281 + t1294 * t1291 * t1283 - t1
     #294 * t1279 * t1289
        t1300 = 0.1E1 / t1299
        t1301 = t4 * t1300
        t1306 = u(t64,j,t1277,n)
        t1307 = u(t5,j,t1277,n)
        t1309 = (t1306 - t1307) * t94
        t1310 = u(i,j,t1277,n)
        t1312 = (t1307 - t1310) * t94
        t1314 = t1309 / 0.2E1 + t1312 / 0.2E1
        t1202 = t1301 * (t1278 * t1294 + t1291 * t1285 + t1289 * t1281)
        t1316 = t1202 * t1314
        t1318 = (t503 - t1316) * t236
        t1327 = dy ** 2
        t1328 = j + 2
        t1329 = u(t5,t1328,t233,n)
        t1331 = (t1329 - t391) * t183
        t1335 = j - 2
        t1336 = u(t5,t1335,t233,n)
        t1338 = (t414 - t1336) * t183
        t1346 = u(t5,t1328,k,n)
        t1348 = (t1346 - t198) * t183
        t1351 = (t1348 / 0.2E1 - t203 / 0.2E1) * t183
        t1352 = u(t5,t1335,k,n)
        t1354 = (t201 - t1352) * t183
        t1357 = (t200 / 0.2E1 - t1354 / 0.2E1) * t183
        t1244 = (t1351 - t1357) * t183
        t1361 = t397 * t1244
        t1364 = u(t5,t1328,t238,n)
        t1366 = (t1364 - t394) * t183
        t1370 = u(t5,t1335,t238,n)
        t1372 = (t417 - t1370) * t183
        t1386 = u(t64,t1328,k,n)
        t1388 = (t1386 - t181) * t183
        t1392 = u(t64,t1335,k,n)
        t1394 = (t186 - t1392) * t183
        t1404 = t196 * t1244
        t1407 = u(i,t1328,k,n)
        t1409 = (t1407 - t216) * t183
        t1412 = (t1409 / 0.2E1 - t221 / 0.2E1) * t183
        t1413 = u(i,t1335,k,n)
        t1415 = (t219 - t1413) * t183
        t1418 = (t218 / 0.2E1 - t1415 / 0.2E1) * t183
        t1275 = (t1412 - t1418) * t183
        t1422 = t214 * t1275
        t1424 = (t1404 - t1422) * t94
        t1429 = rx(t5,t1328,k,0,0)
        t1430 = rx(t5,t1328,k,1,1)
        t1432 = rx(t5,t1328,k,2,2)
        t1434 = rx(t5,t1328,k,1,2)
        t1436 = rx(t5,t1328,k,2,1)
        t1438 = rx(t5,t1328,k,1,0)
        t1440 = rx(t5,t1328,k,0,2)
        t1442 = rx(t5,t1328,k,0,1)
        t1445 = rx(t5,t1328,k,2,0)
        t1450 = t1429 * t1430 * t1432 - t1429 * t1434 * t1436 + t1438 * 
     #t1436 * t1440 - t1438 * t1442 * t1432 + t1445 * t1442 * t1434 - t1
     #445 * t1430 * t1440
        t1451 = 0.1E1 / t1450
        t1452 = t4 * t1451
        t1458 = (t1329 - t1346) * t236
        t1460 = (t1346 - t1364) * t236
        t1462 = t1458 / 0.2E1 + t1460 / 0.2E1
        t1315 = t1452 * (t1438 * t1445 + t1430 * t1436 + t1434 * t1432)
        t1464 = t1315 * t1462
        t1466 = (t1464 - t400) * t183
        t1470 = (t408 - t425) * t183
        t1473 = rx(t5,t1335,k,0,0)
        t1474 = rx(t5,t1335,k,1,1)
        t1476 = rx(t5,t1335,k,2,2)
        t1478 = rx(t5,t1335,k,1,2)
        t1480 = rx(t5,t1335,k,2,1)
        t1482 = rx(t5,t1335,k,1,0)
        t1484 = rx(t5,t1335,k,0,2)
        t1486 = rx(t5,t1335,k,0,1)
        t1489 = rx(t5,t1335,k,2,0)
        t1494 = t1473 * t1474 * t1476 - t1473 * t1478 * t1480 + t1482 * 
     #t1480 * t1484 - t1482 * t1486 * t1476 + t1489 * t1486 * t1478 - t1
     #489 * t1474 * t1484
        t1495 = 0.1E1 / t1494
        t1496 = t4 * t1495
        t1502 = (t1336 - t1352) * t236
        t1504 = (t1352 - t1370) * t236
        t1506 = t1502 / 0.2E1 + t1504 / 0.2E1
        t1347 = t1496 * (t1482 * t1489 + t1474 * t1480 + t1478 * t1476)
        t1508 = t1347 * t1506
        t1510 = (t423 - t1508) * t183
        t1522 = (t200 - t203) * t183
        t1524 = ((t1348 - t200) * t183 - t1522) * t183
        t1529 = (t1522 - (t203 - t1354) * t183) * t183
        t1533 = t1438 ** 2
        t1534 = t1430 ** 2
        t1535 = t1434 ** 2
        t1537 = t1451 * (t1533 + t1534 + t1535)
        t1540 = t4 * (t1537 / 0.2E1 + t366 / 0.2E1)
        t1541 = t1540 * t1348
        t1543 = (t1541 - t375) * t183
        t1546 = t1482 ** 2
        t1547 = t1474 ** 2
        t1548 = t1478 ** 2
        t1550 = t1495 * (t1546 + t1547 + t1548)
        t1553 = t4 * (t380 / 0.2E1 + t1550 / 0.2E1)
        t1554 = t1553 * t1354
        t1556 = (t384 - t1554) * t183
        t1564 = u(t5,t180,t1229,n)
        t1566 = (t1564 - t391) * t236
        t1570 = u(t5,t180,t1277,n)
        t1572 = (t394 - t1570) * t236
        t1581 = (t1259 - t250) * t236
        t1584 = (t1581 / 0.2E1 - t255 / 0.2E1) * t236
        t1586 = (t253 - t1307) * t236
        t1589 = (t252 / 0.2E1 - t1586 / 0.2E1) * t236
        t1393 = (t1584 - t1589) * t236
        t1593 = t397 * t1393
        t1596 = u(t5,t185,t1229,n)
        t1598 = (t1596 - t414) * t236
        t1602 = u(t5,t185,t1277,n)
        t1604 = (t417 - t1602) * t236
        t1619 = i + 3
        t1620 = rx(t1619,j,k,0,0)
        t1621 = rx(t1619,j,k,1,1)
        t1623 = rx(t1619,j,k,2,2)
        t1625 = rx(t1619,j,k,1,2)
        t1627 = rx(t1619,j,k,2,1)
        t1629 = rx(t1619,j,k,1,0)
        t1631 = rx(t1619,j,k,0,2)
        t1633 = rx(t1619,j,k,0,1)
        t1636 = rx(t1619,j,k,2,0)
        t1642 = 0.1E1 / (t1620 * t1621 * t1623 - t1620 * t1625 * t1627 +
     # t1629 * t1627 * t1631 - t1629 * t1633 * t1623 + t1636 * t1633 * t
     #1625 - t1636 * t1621 * t1631)
        t1643 = t1620 ** 2
        t1644 = t1633 ** 2
        t1645 = t1631 ** 2
        t1647 = t1642 * (t1643 + t1644 + t1645)
        t1651 = (t33 - t62) * t94
        t1657 = t4 * (t92 / 0.2E1 + t34 - dx * ((t1647 - t92) * t94 / 0.
     #2E1 - t1651 / 0.2E1) / 0.8E1)
        t1659 = t132 * t171
        t1663 = (t1258 - t234) * t236
        t1668 = (t239 - t1306) * t236
        t1678 = t248 * t1393
        t1682 = (t1262 - t267) * t236
        t1685 = (t1682 / 0.2E1 - t272 / 0.2E1) * t236
        t1687 = (t270 - t1310) * t236
        t1690 = (t269 / 0.2E1 - t1687 / 0.2E1) * t236
        t1457 = (t1685 - t1690) * t236
        t1694 = t265 * t1457
        t1696 = (t1678 - t1694) * t94
        t1701 = dx ** 2
        t1702 = u(t1619,j,k,n)
        t1704 = (t1702 - t165) * t94
        t1708 = (t168 - t171) * t94
        t1713 = (t171 - t571) * t94
        t1714 = t1708 - t1713
        t1715 = t1714 * t94
        t1716 = t158 * t1715
        t1721 = t4 * (t1647 / 0.2E1 + t92 / 0.2E1)
        t1724 = (t1721 * t1704 - t169) * t94
        t1727 = t174 - t574
        t1728 = t1727 * t94
        t1493 = ((t1331 / 0.2E1 - t514 / 0.2E1) * t183 - (t512 / 0.2E1 -
     # t1338 / 0.2E1) * t183) * t183
        t1501 = ((t1366 / 0.2E1 - t531 / 0.2E1) * t183 - (t529 / 0.2E1 -
     # t1372 / 0.2E1) * t183) * t183
        t1575 = ((t1566 / 0.2E1 - t396 / 0.2E1) * t236 - (t393 / 0.2E1 -
     # t1572 / 0.2E1) * t236) * t236
        t1580 = ((t1598 / 0.2E1 - t419 / 0.2E1) * t236 - (t416 / 0.2E1 -
     # t1604 / 0.2E1) * t236) * t236
        t1734 = -t1228 * (((t1270 - t466) * t236 - t1274) * t236 / 0.2E1
     # + (t1274 - (t505 - t1318) * t236) * t236 / 0.2E1) / 0.6E1 - t1327
     # * ((t507 * t1493 - t1361) * t236 / 0.2E1 + (t1361 - t521 * t1501)
     # * t236 / 0.2E1) / 0.6E1 - t1327 * ((t178 * ((t1388 / 0.2E1 - t188
     # / 0.2E1) * t183 - (t184 / 0.2E1 - t1394 / 0.2E1) * t183) * t183 -
     # t1404) * t94 / 0.2E1 + t1424 / 0.2E1) / 0.6E1 - t1327 * (((t1466 
     #- t408) * t183 - t1470) * t183 / 0.2E1 + (t1470 - (t425 - t1510) *
     # t183) * t183 / 0.2E1) / 0.6E1 + t322 + t361 + t210 + t426 + t467 
     #+ t506 - t1327 * ((t374 * t1524 - t383 * t1529) * t183 + ((t1543 -
     # t386) * t183 - (t386 - t1556) * t183) * t183) / 0.24E2 - t1228 * 
     #((t388 * t1575 - t1593) * t183 / 0.2E1 + (t1593 - t411 * t1580) * 
     #t183 / 0.2E1) / 0.6E1 + (t1657 * t168 - t1659) * t94 - t1228 * ((t
     #231 * ((t1663 / 0.2E1 - t241 / 0.2E1) * t236 - (t237 / 0.2E1 - t16
     #68 / 0.2E1) * t236) * t236 - t1678) * t94 / 0.2E1 + t1696 / 0.2E1)
     # / 0.6E1 - t1701 * ((t164 * ((t1704 - t168) * t94 - t1708) * t94 -
     # t1716) * t94 + ((t1724 - t174) * t94 - t1728) * t94) / 0.24E2
        t1740 = (t1564 - t1259) * t183
        t1742 = (t1259 - t1596) * t183
        t1744 = t1740 / 0.2E1 + t1742 / 0.2E1
        t1653 = t1253 * (t1239 * t1246 + t1231 * t1237 + t1235 * t1233)
        t1746 = t1653 * t1744
        t1748 = (t1746 - t518) * t236
        t1752 = (t522 - t537) * t236
        t1760 = (t1570 - t1307) * t183
        t1762 = (t1307 - t1602) * t183
        t1764 = t1760 / 0.2E1 + t1762 / 0.2E1
        t1665 = t1301 * (t1287 * t1294 + t1279 * t1285 + t1283 * t1281)
        t1766 = t1665 * t1764
        t1768 = (t535 - t1766) * t236
        t1780 = (t252 - t255) * t236
        t1782 = ((t1581 - t252) * t236 - t1780) * t236
        t1787 = (t1780 - (t255 - t1586) * t236) * t236
        t1791 = t1246 ** 2
        t1792 = t1237 ** 2
        t1793 = t1233 ** 2
        t1795 = t1252 * (t1791 + t1792 + t1793)
        t1798 = t4 * (t1795 / 0.2E1 + t543 / 0.2E1)
        t1799 = t1798 * t1581
        t1801 = (t1799 - t552) * t236
        t1804 = t1294 ** 2
        t1805 = t1285 ** 2
        t1806 = t1281 ** 2
        t1808 = t1300 * (t1804 + t1805 + t1806)
        t1811 = t4 * (t557 / 0.2E1 + t1808 / 0.2E1)
        t1812 = t1811 * t1586
        t1814 = (t561 - t1812) * t236
        t1822 = t4 * t1642
        t1827 = u(t1619,t180,k,n)
        t1829 = (t1827 - t1702) * t183
        t1830 = u(t1619,t185,k,n)
        t1832 = (t1702 - t1830) * t183
        t1695 = t1822 * (t1620 * t1629 + t1633 * t1621 + t1631 * t1625)
        t1838 = (t1695 * (t1829 / 0.2E1 + t1832 / 0.2E1) - t192) * t94
        t1842 = (t209 - t227) * t94
        t1846 = (t227 - t591) * t94
        t1848 = (t1842 - t1846) * t94
        t1858 = (t1386 - t1346) * t94
        t1860 = (t1346 - t1407) * t94
        t1862 = t1858 / 0.2E1 + t1860 / 0.2E1
        t1718 = t1452 * (t1429 * t1438 + t1442 * t1430 + t1440 * t1434)
        t1864 = t1718 * t1862
        t1866 = (t1864 - t315) * t183
        t1870 = (t321 - t360) * t183
        t1878 = (t1392 - t1352) * t94
        t1880 = (t1352 - t1413) * t94
        t1882 = t1878 / 0.2E1 + t1880 / 0.2E1
        t1731 = t1496 * (t1473 * t1482 + t1486 * t1474 + t1484 * t1478)
        t1884 = t1731 * t1882
        t1886 = (t358 - t1884) * t183
        t1896 = t371 / 0.2E1
        t1906 = t4 * (t366 / 0.2E1 + t1896 - dy * ((t1537 - t366) * t183
     # / 0.2E1 - (t371 - t380) * t183 / 0.2E1) / 0.8E1)
        t1918 = t4 * (t1896 + t380 / 0.2E1 - dy * ((t366 - t371) * t183 
     #/ 0.2E1 - (t380 - t1550) * t183 / 0.2E1) / 0.8E1)
        t1922 = u(t1619,j,t233,n)
        t1924 = (t1922 - t234) * t94
        t1930 = (t456 / 0.2E1 - t782 / 0.2E1) * t94
        t1940 = (t168 / 0.2E1 - t571 / 0.2E1) * t94
        t1775 = ((t1704 / 0.2E1 - t171 / 0.2E1) * t94 - t1940) * t94
        t1944 = t248 * t1775
        t1947 = u(t1619,j,t238,n)
        t1949 = (t1947 - t239) * t94
        t1955 = (t497 / 0.2E1 - t821 / 0.2E1) * t94
        t1967 = (t1827 - t181) * t94
        t1973 = (t309 / 0.2E1 - t639 / 0.2E1) * t94
        t1980 = t196 * t1775
        t1984 = (t1830 - t186) * t94
        t1990 = (t352 / 0.2E1 - t680 / 0.2E1) * t94
        t2002 = t548 / 0.2E1
        t2012 = t4 * (t543 / 0.2E1 + t2002 - dz * ((t1795 - t543) * t236
     # / 0.2E1 - (t548 - t557) * t236 / 0.2E1) / 0.8E1)
        t2024 = t4 * (t2002 + t557 / 0.2E1 - dz * ((t543 - t548) * t236 
     #/ 0.2E1 - (t557 - t1808) * t236 / 0.2E1) / 0.8E1)
        t2033 = (t1922 - t1702) * t236
        t2035 = (t1702 - t1947) * t236
        t1833 = t1822 * (t1620 * t1636 + t1633 * t1627 + t1631 * t1623)
        t2041 = (t1833 * (t2033 / 0.2E1 + t2035 / 0.2E1) - t245) * t94
        t2045 = (t261 - t278) * t94
        t2049 = (t278 - t608) * t94
        t2051 = (t2045 - t2049) * t94
        t2056 = -t1228 * (((t1748 - t522) * t236 - t1752) * t236 / 0.2E1
     # + (t1752 - (t537 - t1768) * t236) * t236 / 0.2E1) / 0.6E1 - t1228
     # * ((t551 * t1782 - t560 * t1787) * t236 + ((t1801 - t563) * t236 
     #- (t563 - t1814) * t236) * t236) / 0.24E2 - t1701 * (((t1838 - t20
     #9) * t94 - t1842) * t94 / 0.2E1 + t1848 / 0.2E1) / 0.6E1 - t1327 *
     # (((t1866 - t321) * t183 - t1870) * t183 / 0.2E1 + (t1870 - (t360 
     #- t1886) * t183) * t183 / 0.2E1) / 0.6E1 + (t1906 * t200 - t1918 *
     # t203) * t183 - t1701 * ((t452 * ((t1924 / 0.2E1 - t458 / 0.2E1) *
     # t94 - t1930) * t94 - t1944) * t236 / 0.2E1 + (t1944 - t492 * ((t1
     #949 / 0.2E1 - t499 / 0.2E1) * t94 - t1955) * t94) * t236 / 0.2E1) 
     #/ 0.6E1 - t1701 * ((t306 * ((t1967 / 0.2E1 - t311 / 0.2E1) * t94 -
     # t1973) * t94 - t1980) * t183 / 0.2E1 + (t1980 - t348 * ((t1984 / 
     #0.2E1 - t354 / 0.2E1) * t94 - t1990) * t94) * t183 / 0.2E1) / 0.6E
     #1 + (t2012 * t252 - t2024 * t255) * t236 + t523 + t538 + t409 + t2
     #28 + t262 + t279 - t1701 * (((t2041 - t261) * t94 - t2045) * t94 /
     # 0.2E1 + t2051 / 0.2E1) / 0.6E1
        t2059 = dt * (t1734 + t2056) * t27
        t2062 = t139 / 0.2E1
        t2063 = ut(t1619,j,k,n)
        t2065 = (t2063 - t140) * t94
        t2069 = ((t2065 - t142) * t94 - t144) * t94
        t2070 = t150 * t94
        t2077 = dx * (t142 / 0.2E1 + t2062 - t1701 * (t2069 / 0.2E1 + t2
     #070 / 0.2E1) / 0.6E1) / 0.2E1
        t2078 = beta ** 2
        t2079 = t2078 * t159
        t2080 = ut(t5,t1328,t233,n)
        t2081 = ut(t5,t1328,k,n)
        t2084 = ut(t5,t1328,t238,n)
        t2088 = (t2080 - t2081) * t236 / 0.2E1 + (t2081 - t2084) * t236 
     #/ 0.2E1
        t2092 = (t1315 * t2088 - t1011) * t183
        t2096 = (t1015 - t1028) * t183
        t2099 = ut(t5,t1335,t233,n)
        t2100 = ut(t5,t1335,k,n)
        t2103 = ut(t5,t1335,t238,n)
        t2107 = (t2099 - t2100) * t236 / 0.2E1 + (t2100 - t2103) * t236 
     #/ 0.2E1
        t2111 = (t1026 - t1347 * t2107) * t183
        t2124 = ut(t64,t1328,k,n)
        t2126 = (t2124 - t2081) * t94
        t2127 = ut(i,t1328,k,n)
        t2129 = (t2081 - t2127) * t94
        t2135 = (t1718 * (t2126 / 0.2E1 + t2129 / 0.2E1) - t979) * t183
        t2139 = (t985 - t996) * t183
        t2142 = ut(t64,t1335,k,n)
        t2144 = (t2142 - t2100) * t94
        t2145 = ut(i,t1335,k,n)
        t2147 = (t2100 - t2145) * t94
        t2153 = (t994 - t1731 * (t2144 / 0.2E1 + t2147 / 0.2E1)) * t183
        t2162 = ut(t1619,j,t233,n)
        t2164 = (t2162 - t936) * t94
        t2170 = (t1031 / 0.2E1 - t1168 / 0.2E1) * t94
        t2180 = (t142 / 0.2E1 - t147 / 0.2E1) * t94
        t2028 = ((t2065 / 0.2E1 - t139 / 0.2E1) * t94 - t2180) * t94
        t2184 = t248 * t2028
        t2187 = ut(t1619,j,t238,n)
        t2189 = (t2187 - t939) * t94
        t2195 = (t1044 / 0.2E1 - t1179 / 0.2E1) * t94
        t2206 = ut(t64,j,t1229,n)
        t2207 = ut(t5,j,t1229,n)
        t2209 = (t2206 - t2207) * t94
        t2210 = ut(i,j,t1229,n)
        t2212 = (t2207 - t2210) * t94
        t2218 = (t1155 * (t2209 / 0.2E1 + t2212 / 0.2E1) - t1037) * t236
        t2222 = (t1041 - t1052) * t236
        t2225 = ut(t64,j,t1277,n)
        t2226 = ut(t5,j,t1277,n)
        t2228 = (t2225 - t2226) * t94
        t2229 = ut(i,j,t1277,n)
        t2231 = (t2226 - t2229) * t94
        t2237 = (t1050 - t1202 * (t2228 / 0.2E1 + t2231 / 0.2E1)) * t236
        t2247 = (t2080 - t1002) * t183
        t2252 = (t1017 - t2099) * t183
        t2261 = (t2081 - t910) * t183
        t2264 = (t2261 / 0.2E1 - t915 / 0.2E1) * t183
        t2266 = (t913 - t2100) * t183
        t2269 = (t912 / 0.2E1 - t2266 / 0.2E1) * t183
        t2073 = (t2264 - t2269) * t183
        t2273 = t397 * t2073
        t2277 = (t2084 - t1005) * t183
        t2282 = (t1020 - t2103) * t183
        t2297 = t132 * t139
        t2307 = (t912 - t915) * t183
        t2309 = ((t2261 - t912) * t183 - t2307) * t183
        t2314 = (t2307 - (t915 - t2266) * t183) * t183
        t2320 = (t1540 * t2261 - t998) * t183
        t2325 = (t999 - t1553 * t2266) * t183
        t2333 = ut(t5,t180,t1229,n)
        t2335 = (t2333 - t1002) * t236
        t2339 = ut(t5,t180,t1277,n)
        t2341 = (t1005 - t2339) * t236
        t2350 = (t2207 - t946) * t236
        t2353 = (t2350 / 0.2E1 - t951 / 0.2E1) * t236
        t2355 = (t949 - t2226) * t236
        t2358 = (t948 / 0.2E1 - t2355 / 0.2E1) * t236
        t2121 = (t2353 - t2358) * t236
        t2362 = t397 * t2121
        t2365 = ut(t5,t185,t1229,n)
        t2367 = (t2365 - t1017) * t236
        t2371 = ut(t5,t185,t1277,n)
        t2373 = (t1020 - t2371) * t236
        t2392 = (t2333 - t2207) * t183 / 0.2E1 + (t2207 - t2365) * t183 
     #/ 0.2E1
        t2396 = (t1653 * t2392 - t1061) * t236
        t2400 = (t1065 - t1076) * t236
        t2408 = (t2339 - t2226) * t183 / 0.2E1 + (t2226 - t2371) * t183 
     #/ 0.2E1
        t2412 = (t1074 - t1665 * t2408) * t236
        t2422 = t158 * t2070
        t2427 = (t1721 * t2065 - t896) * t94
        t2430 = t1222 * t94
        t2240 = ((t2247 / 0.2E1 - t1057 / 0.2E1) * t183 - (t1055 / 0.2E1
     # - t2252 / 0.2E1) * t183) * t183
        t2244 = ((t2277 / 0.2E1 - t1070 / 0.2E1) * t183 - (t1068 / 0.2E1
     # - t2282 / 0.2E1) * t183) * t183
        t2283 = ((t2335 / 0.2E1 - t1007 / 0.2E1) * t236 - (t1004 / 0.2E1
     # - t2341 / 0.2E1) * t236) * t236
        t2288 = ((t2367 / 0.2E1 - t1022 / 0.2E1) * t236 - (t1019 / 0.2E1
     # - t2373 / 0.2E1) * t236) * t236
        t2436 = -t1327 * (((t2092 - t1015) * t183 - t2096) * t183 / 0.2E
     #1 + (t2096 - (t1028 - t2111) * t183) * t183 / 0.2E1) / 0.6E1 + (t1
     #906 * t912 - t1918 * t915) * t183 - t1327 * (((t2135 - t985) * t18
     #3 - t2139) * t183 / 0.2E1 + (t2139 - (t996 - t2153) * t183) * t183
     # / 0.2E1) / 0.6E1 - t1701 * ((t452 * ((t2164 / 0.2E1 - t1033 / 0.2
     #E1) * t94 - t2170) * t94 - t2184) * t236 / 0.2E1 + (t2184 - t492 *
     # ((t2189 / 0.2E1 - t1046 / 0.2E1) * t94 - t2195) * t94) * t236 / 0
     #.2E1) / 0.6E1 - t1228 * (((t2218 - t1041) * t236 - t2222) * t236 /
     # 0.2E1 + (t2222 - (t1052 - t2237) * t236) * t236 / 0.2E1) / 0.6E1 
     #- t1327 * ((t507 * t2240 - t2273) * t236 / 0.2E1 + (t2273 - t521 *
     # t2244) * t236 / 0.2E1) / 0.6E1 + (t1657 * t142 - t2297) * t94 + (
     #t2012 * t948 - t2024 * t951) * t236 - t1327 * ((t374 * t2309 - t38
     #3 * t2314) * t183 + ((t2320 - t1001) * t183 - (t1001 - t2325) * t1
     #83) * t183) / 0.24E2 - t1228 * ((t388 * t2283 - t2362) * t183 / 0.
     #2E1 + (t2362 - t411 * t2288) * t183 / 0.2E1) / 0.6E1 + t986 - t122
     #8 * (((t2396 - t1065) * t236 - t2400) * t236 / 0.2E1 + (t2400 - (t
     #1076 - t2412) * t236) * t236 / 0.2E1) / 0.6E1 - t1701 * ((t164 * t
     #2069 - t2422) * t94 + ((t2427 - t899) * t94 - t2430) * t94) / 0.24
     #E2 + t922 + t935
        t2440 = (t948 - t951) * t236
        t2442 = ((t2350 - t948) * t236 - t2440) * t236
        t2447 = (t2440 - (t951 - t2355) * t236) * t236
        t2453 = (t1798 * t2350 - t1078) * t236
        t2458 = (t1079 - t1811 * t2355) * t236
        t2466 = ut(t1619,t180,k,n)
        t2468 = (t2466 - t900) * t94
        t2474 = (t973 / 0.2E1 - t1114 / 0.2E1) * t94
        t2481 = t196 * t2028
        t2484 = ut(t1619,t185,k,n)
        t2486 = (t2484 - t903) * t94
        t2492 = (t988 / 0.2E1 - t1127 / 0.2E1) * t94
        t2512 = (t1833 * ((t2162 - t2063) * t236 / 0.2E1 + (t2063 - t218
     #7) * t236 / 0.2E1) - t945) * t94
        t2516 = (t957 - t970) * t94
        t2520 = (t970 - t1111) * t94
        t2522 = (t2516 - t2520) * t94
        t2536 = (t1695 * ((t2466 - t2063) * t183 / 0.2E1 + (t2063 - t248
     #4) * t183 / 0.2E1) - t909) * t94
        t2540 = (t921 - t934) * t94
        t2544 = (t934 - t1098) * t94
        t2546 = (t2540 - t2544) * t94
        t2552 = (t2206 - t936) * t236
        t2557 = (t939 - t2225) * t236
        t2567 = t248 * t2121
        t2571 = (t2210 - t959) * t236
        t2574 = (t2571 / 0.2E1 - t964 / 0.2E1) * t236
        t2576 = (t962 - t2229) * t236
        t2579 = (t961 / 0.2E1 - t2576 / 0.2E1) * t236
        t2387 = (t2574 - t2579) * t236
        t2583 = t265 * t2387
        t2585 = (t2567 - t2583) * t94
        t2591 = (t2124 - t900) * t183
        t2596 = (t903 - t2142) * t183
        t2606 = t196 * t2073
        t2610 = (t2127 - t923) * t183
        t2613 = (t2610 / 0.2E1 - t928 / 0.2E1) * t183
        t2615 = (t926 - t2145) * t183
        t2618 = (t925 / 0.2E1 - t2615 / 0.2E1) * t183
        t2399 = (t2613 - t2618) * t183
        t2622 = t214 * t2399
        t2624 = (t2606 - t2622) * t94
        t2629 = t958 - t1228 * ((t551 * t2442 - t560 * t2447) * t236 + (
     #(t2453 - t1081) * t236 - (t1081 - t2458) * t236) * t236) / 0.24E2 
     #- t1701 * ((t306 * ((t2468 / 0.2E1 - t975 / 0.2E1) * t94 - t2474) 
     #* t94 - t2481) * t183 / 0.2E1 + (t2481 - t348 * ((t2486 / 0.2E1 - 
     #t990 / 0.2E1) * t94 - t2492) * t94) * t183 / 0.2E1) / 0.6E1 + t997
     # + t1016 + t1029 - t1701 * (((t2512 - t957) * t94 - t2516) * t94 /
     # 0.2E1 + t2522 / 0.2E1) / 0.6E1 + t971 + t1042 + t1053 + t1066 + t
     #1077 - t1701 * (((t2536 - t921) * t94 - t2540) * t94 / 0.2E1 + t25
     #46 / 0.2E1) / 0.6E1 - t1228 * ((t231 * ((t2552 / 0.2E1 - t941 / 0.
     #2E1) * t236 - (t938 / 0.2E1 - t2557 / 0.2E1) * t236) * t236 - t256
     #7) * t94 / 0.2E1 + t2585 / 0.2E1) / 0.6E1 - t1327 * ((t178 * ((t25
     #91 / 0.2E1 - t905 / 0.2E1) * t183 - (t902 / 0.2E1 - t2596 / 0.2E1)
     # * t183) * t183 - t2606) * t94 / 0.2E1 + t2624 / 0.2E1) / 0.6E1
        t2632 = t161 * (t2436 + t2629) * t27
        t2635 = dt * dx
        t2638 = rx(t64,t180,k,0,0)
        t2639 = rx(t64,t180,k,1,1)
        t2641 = rx(t64,t180,k,2,2)
        t2643 = rx(t64,t180,k,1,2)
        t2645 = rx(t64,t180,k,2,1)
        t2647 = rx(t64,t180,k,1,0)
        t2649 = rx(t64,t180,k,0,2)
        t2651 = rx(t64,t180,k,0,1)
        t2654 = rx(t64,t180,k,2,0)
        t2659 = t2638 * t2639 * t2641 - t2638 * t2643 * t2645 + t2647 * 
     #t2645 * t2649 - t2647 * t2651 * t2641 + t2654 * t2651 * t2643 - t2
     #654 * t2639 * t2649
        t2660 = 0.1E1 / t2659
        t2661 = t4 * t2660
        t2667 = t1967 / 0.2E1 + t309 / 0.2E1
        t2519 = t2661 * (t2638 * t2647 + t2651 * t2639 + t2649 * t2643)
        t2669 = t2519 * t2667
        t2671 = t1704 / 0.2E1 + t168 / 0.2E1
        t2673 = t178 * t2671
        t2676 = (t2669 - t2673) * t183 / 0.2E1
        t2677 = rx(t64,t185,k,0,0)
        t2678 = rx(t64,t185,k,1,1)
        t2680 = rx(t64,t185,k,2,2)
        t2682 = rx(t64,t185,k,1,2)
        t2684 = rx(t64,t185,k,2,1)
        t2686 = rx(t64,t185,k,1,0)
        t2688 = rx(t64,t185,k,0,2)
        t2690 = rx(t64,t185,k,0,1)
        t2693 = rx(t64,t185,k,2,0)
        t2698 = t2677 * t2678 * t2680 - t2677 * t2682 * t2684 + t2686 * 
     #t2684 * t2688 - t2686 * t2690 * t2680 + t2693 * t2690 * t2682 - t2
     #693 * t2678 * t2688
        t2699 = 0.1E1 / t2698
        t2700 = t4 * t2699
        t2706 = t1984 / 0.2E1 + t352 / 0.2E1
        t2543 = t2700 * (t2677 * t2686 + t2690 * t2678 + t2688 * t2682)
        t2708 = t2543 * t2706
        t2711 = (t2673 - t2708) * t183 / 0.2E1
        t2712 = t2647 ** 2
        t2713 = t2639 ** 2
        t2714 = t2643 ** 2
        t2716 = t2660 * (t2712 + t2713 + t2714)
        t2717 = t74 ** 2
        t2718 = t66 ** 2
        t2719 = t70 ** 2
        t2721 = t87 * (t2717 + t2718 + t2719)
        t2724 = t4 * (t2716 / 0.2E1 + t2721 / 0.2E1)
        t2725 = t2724 * t184
        t2726 = t2686 ** 2
        t2727 = t2678 ** 2
        t2728 = t2682 ** 2
        t2730 = t2699 * (t2726 + t2727 + t2728)
        t2733 = t4 * (t2721 / 0.2E1 + t2730 / 0.2E1)
        t2734 = t2733 * t188
        t2741 = u(t64,t180,t233,n)
        t2743 = (t2741 - t181) * t236
        t2744 = u(t64,t180,t238,n)
        t2746 = (t181 - t2744) * t236
        t2748 = t2743 / 0.2E1 + t2746 / 0.2E1
        t2563 = t2661 * (t2647 * t2654 + t2639 * t2645 + t2643 * t2641)
        t2750 = t2563 * t2748
        t2568 = t175 * (t74 * t81 + t66 * t72 + t70 * t68)
        t2756 = t2568 * t243
        t2759 = (t2750 - t2756) * t183 / 0.2E1
        t2764 = u(t64,t185,t233,n)
        t2766 = (t2764 - t186) * t236
        t2767 = u(t64,t185,t238,n)
        t2769 = (t186 - t2767) * t236
        t2771 = t2766 / 0.2E1 + t2769 / 0.2E1
        t2582 = t2700 * (t2686 * t2693 + t2678 * t2684 + t2682 * t2680)
        t2773 = t2582 * t2771
        t2776 = (t2756 - t2773) * t183 / 0.2E1
        t2777 = rx(t64,j,t233,0,0)
        t2778 = rx(t64,j,t233,1,1)
        t2780 = rx(t64,j,t233,2,2)
        t2782 = rx(t64,j,t233,1,2)
        t2784 = rx(t64,j,t233,2,1)
        t2786 = rx(t64,j,t233,1,0)
        t2788 = rx(t64,j,t233,0,2)
        t2790 = rx(t64,j,t233,0,1)
        t2793 = rx(t64,j,t233,2,0)
        t2798 = t2777 * t2778 * t2780 - t2777 * t2782 * t2784 + t2786 * 
     #t2784 * t2788 - t2786 * t2790 * t2780 + t2793 * t2790 * t2782 - t2
     #793 * t2778 * t2788
        t2799 = 0.1E1 / t2798
        t2800 = t4 * t2799
        t2806 = t1924 / 0.2E1 + t456 / 0.2E1
        t2605 = t2800 * (t2777 * t2793 + t2790 * t2784 + t2788 * t2780)
        t2808 = t2605 * t2806
        t2810 = t231 * t2671
        t2813 = (t2808 - t2810) * t236 / 0.2E1
        t2814 = rx(t64,j,t238,0,0)
        t2815 = rx(t64,j,t238,1,1)
        t2817 = rx(t64,j,t238,2,2)
        t2819 = rx(t64,j,t238,1,2)
        t2821 = rx(t64,j,t238,2,1)
        t2823 = rx(t64,j,t238,1,0)
        t2825 = rx(t64,j,t238,0,2)
        t2827 = rx(t64,j,t238,0,1)
        t2830 = rx(t64,j,t238,2,0)
        t2835 = t2815 * t2814 * t2817 - t2814 * t2819 * t2821 + t2823 * 
     #t2821 * t2825 - t2823 * t2827 * t2817 + t2830 * t2827 * t2819 - t2
     #830 * t2815 * t2825
        t2836 = 0.1E1 / t2835
        t2837 = t4 * t2836
        t2843 = t1949 / 0.2E1 + t497 / 0.2E1
        t2634 = t2837 * (t2814 * t2830 + t2827 * t2821 + t2825 * t2817)
        t2845 = t2634 * t2843
        t2848 = (t2810 - t2845) * t236 / 0.2E1
        t2854 = (t2741 - t234) * t183
        t2856 = (t234 - t2764) * t183
        t2858 = t2854 / 0.2E1 + t2856 / 0.2E1
        t2652 = t2800 * (t2786 * t2793 + t2778 * t2784 + t2782 * t2780)
        t2860 = t2652 * t2858
        t2862 = t2568 * t190
        t2865 = (t2860 - t2862) * t236 / 0.2E1
        t2871 = (t2744 - t239) * t183
        t2873 = (t239 - t2767) * t183
        t2875 = t2871 / 0.2E1 + t2873 / 0.2E1
        t2666 = t2837 * (t2823 * t2830 + t2815 * t2821 + t2819 * t2817)
        t2877 = t2666 * t2875
        t2880 = (t2862 - t2877) * t236 / 0.2E1
        t2881 = t2793 ** 2
        t2882 = t2784 ** 2
        t2883 = t2780 ** 2
        t2885 = t2799 * (t2881 + t2882 + t2883)
        t2886 = t81 ** 2
        t2887 = t72 ** 2
        t2888 = t68 ** 2
        t2890 = t87 * (t2886 + t2887 + t2888)
        t2893 = t4 * (t2885 / 0.2E1 + t2890 / 0.2E1)
        t2894 = t2893 * t237
        t2895 = t2830 ** 2
        t2896 = t2821 ** 2
        t2897 = t2817 ** 2
        t2899 = t2836 * (t2895 + t2896 + t2897)
        t2902 = t4 * (t2890 / 0.2E1 + t2899 / 0.2E1)
        t2903 = t2902 * t241
        t2906 = t1724 + t1838 / 0.2E1 + t210 + t2041 / 0.2E1 + t262 + t2
     #676 + t2711 + (t2725 - t2734) * t183 + t2759 + t2776 + t2813 + t28
     #48 + t2865 + t2880 + (t2894 - t2903) * t236
        t2907 = t2906 * t86
        t2909 = (t2907 - t565) * t94
        t2910 = t888 * t94
        t2912 = t2909 / 0.2E1 + t2910 / 0.2E1
        t2913 = t2635 * t2912
        t2921 = t1701 * (t144 - dx * (t2069 - t2070) / 0.12E2) / 0.12E2
        t2922 = t2078 * beta
        t2923 = t2922 * t893
        t2925 = t158 * t2910
        t2928 = rx(t1619,t180,k,0,0)
        t2929 = rx(t1619,t180,k,1,1)
        t2931 = rx(t1619,t180,k,2,2)
        t2933 = rx(t1619,t180,k,1,2)
        t2935 = rx(t1619,t180,k,2,1)
        t2937 = rx(t1619,t180,k,1,0)
        t2939 = rx(t1619,t180,k,0,2)
        t2941 = rx(t1619,t180,k,0,1)
        t2944 = rx(t1619,t180,k,2,0)
        t2950 = 0.1E1 / (t2928 * t2929 * t2931 - t2928 * t2933 * t2935 +
     # t2937 * t2935 * t2939 - t2937 * t2941 * t2931 + t2944 * t2941 * t
     #2933 - t2944 * t2929 * t2939)
        t2951 = t2928 ** 2
        t2952 = t2941 ** 2
        t2953 = t2939 ** 2
        t2956 = t2638 ** 2
        t2957 = t2651 ** 2
        t2958 = t2649 ** 2
        t2960 = t2660 * (t2956 + t2957 + t2958)
        t2965 = t280 ** 2
        t2966 = t293 ** 2
        t2967 = t291 ** 2
        t2969 = t302 * (t2965 + t2966 + t2967)
        t2972 = t4 * (t2960 / 0.2E1 + t2969 / 0.2E1)
        t2973 = t2972 * t309
        t2976 = t4 * t2950
        t2981 = u(t1619,t1328,k,n)
        t2989 = t1388 / 0.2E1 + t184 / 0.2E1
        t2991 = t2519 * t2989
        t2996 = t1348 / 0.2E1 + t200 / 0.2E1
        t2998 = t306 * t2996
        t3000 = (t2991 - t2998) * t94
        t3001 = t3000 / 0.2E1
        t3006 = u(t1619,t180,t233,n)
        t3009 = u(t1619,t180,t238,n)
        t2758 = t2661 * (t2638 * t2654 + t2651 * t2645 + t2649 * t2641)
        t3021 = t2758 * t2748
        t2765 = t303 * (t280 * t296 + t293 * t287 + t291 * t283)
        t3030 = t2765 * t398
        t3032 = (t3021 - t3030) * t94
        t3033 = t3032 / 0.2E1
        t3034 = rx(t64,t1328,k,0,0)
        t3035 = rx(t64,t1328,k,1,1)
        t3037 = rx(t64,t1328,k,2,2)
        t3039 = rx(t64,t1328,k,1,2)
        t3041 = rx(t64,t1328,k,2,1)
        t3043 = rx(t64,t1328,k,1,0)
        t3045 = rx(t64,t1328,k,0,2)
        t3047 = rx(t64,t1328,k,0,1)
        t3050 = rx(t64,t1328,k,2,0)
        t3056 = 0.1E1 / (t3034 * t3035 * t3037 - t3041 * t3039 * t3034 +
     # t3043 * t3041 * t3045 - t3043 * t3047 * t3037 + t3050 * t3047 * t
     #3039 - t3050 * t3035 * t3045)
        t3057 = t4 * t3056
        t3071 = t3043 ** 2
        t3072 = t3035 ** 2
        t3073 = t3039 ** 2
        t3086 = u(t64,t1328,t233,n)
        t3089 = u(t64,t1328,t238,n)
        t3093 = (t3086 - t1386) * t236 / 0.2E1 + (t1386 - t3089) * t236 
     #/ 0.2E1
        t3099 = rx(t64,t180,t233,0,0)
        t3100 = rx(t64,t180,t233,1,1)
        t3102 = rx(t64,t180,t233,2,2)
        t3104 = rx(t64,t180,t233,1,2)
        t3106 = rx(t64,t180,t233,2,1)
        t3108 = rx(t64,t180,t233,1,0)
        t3110 = rx(t64,t180,t233,0,2)
        t3112 = rx(t64,t180,t233,0,1)
        t3115 = rx(t64,t180,t233,2,0)
        t3121 = 0.1E1 / (t3099 * t3100 * t3102 - t3106 * t3104 * t3099 +
     # t3108 * t3106 * t3110 - t3108 * t3112 * t3102 + t3115 * t3112 * t
     #3104 - t3115 * t3100 * t3110)
        t3122 = t4 * t3121
        t3130 = (t2741 - t391) * t94
        t3132 = (t3006 - t2741) * t94 / 0.2E1 + t3130 / 0.2E1
        t3136 = t2758 * t2667
        t3140 = rx(t64,t180,t238,0,0)
        t3141 = rx(t64,t180,t238,1,1)
        t3143 = rx(t64,t180,t238,2,2)
        t3145 = rx(t64,t180,t238,1,2)
        t3147 = rx(t64,t180,t238,2,1)
        t3149 = rx(t64,t180,t238,1,0)
        t3151 = rx(t64,t180,t238,0,2)
        t3153 = rx(t64,t180,t238,0,1)
        t3156 = rx(t64,t180,t238,2,0)
        t3162 = 0.1E1 / (t3140 * t3141 * t3143 - t3140 * t3145 * t3147 +
     # t3149 * t3147 * t3151 - t3149 * t3153 * t3143 + t3156 * t3153 * t
     #3145 - t3156 * t3141 * t3151)
        t3163 = t4 * t3162
        t3171 = (t2744 - t394) * t94
        t3173 = (t3009 - t2744) * t94 / 0.2E1 + t3171 / 0.2E1
        t3186 = (t3086 - t2741) * t183 / 0.2E1 + t2854 / 0.2E1
        t3190 = t2563 * t2989
        t3201 = (t3089 - t2744) * t183 / 0.2E1 + t2871 / 0.2E1
        t3207 = t3115 ** 2
        t3208 = t3106 ** 2
        t3209 = t3102 ** 2
        t3212 = t2654 ** 2
        t3213 = t2645 ** 2
        t3214 = t2641 ** 2
        t3216 = t2660 * (t3212 + t3213 + t3214)
        t3221 = t3156 ** 2
        t3222 = t3147 ** 2
        t3223 = t3143 ** 2
        t2979 = t3057 * (t3034 * t3043 + t3047 * t3035 + t3045 * t3039)
        t3014 = t3122 * (t3099 * t3115 + t3112 * t3106 + t3110 * t3102)
        t3020 = t3163 * (t3140 * t3156 + t3153 * t3147 + t3151 * t3143)
        t3026 = t3122 * (t3108 * t3115 + t3100 * t3106 + t3104 * t3102)
        t3038 = t3163 * (t3149 * t3156 + t3141 * t3147 + t3145 * t3143)
        t3232 = (t4 * (t2950 * (t2951 + t2952 + t2953) / 0.2E1 + t2960 /
     # 0.2E1) * t1967 - t2973) * t94 + (t2976 * (t2928 * t2937 + t2941 *
     # t2929 + t2939 * t2933) * ((t2981 - t1827) * t183 / 0.2E1 + t1829 
     #/ 0.2E1) - t2991) * t94 / 0.2E1 + t3001 + (t2976 * (t2928 * t2944 
     #+ t2941 * t2935 + t2939 * t2931) * ((t3006 - t1827) * t236 / 0.2E1
     # + (t1827 - t3009) * t236 / 0.2E1) - t3021) * t94 / 0.2E1 + t3033 
     #+ (t2979 * ((t2981 - t1386) * t94 / 0.2E1 + t1858 / 0.2E1) - t2669
     #) * t183 / 0.2E1 + t2676 + (t4 * (t3056 * (t3071 + t3072 + t3073) 
     #/ 0.2E1 + t2716 / 0.2E1) * t1388 - t2725) * t183 + (t3057 * (t3043
     # * t3050 + t3035 * t3041 + t3039 * t3037) * t3093 - t2750) * t183 
     #/ 0.2E1 + t2759 + (t3014 * t3132 - t3136) * t236 / 0.2E1 + (t3136 
     #- t3020 * t3173) * t236 / 0.2E1 + (t3026 * t3186 - t3190) * t236 /
     # 0.2E1 + (t3190 - t3038 * t3201) * t236 / 0.2E1 + (t4 * (t3121 * (
     #t3207 + t3208 + t3209) / 0.2E1 + t3216 / 0.2E1) * t2743 - t4 * (t3
     #216 / 0.2E1 + t3162 * (t3221 + t3222 + t3223) / 0.2E1) * t2746) * 
     #t236
        t3233 = t3232 * t2659
        t3236 = rx(t1619,t185,k,0,0)
        t3237 = rx(t1619,t185,k,1,1)
        t3239 = rx(t1619,t185,k,2,2)
        t3241 = rx(t1619,t185,k,1,2)
        t3243 = rx(t1619,t185,k,2,1)
        t3245 = rx(t1619,t185,k,1,0)
        t3247 = rx(t1619,t185,k,0,2)
        t3249 = rx(t1619,t185,k,0,1)
        t3252 = rx(t1619,t185,k,2,0)
        t3258 = 0.1E1 / (t3236 * t3237 * t3239 - t3236 * t3241 * t3243 +
     # t3245 * t3243 * t3247 - t3245 * t3249 * t3239 + t3252 * t3249 * t
     #3241 - t3252 * t3237 * t3247)
        t3259 = t3236 ** 2
        t3260 = t3249 ** 2
        t3261 = t3247 ** 2
        t3264 = t2677 ** 2
        t3265 = t2690 ** 2
        t3266 = t2688 ** 2
        t3268 = t2699 * (t3264 + t3265 + t3266)
        t3273 = t323 ** 2
        t3274 = t336 ** 2
        t3275 = t334 ** 2
        t3277 = t345 * (t3273 + t3274 + t3275)
        t3280 = t4 * (t3268 / 0.2E1 + t3277 / 0.2E1)
        t3281 = t3280 * t352
        t3284 = t4 * t3258
        t3289 = u(t1619,t1335,k,n)
        t3297 = t188 / 0.2E1 + t1394 / 0.2E1
        t3299 = t2543 * t3297
        t3304 = t203 / 0.2E1 + t1354 / 0.2E1
        t3306 = t348 * t3304
        t3308 = (t3299 - t3306) * t94
        t3309 = t3308 / 0.2E1
        t3314 = u(t1619,t185,t233,n)
        t3317 = u(t1619,t185,t238,n)
        t3095 = t2700 * (t2677 * t2693 + t2690 * t2684 + t2688 * t2680)
        t3329 = t3095 * t2771
        t3101 = t346 * (t323 * t339 + t336 * t330 + t334 * t326)
        t3338 = t3101 * t421
        t3340 = (t3329 - t3338) * t94
        t3341 = t3340 / 0.2E1
        t3342 = rx(t64,t1335,k,0,0)
        t3343 = rx(t64,t1335,k,1,1)
        t3345 = rx(t64,t1335,k,2,2)
        t3347 = rx(t64,t1335,k,1,2)
        t3349 = rx(t64,t1335,k,2,1)
        t3351 = rx(t64,t1335,k,1,0)
        t3353 = rx(t64,t1335,k,0,2)
        t3355 = rx(t64,t1335,k,0,1)
        t3358 = rx(t64,t1335,k,2,0)
        t3364 = 0.1E1 / (t3343 * t3342 * t3345 - t3342 * t3347 * t3349 +
     # t3351 * t3349 * t3353 - t3351 * t3355 * t3345 + t3358 * t3355 * t
     #3347 - t3358 * t3343 * t3353)
        t3365 = t4 * t3364
        t3379 = t3351 ** 2
        t3380 = t3343 ** 2
        t3381 = t3347 ** 2
        t3394 = u(t64,t1335,t233,n)
        t3397 = u(t64,t1335,t238,n)
        t3401 = (t3394 - t1392) * t236 / 0.2E1 + (t1392 - t3397) * t236 
     #/ 0.2E1
        t3407 = rx(t64,t185,t233,0,0)
        t3408 = rx(t64,t185,t233,1,1)
        t3410 = rx(t64,t185,t233,2,2)
        t3412 = rx(t64,t185,t233,1,2)
        t3414 = rx(t64,t185,t233,2,1)
        t3416 = rx(t64,t185,t233,1,0)
        t3418 = rx(t64,t185,t233,0,2)
        t3420 = rx(t64,t185,t233,0,1)
        t3423 = rx(t64,t185,t233,2,0)
        t3429 = 0.1E1 / (t3407 * t3408 * t3410 - t3407 * t3412 * t3414 +
     # t3416 * t3414 * t3418 - t3416 * t3420 * t3410 + t3423 * t3420 * t
     #3412 - t3423 * t3408 * t3418)
        t3430 = t4 * t3429
        t3438 = (t2764 - t414) * t94
        t3440 = (t3314 - t2764) * t94 / 0.2E1 + t3438 / 0.2E1
        t3444 = t3095 * t2706
        t3448 = rx(t64,t185,t238,0,0)
        t3449 = rx(t64,t185,t238,1,1)
        t3451 = rx(t64,t185,t238,2,2)
        t3453 = rx(t64,t185,t238,1,2)
        t3455 = rx(t64,t185,t238,2,1)
        t3457 = rx(t64,t185,t238,1,0)
        t3459 = rx(t64,t185,t238,0,2)
        t3461 = rx(t64,t185,t238,0,1)
        t3464 = rx(t64,t185,t238,2,0)
        t3470 = 0.1E1 / (t3448 * t3449 * t3451 - t3448 * t3453 * t3455 +
     # t3457 * t3455 * t3459 - t3457 * t3461 * t3451 + t3464 * t3461 * t
     #3453 - t3464 * t3449 * t3459)
        t3471 = t4 * t3470
        t3479 = (t2767 - t417) * t94
        t3481 = (t3317 - t2767) * t94 / 0.2E1 + t3479 / 0.2E1
        t3494 = t2856 / 0.2E1 + (t2764 - t3394) * t183 / 0.2E1
        t3498 = t2582 * t3297
        t3509 = t2873 / 0.2E1 + (t2767 - t3397) * t183 / 0.2E1
        t3515 = t3423 ** 2
        t3516 = t3414 ** 2
        t3517 = t3410 ** 2
        t3520 = t2693 ** 2
        t3521 = t2684 ** 2
        t3522 = t2680 ** 2
        t3524 = t2699 * (t3520 + t3521 + t3522)
        t3529 = t3464 ** 2
        t3530 = t3455 ** 2
        t3531 = t3451 ** 2
        t3278 = t3365 * (t3342 * t3351 + t3355 * t3343 + t3353 * t3347)
        t3315 = t3430 * (t3407 * t3423 + t3420 * t3414 + t3418 * t3410)
        t3321 = t3471 * (t3448 * t3464 + t3461 * t3455 + t3459 * t3451)
        t3326 = t3430 * (t3416 * t3423 + t3408 * t3414 + t3412 * t3410)
        t3333 = t3471 * (t3457 * t3464 + t3449 * t3455 + t3453 * t3451)
        t3540 = (t4 * (t3258 * (t3259 + t3260 + t3261) / 0.2E1 + t3268 /
     # 0.2E1) * t1984 - t3281) * t94 + (t3284 * (t3236 * t3245 + t3249 *
     # t3237 + t3247 * t3241) * (t1832 / 0.2E1 + (t1830 - t3289) * t183 
     #/ 0.2E1) - t3299) * t94 / 0.2E1 + t3309 + (t3284 * (t3236 * t3252 
     #+ t3249 * t3243 + t3247 * t3239) * ((t3314 - t1830) * t236 / 0.2E1
     # + (t1830 - t3317) * t236 / 0.2E1) - t3329) * t94 / 0.2E1 + t3341 
     #+ t2711 + (t2708 - t3278 * ((t3289 - t1392) * t94 / 0.2E1 + t1878 
     #/ 0.2E1)) * t183 / 0.2E1 + (t2734 - t4 * (t2730 / 0.2E1 + t3364 * 
     #(t3379 + t3380 + t3381) / 0.2E1) * t1394) * t183 + t2776 + (t2773 
     #- t3365 * (t3351 * t3358 + t3343 * t3349 + t3347 * t3345) * t3401)
     # * t183 / 0.2E1 + (t3315 * t3440 - t3444) * t236 / 0.2E1 + (t3444 
     #- t3321 * t3481) * t236 / 0.2E1 + (t3326 * t3494 - t3498) * t236 /
     # 0.2E1 + (t3498 - t3333 * t3509) * t236 / 0.2E1 + (t4 * (t3429 * (
     #t3515 + t3516 + t3517) / 0.2E1 + t3524 / 0.2E1) * t2766 - t4 * (t3
     #524 / 0.2E1 + t3470 * (t3529 + t3530 + t3531) / 0.2E1) * t2769) * 
     #t236
        t3541 = t3540 * t2698
        t3548 = t610 ** 2
        t3549 = t623 ** 2
        t3550 = t621 ** 2
        t3552 = t632 * (t3548 + t3549 + t3550)
        t3555 = t4 * (t2969 / 0.2E1 + t3552 / 0.2E1)
        t3556 = t3555 * t311
        t3558 = (t2973 - t3556) * t94
        t3560 = t1409 / 0.2E1 + t218 / 0.2E1
        t3562 = t629 * t3560
        t3564 = (t2998 - t3562) * t94
        t3565 = t3564 / 0.2E1
        t3377 = t633 * (t610 * t626 + t623 * t617 + t621 * t613)
        t3571 = t3377 * t724
        t3573 = (t3030 - t3571) * t94
        t3574 = t3573 / 0.2E1
        t3575 = t1866 / 0.2E1
        t3576 = t1466 / 0.2E1
        t3577 = rx(t5,t180,t233,0,0)
        t3578 = rx(t5,t180,t233,1,1)
        t3580 = rx(t5,t180,t233,2,2)
        t3582 = rx(t5,t180,t233,1,2)
        t3584 = rx(t5,t180,t233,2,1)
        t3586 = rx(t5,t180,t233,1,0)
        t3588 = rx(t5,t180,t233,0,2)
        t3590 = rx(t5,t180,t233,0,1)
        t3593 = rx(t5,t180,t233,2,0)
        t3598 = t3577 * t3578 * t3580 - t3577 * t3582 * t3584 + t3586 * 
     #t3584 * t3588 - t3586 * t3590 * t3580 + t3593 * t3590 * t3582 - t3
     #593 * t3578 * t3588
        t3599 = 0.1E1 / t3598
        t3600 = t4 * t3599
        t3606 = (t391 - t717) * t94
        t3608 = t3130 / 0.2E1 + t3606 / 0.2E1
        t3402 = t3600 * (t3577 * t3593 + t3590 * t3584 + t3588 * t3580)
        t3610 = t3402 * t3608
        t3612 = t2765 * t313
        t3615 = (t3610 - t3612) * t236 / 0.2E1
        t3616 = rx(t5,t180,t238,0,0)
        t3617 = rx(t5,t180,t238,1,1)
        t3619 = rx(t5,t180,t238,2,2)
        t3621 = rx(t5,t180,t238,1,2)
        t3623 = rx(t5,t180,t238,2,1)
        t3625 = rx(t5,t180,t238,1,0)
        t3627 = rx(t5,t180,t238,0,2)
        t3629 = rx(t5,t180,t238,0,1)
        t3632 = rx(t5,t180,t238,2,0)
        t3637 = t3616 * t3617 * t3619 - t3616 * t3621 * t3623 + t3625 * 
     #t3623 * t3627 - t3625 * t3629 * t3619 + t3632 * t3629 * t3621 - t3
     #632 * t3617 * t3627
        t3638 = 0.1E1 / t3637
        t3639 = t4 * t3638
        t3645 = (t394 - t720) * t94
        t3647 = t3171 / 0.2E1 + t3645 / 0.2E1
        t3433 = t3639 * (t3616 * t3632 + t3629 * t3623 + t3627 * t3619)
        t3649 = t3433 * t3647
        t3652 = (t3612 - t3649) * t236 / 0.2E1
        t3658 = t1331 / 0.2E1 + t512 / 0.2E1
        t3443 = t3600 * (t3586 * t3593 + t3578 * t3584 + t3582 * t3580)
        t3660 = t3443 * t3658
        t3662 = t388 * t2996
        t3665 = (t3660 - t3662) * t236 / 0.2E1
        t3671 = t1366 / 0.2E1 + t529 / 0.2E1
        t3456 = t3639 * (t3625 * t3632 + t3617 * t3623 + t3621 * t3619)
        t3673 = t3456 * t3671
        t3676 = (t3662 - t3673) * t236 / 0.2E1
        t3677 = t3593 ** 2
        t3678 = t3584 ** 2
        t3679 = t3580 ** 2
        t3681 = t3599 * (t3677 + t3678 + t3679)
        t3682 = t296 ** 2
        t3683 = t287 ** 2
        t3684 = t283 ** 2
        t3686 = t302 * (t3682 + t3683 + t3684)
        t3689 = t4 * (t3681 / 0.2E1 + t3686 / 0.2E1)
        t3690 = t3689 * t393
        t3691 = t3632 ** 2
        t3692 = t3623 ** 2
        t3693 = t3619 ** 2
        t3695 = t3638 * (t3691 + t3692 + t3693)
        t3698 = t4 * (t3686 / 0.2E1 + t3695 / 0.2E1)
        t3699 = t3698 * t396
        t3702 = t3558 + t3001 + t3565 + t3033 + t3574 + t3575 + t322 + t
     #1543 + t3576 + t409 + t3615 + t3652 + t3665 + t3676 + (t3690 - t36
     #99) * t236
        t3703 = t3702 * t301
        t3705 = (t3703 - t565) * t183
        t3706 = t651 ** 2
        t3707 = t664 ** 2
        t3708 = t662 ** 2
        t3710 = t673 * (t3706 + t3707 + t3708)
        t3713 = t4 * (t3277 / 0.2E1 + t3710 / 0.2E1)
        t3714 = t3713 * t354
        t3716 = (t3281 - t3714) * t94
        t3718 = t221 / 0.2E1 + t1415 / 0.2E1
        t3720 = t669 * t3718
        t3722 = (t3306 - t3720) * t94
        t3723 = t3722 / 0.2E1
        t3487 = t674 * (t651 * t667 + t664 * t658 + t662 * t654)
        t3729 = t3487 * t747
        t3731 = (t3338 - t3729) * t94
        t3732 = t3731 / 0.2E1
        t3733 = t1886 / 0.2E1
        t3734 = t1510 / 0.2E1
        t3735 = rx(t5,t185,t233,0,0)
        t3736 = rx(t5,t185,t233,1,1)
        t3738 = rx(t5,t185,t233,2,2)
        t3740 = rx(t5,t185,t233,1,2)
        t3742 = rx(t5,t185,t233,2,1)
        t3744 = rx(t5,t185,t233,1,0)
        t3746 = rx(t5,t185,t233,0,2)
        t3748 = rx(t5,t185,t233,0,1)
        t3751 = rx(t5,t185,t233,2,0)
        t3756 = t3735 * t3736 * t3738 - t3735 * t3740 * t3742 + t3744 * 
     #t3742 * t3746 - t3744 * t3748 * t3738 + t3751 * t3748 * t3740 - t3
     #751 * t3736 * t3746
        t3757 = 0.1E1 / t3756
        t3758 = t4 * t3757
        t3764 = (t414 - t740) * t94
        t3766 = t3438 / 0.2E1 + t3764 / 0.2E1
        t3511 = t3758 * (t3735 * t3751 + t3748 * t3742 + t3746 * t3738)
        t3768 = t3511 * t3766
        t3770 = t3101 * t356
        t3773 = (t3768 - t3770) * t236 / 0.2E1
        t3774 = rx(t5,t185,t238,0,0)
        t3775 = rx(t5,t185,t238,1,1)
        t3777 = rx(t5,t185,t238,2,2)
        t3779 = rx(t5,t185,t238,1,2)
        t3781 = rx(t5,t185,t238,2,1)
        t3783 = rx(t5,t185,t238,1,0)
        t3785 = rx(t5,t185,t238,0,2)
        t3787 = rx(t5,t185,t238,0,1)
        t3790 = rx(t5,t185,t238,2,0)
        t3795 = t3774 * t3775 * t3777 - t3774 * t3779 * t3781 + t3783 * 
     #t3781 * t3785 - t3783 * t3787 * t3777 + t3790 * t3787 * t3779 - t3
     #790 * t3775 * t3785
        t3796 = 0.1E1 / t3795
        t3797 = t4 * t3796
        t3803 = (t417 - t743) * t94
        t3805 = t3479 / 0.2E1 + t3803 / 0.2E1
        t3543 = t3797 * (t3774 * t3790 + t3787 * t3781 + t3785 * t3777)
        t3807 = t3543 * t3805
        t3810 = (t3770 - t3807) * t236 / 0.2E1
        t3816 = t514 / 0.2E1 + t1338 / 0.2E1
        t3554 = t3758 * (t3744 * t3751 + t3736 * t3742 + t3740 * t3738)
        t3818 = t3554 * t3816
        t3820 = t411 * t3304
        t3823 = (t3818 - t3820) * t236 / 0.2E1
        t3829 = t531 / 0.2E1 + t1372 / 0.2E1
        t3568 = t3797 * (t3783 * t3790 + t3775 * t3781 + t3779 * t3777)
        t3831 = t3568 * t3829
        t3834 = (t3820 - t3831) * t236 / 0.2E1
        t3835 = t3751 ** 2
        t3836 = t3742 ** 2
        t3837 = t3738 ** 2
        t3839 = t3757 * (t3835 + t3836 + t3837)
        t3840 = t339 ** 2
        t3841 = t330 ** 2
        t3842 = t326 ** 2
        t3844 = t345 * (t3840 + t3841 + t3842)
        t3847 = t4 * (t3839 / 0.2E1 + t3844 / 0.2E1)
        t3848 = t3847 * t416
        t3849 = t3790 ** 2
        t3850 = t3781 ** 2
        t3851 = t3777 ** 2
        t3853 = t3796 * (t3849 + t3850 + t3851)
        t3856 = t4 * (t3844 / 0.2E1 + t3853 / 0.2E1)
        t3857 = t3856 * t419
        t3860 = t3716 + t3309 + t3723 + t3341 + t3732 + t361 + t3733 + t
     #1556 + t426 + t3734 + t3773 + t3810 + t3823 + t3834 + (t3848 - t38
     #57) * t236
        t3861 = t3860 * t344
        t3863 = (t565 - t3861) * t183
        t3865 = t3705 / 0.2E1 + t3863 / 0.2E1
        t3867 = t196 * t3865
        t3871 = rx(t96,t180,k,0,0)
        t3872 = rx(t96,t180,k,1,1)
        t3874 = rx(t96,t180,k,2,2)
        t3876 = rx(t96,t180,k,1,2)
        t3878 = rx(t96,t180,k,2,1)
        t3880 = rx(t96,t180,k,1,0)
        t3882 = rx(t96,t180,k,0,2)
        t3884 = rx(t96,t180,k,0,1)
        t3887 = rx(t96,t180,k,2,0)
        t3892 = t3871 * t3872 * t3874 - t3871 * t3876 * t3878 + t3880 * 
     #t3878 * t3882 - t3880 * t3884 * t3874 + t3887 * t3884 * t3876 - t3
     #887 * t3872 * t3882
        t3893 = 0.1E1 / t3892
        t3894 = t3871 ** 2
        t3895 = t3884 ** 2
        t3896 = t3882 ** 2
        t3898 = t3893 * (t3894 + t3895 + t3896)
        t3901 = t4 * (t3552 / 0.2E1 + t3898 / 0.2E1)
        t3902 = t3901 * t639
        t3904 = (t3556 - t3902) * t94
        t3905 = t4 * t3893
        t3910 = u(t96,t1328,k,n)
        t3912 = (t3910 - t580) * t183
        t3914 = t3912 / 0.2E1 + t582 / 0.2E1
        t3636 = t3905 * (t3871 * t3880 + t3884 * t3872 + t3882 * t3876)
        t3916 = t3636 * t3914
        t3918 = (t3562 - t3916) * t94
        t3919 = t3918 / 0.2E1
        t3924 = u(t96,t180,t233,n)
        t3926 = (t3924 - t580) * t236
        t3927 = u(t96,t180,t238,n)
        t3929 = (t580 - t3927) * t236
        t3931 = t3926 / 0.2E1 + t3929 / 0.2E1
        t3651 = t3905 * (t3871 * t3887 + t3884 * t3878 + t3882 * t3874)
        t3933 = t3651 * t3931
        t3935 = (t3571 - t3933) * t94
        t3936 = t3935 / 0.2E1
        t3937 = rx(i,t1328,k,0,0)
        t3938 = rx(i,t1328,k,1,1)
        t3940 = rx(i,t1328,k,2,2)
        t3942 = rx(i,t1328,k,1,2)
        t3944 = rx(i,t1328,k,2,1)
        t3946 = rx(i,t1328,k,1,0)
        t3948 = rx(i,t1328,k,0,2)
        t3950 = rx(i,t1328,k,0,1)
        t3953 = rx(i,t1328,k,2,0)
        t3958 = t3937 * t3938 * t3940 - t3937 * t3942 * t3944 + t3946 * 
     #t3944 * t3948 - t3946 * t3950 * t3940 + t3953 * t3950 * t3942 - t3
     #953 * t3938 * t3948
        t3959 = 0.1E1 / t3958
        t3960 = t4 * t3959
        t3964 = t3937 * t3946 + t3950 * t3938 + t3948 * t3942
        t3966 = (t1407 - t3910) * t94
        t3968 = t1860 / 0.2E1 + t3966 / 0.2E1
        t3688 = t3960 * t3964
        t3970 = t3688 * t3968
        t3972 = (t3970 - t643) * t183
        t3973 = t3972 / 0.2E1
        t3974 = t3946 ** 2
        t3975 = t3938 ** 2
        t3976 = t3942 ** 2
        t3978 = t3959 * (t3974 + t3975 + t3976)
        t3981 = t4 * (t3978 / 0.2E1 + t692 / 0.2E1)
        t3982 = t3981 * t1409
        t3984 = (t3982 - t701) * t183
        t3988 = t3946 * t3953 + t3938 * t3944 + t3942 * t3940
        t3989 = u(i,t1328,t233,n)
        t3991 = (t3989 - t1407) * t236
        t3992 = u(i,t1328,t238,n)
        t3994 = (t1407 - t3992) * t236
        t3996 = t3991 / 0.2E1 + t3994 / 0.2E1
        t3719 = t3960 * t3988
        t3998 = t3719 * t3996
        t4000 = (t3998 - t726) * t183
        t4001 = t4000 / 0.2E1
        t4002 = rx(i,t180,t233,0,0)
        t4003 = rx(i,t180,t233,1,1)
        t4005 = rx(i,t180,t233,2,2)
        t4007 = rx(i,t180,t233,1,2)
        t4009 = rx(i,t180,t233,2,1)
        t4011 = rx(i,t180,t233,1,0)
        t4013 = rx(i,t180,t233,0,2)
        t4015 = rx(i,t180,t233,0,1)
        t4018 = rx(i,t180,t233,2,0)
        t4023 = t4002 * t4003 * t4005 - t4002 * t4007 * t4009 + t4011 * 
     #t4009 * t4013 - t4011 * t4015 * t4005 + t4018 * t4015 * t4007 - t4
     #018 * t4003 * t4013
        t4024 = 0.1E1 / t4023
        t4025 = t4 * t4024
        t4031 = (t717 - t3924) * t94
        t4033 = t3606 / 0.2E1 + t4031 / 0.2E1
        t3759 = t4025 * (t4002 * t4018 + t4015 * t4009 + t4013 * t4005)
        t4035 = t3759 * t4033
        t4037 = t3377 * t641
        t4039 = (t4035 - t4037) * t236
        t4040 = t4039 / 0.2E1
        t4041 = rx(i,t180,t238,0,0)
        t4042 = rx(i,t180,t238,1,1)
        t4044 = rx(i,t180,t238,2,2)
        t4046 = rx(i,t180,t238,1,2)
        t4048 = rx(i,t180,t238,2,1)
        t4050 = rx(i,t180,t238,1,0)
        t4052 = rx(i,t180,t238,0,2)
        t4054 = rx(i,t180,t238,0,1)
        t4057 = rx(i,t180,t238,2,0)
        t4062 = t4041 * t4042 * t4044 - t4041 * t4046 * t4048 + t4050 * 
     #t4048 * t4052 - t4050 * t4054 * t4044 + t4057 * t4054 * t4046 - t4
     #057 * t4042 * t4052
        t4063 = 0.1E1 / t4062
        t4064 = t4 * t4063
        t4070 = (t720 - t3927) * t94
        t4072 = t3645 / 0.2E1 + t4070 / 0.2E1
        t3793 = t4064 * (t4041 * t4057 + t4054 * t4048 + t4052 * t4044)
        t4074 = t3793 * t4072
        t4076 = (t4037 - t4074) * t236
        t4077 = t4076 / 0.2E1
        t4083 = (t3989 - t717) * t183
        t4085 = t4083 / 0.2E1 + t834 / 0.2E1
        t3806 = t4025 * (t4011 * t4018 + t4003 * t4009 + t4007 * t4005)
        t4087 = t3806 * t4085
        t4089 = t708 * t3560
        t4091 = (t4087 - t4089) * t236
        t4092 = t4091 / 0.2E1
        t4098 = (t3992 - t720) * t183
        t4100 = t4098 / 0.2E1 + t851 / 0.2E1
        t3817 = t4064 * (t4050 * t4057 + t4042 * t4048 + t4046 * t4044)
        t4102 = t3817 * t4100
        t4104 = (t4089 - t4102) * t236
        t4105 = t4104 / 0.2E1
        t4106 = t4018 ** 2
        t4107 = t4009 ** 2
        t4108 = t4005 ** 2
        t4110 = t4024 * (t4106 + t4107 + t4108)
        t4111 = t626 ** 2
        t4112 = t617 ** 2
        t4113 = t613 ** 2
        t4115 = t632 * (t4111 + t4112 + t4113)
        t4118 = t4 * (t4110 / 0.2E1 + t4115 / 0.2E1)
        t4119 = t4118 * t719
        t4120 = t4057 ** 2
        t4121 = t4048 ** 2
        t4122 = t4044 ** 2
        t4124 = t4063 * (t4120 + t4121 + t4122)
        t4127 = t4 * (t4115 / 0.2E1 + t4124 / 0.2E1)
        t4128 = t4127 * t722
        t4130 = (t4119 - t4128) * t236
        t4131 = t3904 + t3565 + t3919 + t3574 + t3936 + t3973 + t650 + t
     #3984 + t4001 + t735 + t4040 + t4077 + t4092 + t4105 + t4130
        t4132 = t4131 * t631
        t4133 = t4132 - t887
        t4134 = t4133 * t183
        t4135 = rx(t96,t185,k,0,0)
        t4136 = rx(t96,t185,k,1,1)
        t4138 = rx(t96,t185,k,2,2)
        t4140 = rx(t96,t185,k,1,2)
        t4142 = rx(t96,t185,k,2,1)
        t4144 = rx(t96,t185,k,1,0)
        t4146 = rx(t96,t185,k,0,2)
        t4148 = rx(t96,t185,k,0,1)
        t4151 = rx(t96,t185,k,2,0)
        t4156 = t4135 * t4136 * t4138 - t4135 * t4140 * t4142 + t4144 * 
     #t4142 * t4146 - t4144 * t4148 * t4138 + t4151 * t4148 * t4140 - t4
     #151 * t4136 * t4146
        t4157 = 0.1E1 / t4156
        t4158 = t4135 ** 2
        t4159 = t4148 ** 2
        t4160 = t4146 ** 2
        t4162 = t4157 * (t4158 + t4159 + t4160)
        t4165 = t4 * (t3710 / 0.2E1 + t4162 / 0.2E1)
        t4166 = t4165 * t680
        t4168 = (t3714 - t4166) * t94
        t4169 = t4 * t4157
        t4174 = u(t96,t1335,k,n)
        t4176 = (t583 - t4174) * t183
        t4178 = t585 / 0.2E1 + t4176 / 0.2E1
        t3883 = t4169 * (t4135 * t4144 + t4148 * t4136 + t4146 * t4140)
        t4180 = t3883 * t4178
        t4182 = (t3720 - t4180) * t94
        t4183 = t4182 / 0.2E1
        t4188 = u(t96,t185,t233,n)
        t4190 = (t4188 - t583) * t236
        t4191 = u(t96,t185,t238,n)
        t4193 = (t583 - t4191) * t236
        t4195 = t4190 / 0.2E1 + t4193 / 0.2E1
        t3899 = t4169 * (t4135 * t4151 + t4148 * t4142 + t4146 * t4138)
        t4197 = t3899 * t4195
        t4199 = (t3729 - t4197) * t94
        t4200 = t4199 / 0.2E1
        t4201 = rx(i,t1335,k,0,0)
        t4202 = rx(i,t1335,k,1,1)
        t4204 = rx(i,t1335,k,2,2)
        t4206 = rx(i,t1335,k,1,2)
        t4208 = rx(i,t1335,k,2,1)
        t4210 = rx(i,t1335,k,1,0)
        t4212 = rx(i,t1335,k,0,2)
        t4214 = rx(i,t1335,k,0,1)
        t4217 = rx(i,t1335,k,2,0)
        t4222 = t4201 * t4202 * t4204 - t4201 * t4206 * t4208 + t4210 * 
     #t4208 * t4212 - t4210 * t4214 * t4204 + t4217 * t4214 * t4206 - t4
     #217 * t4202 * t4212
        t4223 = 0.1E1 / t4222
        t4224 = t4 * t4223
        t4228 = t4201 * t4210 + t4214 * t4202 + t4212 * t4206
        t4230 = (t1413 - t4174) * t94
        t4232 = t1880 / 0.2E1 + t4230 / 0.2E1
        t3941 = t4224 * t4228
        t4234 = t3941 * t4232
        t4236 = (t684 - t4234) * t183
        t4237 = t4236 / 0.2E1
        t4238 = t4210 ** 2
        t4239 = t4202 ** 2
        t4240 = t4206 ** 2
        t4242 = t4223 * (t4238 + t4239 + t4240)
        t4245 = t4 * (t706 / 0.2E1 + t4242 / 0.2E1)
        t4246 = t4245 * t1415
        t4248 = (t710 - t4246) * t183
        t4252 = t4210 * t4217 + t4202 * t4208 + t4206 * t4204
        t4253 = u(i,t1335,t233,n)
        t4255 = (t4253 - t1413) * t236
        t4256 = u(i,t1335,t238,n)
        t4258 = (t1413 - t4256) * t236
        t4260 = t4255 / 0.2E1 + t4258 / 0.2E1
        t3962 = t4224 * t4252
        t4262 = t3962 * t4260
        t4264 = (t749 - t4262) * t183
        t4265 = t4264 / 0.2E1
        t4266 = rx(i,t185,t233,0,0)
        t4267 = rx(i,t185,t233,1,1)
        t4269 = rx(i,t185,t233,2,2)
        t4271 = rx(i,t185,t233,1,2)
        t4273 = rx(i,t185,t233,2,1)
        t4275 = rx(i,t185,t233,1,0)
        t4277 = rx(i,t185,t233,0,2)
        t4279 = rx(i,t185,t233,0,1)
        t4282 = rx(i,t185,t233,2,0)
        t4287 = t4266 * t4267 * t4269 - t4266 * t4271 * t4273 + t4275 * 
     #t4273 * t4277 - t4275 * t4279 * t4269 + t4282 * t4279 * t4271 - t4
     #282 * t4267 * t4277
        t4288 = 0.1E1 / t4287
        t4289 = t4 * t4288
        t4295 = (t740 - t4188) * t94
        t4297 = t3764 / 0.2E1 + t4295 / 0.2E1
        t4006 = t4289 * (t4266 * t4282 + t4279 * t4273 + t4277 * t4269)
        t4299 = t4006 * t4297
        t4301 = t3487 * t682
        t4303 = (t4299 - t4301) * t236
        t4304 = t4303 / 0.2E1
        t4305 = rx(i,t185,t238,0,0)
        t4306 = rx(i,t185,t238,1,1)
        t4308 = rx(i,t185,t238,2,2)
        t4310 = rx(i,t185,t238,1,2)
        t4312 = rx(i,t185,t238,2,1)
        t4314 = rx(i,t185,t238,1,0)
        t4316 = rx(i,t185,t238,0,2)
        t4318 = rx(i,t185,t238,0,1)
        t4321 = rx(i,t185,t238,2,0)
        t4326 = t4305 * t4306 * t4308 - t4305 * t4310 * t4312 + t4314 * 
     #t4312 * t4316 - t4314 * t4318 * t4308 + t4321 * t4318 * t4310 - t4
     #321 * t4306 * t4316
        t4327 = 0.1E1 / t4326
        t4328 = t4 * t4327
        t4334 = (t743 - t4191) * t94
        t4336 = t3803 / 0.2E1 + t4334 / 0.2E1
        t4043 = t4328 * (t4305 * t4321 + t4318 * t4312 + t4316 * t4308)
        t4338 = t4043 * t4336
        t4340 = (t4301 - t4338) * t236
        t4341 = t4340 / 0.2E1
        t4347 = (t740 - t4253) * t183
        t4349 = t836 / 0.2E1 + t4347 / 0.2E1
        t4056 = t4289 * (t4275 * t4282 + t4267 * t4273 + t4271 * t4269)
        t4351 = t4056 * t4349
        t4353 = t731 * t3718
        t4355 = (t4351 - t4353) * t236
        t4356 = t4355 / 0.2E1
        t4362 = (t743 - t4256) * t183
        t4364 = t853 / 0.2E1 + t4362 / 0.2E1
        t4067 = t4328 * (t4314 * t4321 + t4306 * t4312 + t4310 * t4308)
        t4366 = t4067 * t4364
        t4368 = (t4353 - t4366) * t236
        t4369 = t4368 / 0.2E1
        t4370 = t4282 ** 2
        t4371 = t4273 ** 2
        t4372 = t4269 ** 2
        t4374 = t4288 * (t4370 + t4371 + t4372)
        t4375 = t667 ** 2
        t4376 = t658 ** 2
        t4377 = t654 ** 2
        t4379 = t673 * (t4375 + t4376 + t4377)
        t4382 = t4 * (t4374 / 0.2E1 + t4379 / 0.2E1)
        t4383 = t4382 * t742
        t4384 = t4321 ** 2
        t4385 = t4312 ** 2
        t4386 = t4308 ** 2
        t4388 = t4327 * (t4384 + t4385 + t4386)
        t4391 = t4 * (t4379 / 0.2E1 + t4388 / 0.2E1)
        t4392 = t4391 * t745
        t4394 = (t4383 - t4392) * t236
        t4395 = t4168 + t3723 + t4183 + t3732 + t4200 + t687 + t4237 + t
     #4248 + t752 + t4265 + t4304 + t4341 + t4356 + t4369 + t4394
        t4396 = t4395 * t672
        t4397 = t887 - t4396
        t4398 = t4397 * t183
        t4400 = t4134 / 0.2E1 + t4398 / 0.2E1
        t4402 = t214 * t4400
        t4405 = (t3867 - t4402) * t94 / 0.2E1
        t4406 = rx(t1619,j,t233,0,0)
        t4407 = rx(t1619,j,t233,1,1)
        t4409 = rx(t1619,j,t233,2,2)
        t4411 = rx(t1619,j,t233,1,2)
        t4413 = rx(t1619,j,t233,2,1)
        t4415 = rx(t1619,j,t233,1,0)
        t4417 = rx(t1619,j,t233,0,2)
        t4419 = rx(t1619,j,t233,0,1)
        t4422 = rx(t1619,j,t233,2,0)
        t4428 = 0.1E1 / (t4406 * t4407 * t4409 - t4406 * t4411 * t4413 +
     # t4415 * t4413 * t4417 - t4415 * t4419 * t4409 + t4422 * t4419 * t
     #4411 - t4422 * t4407 * t4417)
        t4429 = t4406 ** 2
        t4430 = t4419 ** 2
        t4431 = t4417 ** 2
        t4434 = t2777 ** 2
        t4435 = t2790 ** 2
        t4436 = t2788 ** 2
        t4438 = t2799 * (t4434 + t4435 + t4436)
        t4443 = t427 ** 2
        t4444 = t440 ** 2
        t4445 = t438 ** 2
        t4447 = t449 * (t4443 + t4444 + t4445)
        t4450 = t4 * (t4438 / 0.2E1 + t4447 / 0.2E1)
        t4451 = t4450 * t456
        t4454 = t4 * t4428
        t4147 = t2800 * (t2777 * t2786 + t2790 * t2778 + t2788 * t2782)
        t4472 = t4147 * t2858
        t4153 = t450 * (t427 * t436 + t440 * t428 + t438 * t432)
        t4481 = t4153 * t516
        t4483 = (t4472 - t4481) * t94
        t4484 = t4483 / 0.2E1
        t4489 = u(t1619,j,t1229,n)
        t4497 = t1663 / 0.2E1 + t237 / 0.2E1
        t4499 = t2605 * t4497
        t4504 = t1581 / 0.2E1 + t252 / 0.2E1
        t4506 = t452 * t4504
        t4508 = (t4499 - t4506) * t94
        t4509 = t4508 / 0.2E1
        t4517 = t4147 * t2806
        t4530 = t3108 ** 2
        t4531 = t3100 ** 2
        t4532 = t3104 ** 2
        t4535 = t2786 ** 2
        t4536 = t2778 ** 2
        t4537 = t2782 ** 2
        t4539 = t2799 * (t4535 + t4536 + t4537)
        t4544 = t3416 ** 2
        t4545 = t3408 ** 2
        t4546 = t3412 ** 2
        t4555 = u(t64,t180,t1229,n)
        t4559 = (t4555 - t2741) * t236 / 0.2E1 + t2743 / 0.2E1
        t4563 = t2652 * t4497
        t4567 = u(t64,t185,t1229,n)
        t4571 = (t4567 - t2764) * t236 / 0.2E1 + t2766 / 0.2E1
        t4577 = rx(t64,j,t1229,0,0)
        t4578 = rx(t64,j,t1229,1,1)
        t4580 = rx(t64,j,t1229,2,2)
        t4582 = rx(t64,j,t1229,1,2)
        t4584 = rx(t64,j,t1229,2,1)
        t4586 = rx(t64,j,t1229,1,0)
        t4588 = rx(t64,j,t1229,0,2)
        t4590 = rx(t64,j,t1229,0,1)
        t4593 = rx(t64,j,t1229,2,0)
        t4599 = 0.1E1 / (t4577 * t4578 * t4580 - t4577 * t4582 * t4584 +
     # t4586 * t4584 * t4588 - t4586 * t4590 * t4580 + t4593 * t4590 * t
     #4582 - t4593 * t4578 * t4588)
        t4600 = t4 * t4599
        t4623 = (t4555 - t1258) * t183 / 0.2E1 + (t1258 - t4567) * t183 
     #/ 0.2E1
        t4629 = t4593 ** 2
        t4630 = t4584 ** 2
        t4631 = t4580 ** 2
        t4311 = t3122 * (t3099 * t3108 + t3112 * t3100 + t3110 * t3104)
        t4320 = t3430 * (t3407 * t3416 + t3420 * t3408 + t3418 * t3412)
        t4373 = t4600 * (t4577 * t4593 + t4590 * t4584 + t4588 * t4580)
        t4640 = (t4 * (t4428 * (t4429 + t4430 + t4431) / 0.2E1 + t4438 /
     # 0.2E1) * t1924 - t4451) * t94 + (t4454 * (t4406 * t4415 + t4419 *
     # t4407 + t4417 * t4411) * ((t3006 - t1922) * t183 / 0.2E1 + (t1922
     # - t3314) * t183 / 0.2E1) - t4472) * t94 / 0.2E1 + t4484 + (t4454 
     #* (t4406 * t4422 + t4419 * t4413 + t4417 * t4409) * ((t4489 - t192
     #2) * t236 / 0.2E1 + t2033 / 0.2E1) - t4499) * t94 / 0.2E1 + t4509 
     #+ (t4311 * t3132 - t4517) * t183 / 0.2E1 + (t4517 - t4320 * t3440)
     # * t183 / 0.2E1 + (t4 * (t3121 * (t4530 + t4531 + t4532) / 0.2E1 +
     # t4539 / 0.2E1) * t2854 - t4 * (t4539 / 0.2E1 + t3429 * (t4544 + t
     #4545 + t4546) / 0.2E1) * t2856) * t183 + (t3026 * t4559 - t4563) *
     # t183 / 0.2E1 + (t4563 - t3326 * t4571) * t183 / 0.2E1 + (t4373 * 
     #((t4489 - t1258) * t94 / 0.2E1 + t1261 / 0.2E1) - t2808) * t236 / 
     #0.2E1 + t2813 + (t4600 * (t4586 * t4593 + t4578 * t4584 + t4582 * 
     #t4580) * t4623 - t2860) * t236 / 0.2E1 + t2865 + (t4 * (t4599 * (t
     #4629 + t4630 + t4631) / 0.2E1 + t2885 / 0.2E1) * t1663 - t2894) * 
     #t236
        t4641 = t4640 * t2798
        t4644 = rx(t1619,j,t238,0,0)
        t4645 = rx(t1619,j,t238,1,1)
        t4647 = rx(t1619,j,t238,2,2)
        t4649 = rx(t1619,j,t238,1,2)
        t4651 = rx(t1619,j,t238,2,1)
        t4653 = rx(t1619,j,t238,1,0)
        t4655 = rx(t1619,j,t238,0,2)
        t4657 = rx(t1619,j,t238,0,1)
        t4660 = rx(t1619,j,t238,2,0)
        t4666 = 0.1E1 / (t4644 * t4645 * t4647 - t4644 * t4649 * t4651 +
     # t4653 * t4651 * t4655 - t4653 * t4657 * t4647 + t4660 * t4657 * t
     #4649 - t4660 * t4645 * t4655)
        t4667 = t4644 ** 2
        t4668 = t4657 ** 2
        t4669 = t4655 ** 2
        t4672 = t2814 ** 2
        t4673 = t2827 ** 2
        t4674 = t2825 ** 2
        t4676 = t2836 * (t4672 + t4673 + t4674)
        t4681 = t468 ** 2
        t4682 = t481 ** 2
        t4683 = t479 ** 2
        t4685 = t490 * (t4681 + t4682 + t4683)
        t4688 = t4 * (t4676 / 0.2E1 + t4685 / 0.2E1)
        t4689 = t4688 * t497
        t4692 = t4 * t4666
        t4464 = t2837 * (t2814 * t2823 + t2827 * t2815 + t2825 * t2819)
        t4710 = t4464 * t2875
        t4468 = t491 * (t468 * t477 + t481 * t469 + t479 * t473)
        t4719 = t4468 * t533
        t4721 = (t4710 - t4719) * t94
        t4722 = t4721 / 0.2E1
        t4727 = u(t1619,j,t1277,n)
        t4735 = t241 / 0.2E1 + t1668 / 0.2E1
        t4737 = t2634 * t4735
        t4742 = t255 / 0.2E1 + t1586 / 0.2E1
        t4744 = t492 * t4742
        t4746 = (t4737 - t4744) * t94
        t4747 = t4746 / 0.2E1
        t4755 = t4464 * t2843
        t4768 = t3149 ** 2
        t4769 = t3141 ** 2
        t4770 = t3145 ** 2
        t4773 = t2823 ** 2
        t4774 = t2815 ** 2
        t4775 = t2819 ** 2
        t4777 = t2836 * (t4773 + t4774 + t4775)
        t4782 = t3457 ** 2
        t4783 = t3449 ** 2
        t4784 = t3453 ** 2
        t4793 = u(t64,t180,t1277,n)
        t4797 = t2746 / 0.2E1 + (t2744 - t4793) * t236 / 0.2E1
        t4801 = t2666 * t4735
        t4805 = u(t64,t185,t1277,n)
        t4809 = t2769 / 0.2E1 + (t2767 - t4805) * t236 / 0.2E1
        t4815 = rx(t64,j,t1277,0,0)
        t4816 = rx(t64,j,t1277,1,1)
        t4818 = rx(t64,j,t1277,2,2)
        t4820 = rx(t64,j,t1277,1,2)
        t4822 = rx(t64,j,t1277,2,1)
        t4824 = rx(t64,j,t1277,1,0)
        t4826 = rx(t64,j,t1277,0,2)
        t4828 = rx(t64,j,t1277,0,1)
        t4831 = rx(t64,j,t1277,2,0)
        t4837 = 0.1E1 / (t4815 * t4816 * t4818 - t4815 * t4820 * t4822 +
     # t4824 * t4822 * t4826 - t4824 * t4828 * t4818 + t4831 * t4828 * t
     #4820 - t4831 * t4816 * t4826)
        t4838 = t4 * t4837
        t4861 = (t4793 - t1306) * t183 / 0.2E1 + (t1306 - t4805) * t183 
     #/ 0.2E1
        t4867 = t4831 ** 2
        t4868 = t4822 ** 2
        t4869 = t4818 ** 2
        t4574 = t3163 * (t3140 * t3149 + t3153 * t3141 + t3151 * t3145)
        t4583 = t3471 * (t3448 * t3457 + t3461 * t3449 + t3459 * t3453)
        t4618 = t4838 * (t4815 * t4831 + t4828 * t4822 + t4826 * t4818)
        t4878 = (t4 * (t4666 * (t4667 + t4668 + t4669) / 0.2E1 + t4676 /
     # 0.2E1) * t1949 - t4689) * t94 + (t4692 * (t4644 * t4653 + t4657 *
     # t4645 + t4655 * t4649) * ((t3009 - t1947) * t183 / 0.2E1 + (t1947
     # - t3317) * t183 / 0.2E1) - t4710) * t94 / 0.2E1 + t4722 + (t4692 
     #* (t4644 * t4660 + t4657 * t4651 + t4655 * t4647) * (t2035 / 0.2E1
     # + (t1947 - t4727) * t236 / 0.2E1) - t4737) * t94 / 0.2E1 + t4747 
     #+ (t4574 * t3173 - t4755) * t183 / 0.2E1 + (t4755 - t4583 * t3481)
     # * t183 / 0.2E1 + (t4 * (t3162 * (t4768 + t4769 + t4770) / 0.2E1 +
     # t4777 / 0.2E1) * t2871 - t4 * (t4777 / 0.2E1 + t3470 * (t4782 + t
     #4783 + t4784) / 0.2E1) * t2873) * t183 + (t3038 * t4797 - t4801) *
     # t183 / 0.2E1 + (t4801 - t3333 * t4809) * t183 / 0.2E1 + t2848 + (
     #t2845 - t4618 * ((t4727 - t1306) * t94 / 0.2E1 + t1309 / 0.2E1)) *
     # t236 / 0.2E1 + t2880 + (t2877 - t4838 * (t4824 * t4831 + t4816 * 
     #t4822 + t4820 * t4818) * t4861) * t236 / 0.2E1 + (t2903 - t4 * (t2
     #899 / 0.2E1 + t4837 * (t4867 + t4868 + t4869) / 0.2E1) * t1668) * 
     #t236
        t4879 = t4878 * t2835
        t4886 = t753 ** 2
        t4887 = t766 ** 2
        t4888 = t764 ** 2
        t4890 = t775 * (t4886 + t4887 + t4888)
        t4893 = t4 * (t4447 / 0.2E1 + t4890 / 0.2E1)
        t4894 = t4893 * t458
        t4896 = (t4451 - t4894) * t94
        t4670 = t776 * (t753 * t762 + t766 * t754 + t764 * t758)
        t4902 = t4670 * t838
        t4904 = (t4481 - t4902) * t94
        t4905 = t4904 / 0.2E1
        t4907 = t1682 / 0.2E1 + t269 / 0.2E1
        t4909 = t771 * t4907
        t4911 = (t4506 - t4909) * t94
        t4912 = t4911 / 0.2E1
        t4684 = t3600 * (t3577 * t3586 + t3590 * t3578 + t3588 * t3582)
        t4918 = t4684 * t3608
        t4920 = t4153 * t460
        t4923 = (t4918 - t4920) * t183 / 0.2E1
        t4694 = t3758 * (t3735 * t3744 + t3748 * t3736 + t3746 * t3740)
        t4929 = t4694 * t3766
        t4932 = (t4920 - t4929) * t183 / 0.2E1
        t4933 = t3586 ** 2
        t4934 = t3578 ** 2
        t4935 = t3582 ** 2
        t4937 = t3599 * (t4933 + t4934 + t4935)
        t4938 = t436 ** 2
        t4939 = t428 ** 2
        t4940 = t432 ** 2
        t4942 = t449 * (t4938 + t4939 + t4940)
        t4945 = t4 * (t4937 / 0.2E1 + t4942 / 0.2E1)
        t4946 = t4945 * t512
        t4947 = t3744 ** 2
        t4948 = t3736 ** 2
        t4949 = t3740 ** 2
        t4951 = t3757 * (t4947 + t4948 + t4949)
        t4954 = t4 * (t4942 / 0.2E1 + t4951 / 0.2E1)
        t4955 = t4954 * t514
        t4959 = t1566 / 0.2E1 + t393 / 0.2E1
        t4961 = t3443 * t4959
        t4963 = t507 * t4504
        t4966 = (t4961 - t4963) * t183 / 0.2E1
        t4968 = t1598 / 0.2E1 + t416 / 0.2E1
        t4970 = t3554 * t4968
        t4973 = (t4963 - t4970) * t183 / 0.2E1
        t4974 = t1270 / 0.2E1
        t4975 = t1748 / 0.2E1
        t4976 = t4896 + t4484 + t4905 + t4509 + t4912 + t4923 + t4932 + 
     #(t4946 - t4955) * t183 + t4966 + t4973 + t4974 + t467 + t4975 + t5
     #23 + t1801
        t4977 = t4976 * t448
        t4979 = (t4977 - t565) * t236
        t4980 = t792 ** 2
        t4981 = t805 ** 2
        t4982 = t803 ** 2
        t4984 = t814 * (t4980 + t4981 + t4982)
        t4987 = t4 * (t4685 / 0.2E1 + t4984 / 0.2E1)
        t4988 = t4987 * t499
        t4990 = (t4689 - t4988) * t94
        t4726 = t815 * (t792 * t801 + t805 * t793 + t803 * t797)
        t4996 = t4726 * t855
        t4998 = (t4719 - t4996) * t94
        t4999 = t4998 / 0.2E1
        t5001 = t272 / 0.2E1 + t1687 / 0.2E1
        t5003 = t809 * t5001
        t5005 = (t4744 - t5003) * t94
        t5006 = t5005 / 0.2E1
        t4734 = t3639 * (t3616 * t3625 + t3629 * t3617 + t3627 * t3621)
        t5012 = t4734 * t3647
        t5014 = t4468 * t501
        t5017 = (t5012 - t5014) * t183 / 0.2E1
        t4743 = t3797 * (t3774 * t3783 + t3787 * t3775 + t3785 * t3779)
        t5023 = t4743 * t3805
        t5026 = (t5014 - t5023) * t183 / 0.2E1
        t5027 = t3625 ** 2
        t5028 = t3617 ** 2
        t5029 = t3621 ** 2
        t5031 = t3638 * (t5027 + t5028 + t5029)
        t5032 = t477 ** 2
        t5033 = t469 ** 2
        t5034 = t473 ** 2
        t5036 = t490 * (t5032 + t5033 + t5034)
        t5039 = t4 * (t5031 / 0.2E1 + t5036 / 0.2E1)
        t5040 = t5039 * t529
        t5041 = t3783 ** 2
        t5042 = t3775 ** 2
        t5043 = t3779 ** 2
        t5045 = t3796 * (t5041 + t5042 + t5043)
        t5048 = t4 * (t5036 / 0.2E1 + t5045 / 0.2E1)
        t5049 = t5048 * t531
        t5053 = t396 / 0.2E1 + t1572 / 0.2E1
        t5055 = t3456 * t5053
        t5057 = t521 * t4742
        t5060 = (t5055 - t5057) * t183 / 0.2E1
        t5062 = t419 / 0.2E1 + t1604 / 0.2E1
        t5064 = t3568 * t5062
        t5067 = (t5057 - t5064) * t183 / 0.2E1
        t5068 = t1318 / 0.2E1
        t5069 = t1768 / 0.2E1
        t5070 = t4990 + t4722 + t4999 + t4747 + t5006 + t5017 + t5026 + 
     #(t5040 - t5049) * t183 + t5060 + t5067 + t506 + t5068 + t538 + t50
     #69 + t1814
        t5071 = t5070 * t489
        t5073 = (t565 - t5071) * t236
        t5075 = t4979 / 0.2E1 + t5073 / 0.2E1
        t5077 = t248 * t5075
        t5081 = rx(t96,j,t233,0,0)
        t5082 = rx(t96,j,t233,1,1)
        t5084 = rx(t96,j,t233,2,2)
        t5086 = rx(t96,j,t233,1,2)
        t5088 = rx(t96,j,t233,2,1)
        t5090 = rx(t96,j,t233,1,0)
        t5092 = rx(t96,j,t233,0,2)
        t5094 = rx(t96,j,t233,0,1)
        t5097 = rx(t96,j,t233,2,0)
        t5102 = t5081 * t5082 * t5084 - t5081 * t5086 * t5088 + t5090 * 
     #t5088 * t5092 - t5090 * t5094 * t5084 + t5097 * t5094 * t5086 - t5
     #097 * t5082 * t5092
        t5103 = 0.1E1 / t5102
        t5104 = t5081 ** 2
        t5105 = t5094 ** 2
        t5106 = t5092 ** 2
        t5108 = t5103 * (t5104 + t5105 + t5106)
        t5111 = t4 * (t4890 / 0.2E1 + t5108 / 0.2E1)
        t5112 = t5111 * t782
        t5114 = (t4894 - t5112) * t94
        t5115 = t4 * t5103
        t5121 = (t3924 - t597) * t183
        t5123 = (t597 - t4188) * t183
        t5125 = t5121 / 0.2E1 + t5123 / 0.2E1
        t4804 = t5115 * (t5081 * t5090 + t5094 * t5082 + t5092 * t5086)
        t5127 = t4804 * t5125
        t5129 = (t4902 - t5127) * t94
        t5130 = t5129 / 0.2E1
        t5135 = u(t96,j,t1229,n)
        t5137 = (t5135 - t597) * t236
        t5139 = t5137 / 0.2E1 + t599 / 0.2E1
        t4813 = t5115 * (t5081 * t5097 + t5094 * t5088 + t5092 * t5084)
        t5141 = t4813 * t5139
        t5143 = (t4909 - t5141) * t94
        t5144 = t5143 / 0.2E1
        t4823 = t4025 * (t4002 * t4011 + t4015 * t4003 + t4013 * t4007)
        t5150 = t4823 * t4033
        t5152 = t4670 * t784
        t5154 = (t5150 - t5152) * t183
        t5155 = t5154 / 0.2E1
        t4832 = t4289 * (t4266 * t4275 + t4279 * t4267 + t4277 * t4271)
        t5161 = t4832 * t4297
        t5163 = (t5152 - t5161) * t183
        t5164 = t5163 / 0.2E1
        t5165 = t4011 ** 2
        t5166 = t4003 ** 2
        t5167 = t4007 ** 2
        t5169 = t4024 * (t5165 + t5166 + t5167)
        t5170 = t762 ** 2
        t5171 = t754 ** 2
        t5172 = t758 ** 2
        t5174 = t775 * (t5170 + t5171 + t5172)
        t5177 = t4 * (t5169 / 0.2E1 + t5174 / 0.2E1)
        t5178 = t5177 * t834
        t5179 = t4275 ** 2
        t5180 = t4267 ** 2
        t5181 = t4271 ** 2
        t5183 = t4288 * (t5179 + t5180 + t5181)
        t5186 = t4 * (t5174 / 0.2E1 + t5183 / 0.2E1)
        t5187 = t5186 * t836
        t5189 = (t5178 - t5187) * t183
        t5190 = u(i,t180,t1229,n)
        t5192 = (t5190 - t717) * t236
        t5194 = t5192 / 0.2E1 + t719 / 0.2E1
        t5196 = t3806 * t5194
        t5198 = t822 * t4907
        t5200 = (t5196 - t5198) * t183
        t5201 = t5200 / 0.2E1
        t5202 = u(i,t185,t1229,n)
        t5204 = (t5202 - t740) * t236
        t5206 = t5204 / 0.2E1 + t742 / 0.2E1
        t5208 = t4056 * t5206
        t5210 = (t5198 - t5208) * t183
        t5211 = t5210 / 0.2E1
        t5212 = rx(i,j,t1229,0,0)
        t5213 = rx(i,j,t1229,1,1)
        t5215 = rx(i,j,t1229,2,2)
        t5217 = rx(i,j,t1229,1,2)
        t5219 = rx(i,j,t1229,2,1)
        t5221 = rx(i,j,t1229,1,0)
        t5223 = rx(i,j,t1229,0,2)
        t5225 = rx(i,j,t1229,0,1)
        t5228 = rx(i,j,t1229,2,0)
        t5233 = t5212 * t5213 * t5215 - t5212 * t5217 * t5219 + t5221 * 
     #t5219 * t5223 - t5221 * t5225 * t5215 + t5228 * t5225 * t5217 - t5
     #228 * t5213 * t5223
        t5234 = 0.1E1 / t5233
        t5235 = t4 * t5234
        t5239 = t5212 * t5228 + t5225 * t5219 + t5223 * t5215
        t5241 = (t1262 - t5135) * t94
        t5243 = t1264 / 0.2E1 + t5241 / 0.2E1
        t4872 = t5235 * t5239
        t5245 = t4872 * t5243
        t5247 = (t5245 - t786) * t236
        t5248 = t5247 / 0.2E1
        t5252 = t5221 * t5228 + t5213 * t5219 + t5217 * t5215
        t5254 = (t5190 - t1262) * t183
        t5256 = (t1262 - t5202) * t183
        t5258 = t5254 / 0.2E1 + t5256 / 0.2E1
        t4882 = t5235 * t5252
        t5260 = t4882 * t5258
        t5262 = (t5260 - t840) * t236
        t5263 = t5262 / 0.2E1
        t5264 = t5228 ** 2
        t5265 = t5219 ** 2
        t5266 = t5215 ** 2
        t5268 = t5234 * (t5264 + t5265 + t5266)
        t5271 = t4 * (t5268 / 0.2E1 + t865 / 0.2E1)
        t5272 = t5271 * t1682
        t5274 = (t5272 - t874) * t236
        t5275 = t5114 + t4905 + t5130 + t4912 + t5144 + t5155 + t5164 + 
     #t5189 + t5201 + t5211 + t5248 + t791 + t5263 + t845 + t5274
        t5276 = t5275 * t774
        t5277 = t5276 - t887
        t5278 = t5277 * t236
        t5279 = rx(t96,j,t238,0,0)
        t5280 = rx(t96,j,t238,1,1)
        t5282 = rx(t96,j,t238,2,2)
        t5284 = rx(t96,j,t238,1,2)
        t5286 = rx(t96,j,t238,2,1)
        t5288 = rx(t96,j,t238,1,0)
        t5290 = rx(t96,j,t238,0,2)
        t5292 = rx(t96,j,t238,0,1)
        t5295 = rx(t96,j,t238,2,0)
        t5300 = t5279 * t5280 * t5282 - t5279 * t5284 * t5286 + t5288 * 
     #t5286 * t5290 - t5288 * t5292 * t5282 + t5295 * t5292 * t5284 - t5
     #295 * t5280 * t5290
        t5301 = 0.1E1 / t5300
        t5302 = t5279 ** 2
        t5303 = t5292 ** 2
        t5304 = t5290 ** 2
        t5306 = t5301 * (t5302 + t5303 + t5304)
        t5309 = t4 * (t4984 / 0.2E1 + t5306 / 0.2E1)
        t5310 = t5309 * t821
        t5312 = (t4988 - t5310) * t94
        t5313 = t4 * t5301
        t5319 = (t3927 - t600) * t183
        t5321 = (t600 - t4191) * t183
        t5323 = t5319 / 0.2E1 + t5321 / 0.2E1
        t4931 = t5313 * (t5279 * t5288 + t5292 * t5280 + t5290 * t5284)
        t5325 = t4931 * t5323
        t5327 = (t4996 - t5325) * t94
        t5328 = t5327 / 0.2E1
        t5333 = u(t96,j,t1277,n)
        t5335 = (t600 - t5333) * t236
        t5337 = t602 / 0.2E1 + t5335 / 0.2E1
        t4953 = t5313 * (t5279 * t5295 + t5292 * t5286 + t5290 * t5282)
        t5339 = t4953 * t5337
        t5341 = (t5003 - t5339) * t94
        t5342 = t5341 / 0.2E1
        t4962 = t4064 * (t4041 * t4050 + t4054 * t4042 + t4052 * t4046)
        t5348 = t4962 * t4072
        t5350 = t4726 * t823
        t5352 = (t5348 - t5350) * t183
        t5353 = t5352 / 0.2E1
        t4971 = t4328 * (t4305 * t4314 + t4318 * t4306 + t4316 * t4310)
        t5359 = t4971 * t4336
        t5361 = (t5350 - t5359) * t183
        t5362 = t5361 / 0.2E1
        t5363 = t4050 ** 2
        t5364 = t4042 ** 2
        t5365 = t4046 ** 2
        t5367 = t4063 * (t5363 + t5364 + t5365)
        t5368 = t801 ** 2
        t5369 = t793 ** 2
        t5370 = t797 ** 2
        t5372 = t814 * (t5368 + t5369 + t5370)
        t5375 = t4 * (t5367 / 0.2E1 + t5372 / 0.2E1)
        t5376 = t5375 * t851
        t5377 = t4314 ** 2
        t5378 = t4306 ** 2
        t5379 = t4310 ** 2
        t5381 = t4327 * (t5377 + t5378 + t5379)
        t5384 = t4 * (t5372 / 0.2E1 + t5381 / 0.2E1)
        t5385 = t5384 * t853
        t5387 = (t5376 - t5385) * t183
        t5388 = u(i,t180,t1277,n)
        t5390 = (t720 - t5388) * t236
        t5392 = t722 / 0.2E1 + t5390 / 0.2E1
        t5394 = t3817 * t5392
        t5396 = t837 * t5001
        t5398 = (t5394 - t5396) * t183
        t5399 = t5398 / 0.2E1
        t5400 = u(i,t185,t1277,n)
        t5402 = (t743 - t5400) * t236
        t5404 = t745 / 0.2E1 + t5402 / 0.2E1
        t5406 = t4067 * t5404
        t5408 = (t5396 - t5406) * t183
        t5409 = t5408 / 0.2E1
        t5410 = rx(i,j,t1277,0,0)
        t5411 = rx(i,j,t1277,1,1)
        t5413 = rx(i,j,t1277,2,2)
        t5415 = rx(i,j,t1277,1,2)
        t5417 = rx(i,j,t1277,2,1)
        t5419 = rx(i,j,t1277,1,0)
        t5421 = rx(i,j,t1277,0,2)
        t5423 = rx(i,j,t1277,0,1)
        t5426 = rx(i,j,t1277,2,0)
        t5431 = t5410 * t5411 * t5413 - t5410 * t5415 * t5417 + t5419 * 
     #t5417 * t5421 - t5419 * t5423 * t5413 + t5426 * t5423 * t5415 - t5
     #426 * t5411 * t5421
        t5432 = 0.1E1 / t5431
        t5433 = t4 * t5432
        t5437 = t5410 * t5426 + t5423 * t5417 + t5421 * t5413
        t5439 = (t1310 - t5333) * t94
        t5441 = t1312 / 0.2E1 + t5439 / 0.2E1
        t5046 = t5433 * t5437
        t5443 = t5046 * t5441
        t5445 = (t825 - t5443) * t236
        t5446 = t5445 / 0.2E1
        t5450 = t5419 * t5426 + t5411 * t5417 + t5415 * t5413
        t5452 = (t5388 - t1310) * t183
        t5454 = (t1310 - t5400) * t183
        t5456 = t5452 / 0.2E1 + t5454 / 0.2E1
        t5059 = t5433 * t5450
        t5458 = t5059 * t5456
        t5460 = (t857 - t5458) * t236
        t5461 = t5460 / 0.2E1
        t5462 = t5426 ** 2
        t5463 = t5417 ** 2
        t5464 = t5413 ** 2
        t5466 = t5432 * (t5462 + t5463 + t5464)
        t5469 = t4 * (t879 / 0.2E1 + t5466 / 0.2E1)
        t5470 = t5469 * t1687
        t5472 = (t883 - t5470) * t236
        t5473 = t5312 + t4999 + t5328 + t5006 + t5342 + t5353 + t5362 + 
     #t5387 + t5399 + t5409 + t828 + t5446 + t860 + t5461 + t5472
        t5474 = t5473 * t813
        t5475 = t887 - t5474
        t5476 = t5475 * t236
        t5478 = t5278 / 0.2E1 + t5476 / 0.2E1
        t5480 = t265 * t5478
        t5483 = (t5077 - t5480) * t94 / 0.2E1
        t5487 = (t3703 - t4132) * t94
        t5493 = t196 * t2912
        t5500 = (t3861 - t4396) * t94
        t5512 = t3099 ** 2
        t5513 = t3112 ** 2
        t5514 = t3110 ** 2
        t5517 = t3577 ** 2
        t5518 = t3590 ** 2
        t5519 = t3588 ** 2
        t5521 = t3599 * (t5517 + t5518 + t5519)
        t5526 = t4002 ** 2
        t5527 = t4015 ** 2
        t5528 = t4013 ** 2
        t5530 = t4024 * (t5526 + t5527 + t5528)
        t5533 = t4 * (t5521 / 0.2E1 + t5530 / 0.2E1)
        t5534 = t5533 * t3606
        t5540 = t4684 * t3658
        t5545 = t4823 * t4085
        t5548 = (t5540 - t5545) * t94 / 0.2E1
        t5552 = t3402 * t4959
        t5557 = t3759 * t5194
        t5560 = (t5552 - t5557) * t94 / 0.2E1
        t5561 = rx(t5,t1328,t233,0,0)
        t5562 = rx(t5,t1328,t233,1,1)
        t5564 = rx(t5,t1328,t233,2,2)
        t5566 = rx(t5,t1328,t233,1,2)
        t5568 = rx(t5,t1328,t233,2,1)
        t5570 = rx(t5,t1328,t233,1,0)
        t5572 = rx(t5,t1328,t233,0,2)
        t5574 = rx(t5,t1328,t233,0,1)
        t5577 = rx(t5,t1328,t233,2,0)
        t5583 = 0.1E1 / (t5561 * t5562 * t5564 - t5561 * t5566 * t5568 +
     # t5570 * t5568 * t5572 - t5570 * t5574 * t5564 + t5577 * t5574 * t
     #5566 - t5577 * t5562 * t5572)
        t5584 = t4 * t5583
        t5592 = (t1329 - t3989) * t94
        t5594 = (t3086 - t1329) * t94 / 0.2E1 + t5592 / 0.2E1
        t5600 = t5570 ** 2
        t5601 = t5562 ** 2
        t5602 = t5566 ** 2
        t5615 = u(t5,t1328,t1229,n)
        t5619 = (t5615 - t1329) * t236 / 0.2E1 + t1458 / 0.2E1
        t5625 = rx(t5,t180,t1229,0,0)
        t5626 = rx(t5,t180,t1229,1,1)
        t5628 = rx(t5,t180,t1229,2,2)
        t5630 = rx(t5,t180,t1229,1,2)
        t5632 = rx(t5,t180,t1229,2,1)
        t5634 = rx(t5,t180,t1229,1,0)
        t5636 = rx(t5,t180,t1229,0,2)
        t5638 = rx(t5,t180,t1229,0,1)
        t5641 = rx(t5,t180,t1229,2,0)
        t5647 = 0.1E1 / (t5625 * t5626 * t5628 - t5625 * t5630 * t5632 +
     # t5634 * t5632 * t5636 - t5634 * t5638 * t5628 + t5641 * t5638 * t
     #5630 - t5641 * t5626 * t5636)
        t5648 = t4 * t5647
        t5656 = (t1564 - t5190) * t94
        t5658 = (t4555 - t1564) * t94 / 0.2E1 + t5656 / 0.2E1
        t5671 = (t5615 - t1564) * t183 / 0.2E1 + t1740 / 0.2E1
        t5677 = t5641 ** 2
        t5678 = t5632 ** 2
        t5679 = t5628 ** 2
        t5244 = t5584 * (t5561 * t5570 + t5574 * t5562 + t5572 * t5566)
        t5273 = t5584 * (t5570 * t5577 + t5562 * t5568 + t5566 * t5564)
        t5289 = t5648 * (t5625 * t5641 + t5638 * t5632 + t5636 * t5628)
        t5297 = t5648 * (t5634 * t5641 + t5626 * t5632 + t5630 * t5628)
        t5688 = (t4 * (t3121 * (t5512 + t5513 + t5514) / 0.2E1 + t5521 /
     # 0.2E1) * t3130 - t5534) * t94 + (t4311 * t3186 - t5540) * t94 / 0
     #.2E1 + t5548 + (t3014 * t4559 - t5552) * t94 / 0.2E1 + t5560 + (t5
     #244 * t5594 - t4918) * t183 / 0.2E1 + t4923 + (t4 * (t5583 * (t560
     #0 + t5601 + t5602) / 0.2E1 + t4937 / 0.2E1) * t1331 - t4946) * t18
     #3 + (t5273 * t5619 - t4961) * t183 / 0.2E1 + t4966 + (t5289 * t565
     #8 - t3610) * t236 / 0.2E1 + t3615 + (t5297 * t5671 - t3660) * t236
     # / 0.2E1 + t3665 + (t4 * (t5647 * (t5677 + t5678 + t5679) / 0.2E1 
     #+ t3681 / 0.2E1) * t1566 - t3690) * t236
        t5689 = t5688 * t3598
        t5692 = t3140 ** 2
        t5693 = t3153 ** 2
        t5694 = t3151 ** 2
        t5697 = t3616 ** 2
        t5698 = t3629 ** 2
        t5699 = t3627 ** 2
        t5701 = t3638 * (t5697 + t5698 + t5699)
        t5706 = t4041 ** 2
        t5707 = t4054 ** 2
        t5708 = t4052 ** 2
        t5710 = t4063 * (t5706 + t5707 + t5708)
        t5713 = t4 * (t5701 / 0.2E1 + t5710 / 0.2E1)
        t5714 = t5713 * t3645
        t5720 = t4734 * t3671
        t5725 = t4962 * t4100
        t5728 = (t5720 - t5725) * t94 / 0.2E1
        t5732 = t3433 * t5053
        t5737 = t3793 * t5392
        t5740 = (t5732 - t5737) * t94 / 0.2E1
        t5741 = rx(t5,t1328,t238,0,0)
        t5742 = rx(t5,t1328,t238,1,1)
        t5744 = rx(t5,t1328,t238,2,2)
        t5746 = rx(t5,t1328,t238,1,2)
        t5748 = rx(t5,t1328,t238,2,1)
        t5750 = rx(t5,t1328,t238,1,0)
        t5752 = rx(t5,t1328,t238,0,2)
        t5754 = rx(t5,t1328,t238,0,1)
        t5757 = rx(t5,t1328,t238,2,0)
        t5763 = 0.1E1 / (t5741 * t5742 * t5744 - t5741 * t5746 * t5748 +
     # t5750 * t5748 * t5752 - t5750 * t5754 * t5744 + t5757 * t5754 * t
     #5746 - t5757 * t5742 * t5752)
        t5764 = t4 * t5763
        t5772 = (t1364 - t3992) * t94
        t5774 = (t3089 - t1364) * t94 / 0.2E1 + t5772 / 0.2E1
        t5780 = t5750 ** 2
        t5781 = t5742 ** 2
        t5782 = t5746 ** 2
        t5795 = u(t5,t1328,t1277,n)
        t5799 = t1460 / 0.2E1 + (t1364 - t5795) * t236 / 0.2E1
        t5805 = rx(t5,t180,t1277,0,0)
        t5806 = rx(t5,t180,t1277,1,1)
        t5808 = rx(t5,t180,t1277,2,2)
        t5810 = rx(t5,t180,t1277,1,2)
        t5812 = rx(t5,t180,t1277,2,1)
        t5814 = rx(t5,t180,t1277,1,0)
        t5816 = rx(t5,t180,t1277,0,2)
        t5818 = rx(t5,t180,t1277,0,1)
        t5821 = rx(t5,t180,t1277,2,0)
        t5827 = 0.1E1 / (t5805 * t5806 * t5808 - t5805 * t5810 * t5812 +
     # t5814 * t5812 * t5816 - t5814 * t5818 * t5808 + t5821 * t5818 * t
     #5810 - t5821 * t5806 * t5816)
        t5828 = t4 * t5827
        t5836 = (t1570 - t5388) * t94
        t5838 = (t4793 - t1570) * t94 / 0.2E1 + t5836 / 0.2E1
        t5851 = (t5795 - t1570) * t183 / 0.2E1 + t1760 / 0.2E1
        t5857 = t5821 ** 2
        t5858 = t5812 ** 2
        t5859 = t5808 ** 2
        t5489 = t5764 * (t5741 * t5750 + t5754 * t5742 + t5752 * t5746)
        t5504 = t5764 * (t5750 * t5757 + t5742 * t5748 + t5746 * t5744)
        t5509 = t5828 * (t5805 * t5821 + t5818 * t5812 + t5816 * t5808)
        t5520 = t5828 * (t5814 * t5821 + t5806 * t5812 + t5810 * t5808)
        t5868 = (t4 * (t3162 * (t5692 + t5693 + t5694) / 0.2E1 + t5701 /
     # 0.2E1) * t3171 - t5714) * t94 + (t4574 * t3201 - t5720) * t94 / 0
     #.2E1 + t5728 + (t3020 * t4797 - t5732) * t94 / 0.2E1 + t5740 + (t5
     #489 * t5774 - t5012) * t183 / 0.2E1 + t5017 + (t4 * (t5763 * (t578
     #0 + t5781 + t5782) / 0.2E1 + t5031 / 0.2E1) * t1366 - t5040) * t18
     #3 + (t5504 * t5799 - t5055) * t183 / 0.2E1 + t5060 + t3652 + (t364
     #9 - t5509 * t5838) * t236 / 0.2E1 + t3676 + (t3673 - t5520 * t5851
     #) * t236 / 0.2E1 + (t3699 - t4 * (t3695 / 0.2E1 + t5827 * (t5857 +
     # t5858 + t5859) / 0.2E1) * t1572) * t236
        t5869 = t5868 * t3637
        t5873 = (t5689 - t3703) * t236 / 0.2E1 + (t3703 - t5869) * t236 
     #/ 0.2E1
        t5877 = t397 * t5075
        t5881 = t3407 ** 2
        t5882 = t3420 ** 2
        t5883 = t3418 ** 2
        t5886 = t3735 ** 2
        t5887 = t3748 ** 2
        t5888 = t3746 ** 2
        t5890 = t3757 * (t5886 + t5887 + t5888)
        t5895 = t4266 ** 2
        t5896 = t4279 ** 2
        t5897 = t4277 ** 2
        t5899 = t4288 * (t5895 + t5896 + t5897)
        t5902 = t4 * (t5890 / 0.2E1 + t5899 / 0.2E1)
        t5903 = t5902 * t3764
        t5909 = t4694 * t3816
        t5914 = t4832 * t4349
        t5917 = (t5909 - t5914) * t94 / 0.2E1
        t5921 = t3511 * t4968
        t5926 = t4006 * t5206
        t5929 = (t5921 - t5926) * t94 / 0.2E1
        t5930 = rx(t5,t1335,t233,0,0)
        t5931 = rx(t5,t1335,t233,1,1)
        t5933 = rx(t5,t1335,t233,2,2)
        t5935 = rx(t5,t1335,t233,1,2)
        t5937 = rx(t5,t1335,t233,2,1)
        t5939 = rx(t5,t1335,t233,1,0)
        t5941 = rx(t5,t1335,t233,0,2)
        t5943 = rx(t5,t1335,t233,0,1)
        t5946 = rx(t5,t1335,t233,2,0)
        t5952 = 0.1E1 / (t5930 * t5931 * t5933 - t5930 * t5935 * t5937 +
     # t5939 * t5937 * t5941 - t5939 * t5943 * t5933 + t5946 * t5943 * t
     #5935 - t5946 * t5931 * t5941)
        t5953 = t4 * t5952
        t5961 = (t1336 - t4253) * t94
        t5963 = (t3394 - t1336) * t94 / 0.2E1 + t5961 / 0.2E1
        t5969 = t5939 ** 2
        t5970 = t5931 ** 2
        t5971 = t5935 ** 2
        t5984 = u(t5,t1335,t1229,n)
        t5988 = (t5984 - t1336) * t236 / 0.2E1 + t1502 / 0.2E1
        t5994 = rx(t5,t185,t1229,0,0)
        t5995 = rx(t5,t185,t1229,1,1)
        t5997 = rx(t5,t185,t1229,2,2)
        t5999 = rx(t5,t185,t1229,1,2)
        t6001 = rx(t5,t185,t1229,2,1)
        t6003 = rx(t5,t185,t1229,1,0)
        t6005 = rx(t5,t185,t1229,0,2)
        t6007 = rx(t5,t185,t1229,0,1)
        t6010 = rx(t5,t185,t1229,2,0)
        t6016 = 0.1E1 / (t5994 * t5995 * t5997 - t5994 * t5999 * t6001 +
     # t6003 * t6001 * t6005 - t6003 * t6007 * t5997 + t6010 * t6007 * t
     #5999 - t6010 * t5995 * t6005)
        t6017 = t4 * t6016
        t6025 = (t1596 - t5202) * t94
        t6027 = (t4567 - t1596) * t94 / 0.2E1 + t6025 / 0.2E1
        t6040 = t1742 / 0.2E1 + (t1596 - t5984) * t183 / 0.2E1
        t6046 = t6010 ** 2
        t6047 = t6001 ** 2
        t6048 = t5997 ** 2
        t5661 = t5953 * (t5930 * t5939 + t5943 * t5931 + t5941 * t5935)
        t5676 = t5953 * (t5939 * t5946 + t5931 * t5937 + t5935 * t5933)
        t5684 = t6017 * (t5994 * t6010 + t6007 * t6001 + t6005 * t5997)
        t5691 = t6017 * (t6003 * t6010 + t5995 * t6001 + t5999 * t5997)
        t6057 = (t4 * (t3429 * (t5881 + t5882 + t5883) / 0.2E1 + t5890 /
     # 0.2E1) * t3438 - t5903) * t94 + (t4320 * t3494 - t5909) * t94 / 0
     #.2E1 + t5917 + (t3315 * t4571 - t5921) * t94 / 0.2E1 + t5929 + t49
     #32 + (t4929 - t5661 * t5963) * t183 / 0.2E1 + (t4955 - t4 * (t4951
     # / 0.2E1 + t5952 * (t5969 + t5970 + t5971) / 0.2E1) * t1338) * t18
     #3 + t4973 + (t4970 - t5676 * t5988) * t183 / 0.2E1 + (t5684 * t602
     #7 - t3768) * t236 / 0.2E1 + t3773 + (t5691 * t6040 - t3818) * t236
     # / 0.2E1 + t3823 + (t4 * (t6016 * (t6046 + t6047 + t6048) / 0.2E1 
     #+ t3839 / 0.2E1) * t1598 - t3848) * t236
        t6058 = t6057 * t3756
        t6061 = t3448 ** 2
        t6062 = t3461 ** 2
        t6063 = t3459 ** 2
        t6066 = t3774 ** 2
        t6067 = t3787 ** 2
        t6068 = t3785 ** 2
        t6070 = t3796 * (t6066 + t6067 + t6068)
        t6075 = t4305 ** 2
        t6076 = t4318 ** 2
        t6077 = t4316 ** 2
        t6079 = t4327 * (t6075 + t6076 + t6077)
        t6082 = t4 * (t6070 / 0.2E1 + t6079 / 0.2E1)
        t6083 = t6082 * t3803
        t6089 = t4743 * t3829
        t6094 = t4971 * t4364
        t6097 = (t6089 - t6094) * t94 / 0.2E1
        t6101 = t3543 * t5062
        t6106 = t4043 * t5404
        t6109 = (t6101 - t6106) * t94 / 0.2E1
        t6110 = rx(t5,t1335,t238,0,0)
        t6111 = rx(t5,t1335,t238,1,1)
        t6113 = rx(t5,t1335,t238,2,2)
        t6115 = rx(t5,t1335,t238,1,2)
        t6117 = rx(t5,t1335,t238,2,1)
        t6119 = rx(t5,t1335,t238,1,0)
        t6121 = rx(t5,t1335,t238,0,2)
        t6123 = rx(t5,t1335,t238,0,1)
        t6126 = rx(t5,t1335,t238,2,0)
        t6132 = 0.1E1 / (t6110 * t6111 * t6113 - t6110 * t6115 * t6117 +
     # t6119 * t6117 * t6121 - t6119 * t6123 * t6113 + t6126 * t6123 * t
     #6115 - t6126 * t6111 * t6121)
        t6133 = t4 * t6132
        t6141 = (t1370 - t4256) * t94
        t6143 = (t3397 - t1370) * t94 / 0.2E1 + t6141 / 0.2E1
        t6149 = t6119 ** 2
        t6150 = t6111 ** 2
        t6151 = t6115 ** 2
        t6164 = u(t5,t1335,t1277,n)
        t6168 = t1504 / 0.2E1 + (t1370 - t6164) * t236 / 0.2E1
        t6174 = rx(t5,t185,t1277,0,0)
        t6175 = rx(t5,t185,t1277,1,1)
        t6177 = rx(t5,t185,t1277,2,2)
        t6179 = rx(t5,t185,t1277,1,2)
        t6181 = rx(t5,t185,t1277,2,1)
        t6183 = rx(t5,t185,t1277,1,0)
        t6185 = rx(t5,t185,t1277,0,2)
        t6187 = rx(t5,t185,t1277,0,1)
        t6190 = rx(t5,t185,t1277,2,0)
        t6196 = 0.1E1 / (t6174 * t6175 * t6177 - t6174 * t6179 * t6181 +
     # t6183 * t6181 * t6185 - t6183 * t6187 * t6177 + t6190 * t6187 * t
     #6179 - t6190 * t6175 * t6185)
        t6197 = t4 * t6196
        t6205 = (t1602 - t5400) * t94
        t6207 = (t4805 - t1602) * t94 / 0.2E1 + t6205 / 0.2E1
        t6220 = t1762 / 0.2E1 + (t1602 - t6164) * t183 / 0.2E1
        t6226 = t6190 ** 2
        t6227 = t6181 ** 2
        t6228 = t6177 ** 2
        t5830 = t6133 * (t6110 * t6119 + t6123 * t6111 + t6121 * t6115)
        t5846 = t6133 * (t6119 * t6126 + t6111 * t6117 + t6115 * t6113)
        t5853 = t6197 * (t6174 * t6190 + t6187 * t6181 + t6185 * t6177)
        t5861 = t6197 * (t6183 * t6190 + t6175 * t6181 + t6179 * t6177)
        t6237 = (t4 * (t3470 * (t6061 + t6062 + t6063) / 0.2E1 + t6070 /
     # 0.2E1) * t3479 - t6083) * t94 + (t4583 * t3509 - t6089) * t94 / 0
     #.2E1 + t6097 + (t3321 * t4809 - t6101) * t94 / 0.2E1 + t6109 + t50
     #26 + (t5023 - t5830 * t6143) * t183 / 0.2E1 + (t5049 - t4 * (t5045
     # / 0.2E1 + t6132 * (t6149 + t6150 + t6151) / 0.2E1) * t1372) * t18
     #3 + t5067 + (t5064 - t5846 * t6168) * t183 / 0.2E1 + t3810 + (t380
     #7 - t5853 * t6207) * t236 / 0.2E1 + t3834 + (t3831 - t5861 * t6220
     #) * t236 / 0.2E1 + (t3857 - t4 * (t3853 / 0.2E1 + t6196 * (t6226 +
     # t6227 + t6228) / 0.2E1) * t1604) * t236
        t6238 = t6237 * t3795
        t6242 = (t6058 - t3861) * t236 / 0.2E1 + (t3861 - t6238) * t236 
     #/ 0.2E1
        t6251 = (t4977 - t5276) * t94
        t6257 = t248 * t2912
        t6264 = (t5071 - t5474) * t94
        t6277 = (t5689 - t4977) * t183 / 0.2E1 + (t4977 - t6058) * t183 
     #/ 0.2E1
        t6281 = t397 * t3865
        t6290 = (t5869 - t5071) * t183 / 0.2E1 + (t5071 - t6238) * t183 
     #/ 0.2E1
        t6300 = (t164 * t2909 - t2925) * t94 + (t178 * ((t3233 - t2907) 
     #* t183 / 0.2E1 + (t2907 - t3541) * t183 / 0.2E1) - t3867) * t94 / 
     #0.2E1 + t4405 + (t231 * ((t4641 - t2907) * t236 / 0.2E1 + (t2907 -
     # t4879) * t236 / 0.2E1) - t5077) * t94 / 0.2E1 + t5483 + (t306 * (
     #(t3233 - t3703) * t94 / 0.2E1 + t5487 / 0.2E1) - t5493) * t183 / 0
     #.2E1 + (t5493 - t348 * ((t3541 - t3861) * t94 / 0.2E1 + t5500 / 0.
     #2E1)) * t183 / 0.2E1 + (t374 * t3705 - t383 * t3863) * t183 + (t38
     #8 * t5873 - t5877) * t183 / 0.2E1 + (t5877 - t411 * t6242) * t183 
     #/ 0.2E1 + (t452 * ((t4641 - t4977) * t94 / 0.2E1 + t6251 / 0.2E1) 
     #- t6257) * t236 / 0.2E1 + (t6257 - t492 * ((t4879 - t5071) * t94 /
     # 0.2E1 + t6264 / 0.2E1)) * t236 / 0.2E1 + (t507 * t6277 - t6281) *
     # t236 / 0.2E1 + (t6281 - t521 * t6290) * t236 / 0.2E1 + (t551 * t4
     #979 - t560 * t5073) * t236
        t6302 = t895 * t6300 * t27
        t6305 = t161 * dx
        t6313 = t2065 / 0.2E1 + t142 / 0.2E1
        t6315 = t178 * t6313
        t6330 = ut(t64,t180,t233,n)
        t6333 = ut(t64,t180,t238,n)
        t6337 = (t6330 - t900) * t236 / 0.2E1 + (t900 - t6333) * t236 / 
     #0.2E1
        t6341 = t2568 * t943
        t6345 = ut(t64,t185,t233,n)
        t6348 = ut(t64,t185,t238,n)
        t6352 = (t6345 - t903) * t236 / 0.2E1 + (t903 - t6348) * t236 / 
     #0.2E1
        t6363 = t231 * t6313
        t6379 = (t6330 - t936) * t183 / 0.2E1 + (t936 - t6345) * t183 / 
     #0.2E1
        t6383 = t2568 * t907
        t6392 = (t6333 - t939) * t183 / 0.2E1 + (t939 - t6348) * t183 / 
     #0.2E1
        t6402 = t2427 + t2536 / 0.2E1 + t922 + t2512 / 0.2E1 + t958 + (t
     #2519 * (t2468 / 0.2E1 + t973 / 0.2E1) - t6315) * t183 / 0.2E1 + (t
     #6315 - t2543 * (t2486 / 0.2E1 + t988 / 0.2E1)) * t183 / 0.2E1 + (t
     #2724 * t902 - t2733 * t905) * t183 + (t2563 * t6337 - t6341) * t18
     #3 / 0.2E1 + (t6341 - t2582 * t6352) * t183 / 0.2E1 + (t2605 * (t21
     #64 / 0.2E1 + t1031 / 0.2E1) - t6363) * t236 / 0.2E1 + (t6363 - t26
     #34 * (t2189 / 0.2E1 + t1044 / 0.2E1)) * t236 / 0.2E1 + (t2652 * t6
     #379 - t6383) * t236 / 0.2E1 + (t6383 - t2666 * t6392) * t236 / 0.2
     #E1 + (t2893 * t938 - t2902 * t941) * t236
        t6406 = t1217 * t94
        t6409 = t6305 * ((t6402 * t86 - t1083) * t94 / 0.2E1 + t6406 / 0
     #.2E1)
        t6413 = t2635 * (t2909 - t2910)
        t6417 = (t3972 - t649) * t183
        t6419 = (t649 - t686) * t183
        t6421 = (t6417 - t6419) * t183
        t6423 = (t686 - t4236) * t183
        t6425 = (t6419 - t6423) * t183
        t6430 = t692 / 0.2E1
        t6431 = t697 / 0.2E1
        t6435 = (t697 - t706) * t183
        t6440 = t6430 + t6431 - dy * ((t3978 - t692) * t183 / 0.2E1 - t6
     #435 / 0.2E1) / 0.8E1
        t6441 = t4 * t6440
        t6442 = t6441 * t218
        t6443 = t706 / 0.2E1
        t6445 = (t692 - t697) * t183
        t6452 = t6431 + t6443 - dy * (t6445 / 0.2E1 - (t706 - t4242) * t
     #183 / 0.2E1) / 0.8E1
        t6453 = t4 * t6452
        t6454 = t6453 * t221
        t6457 = t124 / 0.2E1
        t6458 = i - 2
        t6459 = rx(t6458,j,k,0,0)
        t6460 = rx(t6458,j,k,1,1)
        t6462 = rx(t6458,j,k,2,2)
        t6464 = rx(t6458,j,k,1,2)
        t6466 = rx(t6458,j,k,2,1)
        t6468 = rx(t6458,j,k,1,0)
        t6470 = rx(t6458,j,k,0,2)
        t6472 = rx(t6458,j,k,0,1)
        t6475 = rx(t6458,j,k,2,0)
        t6480 = t6459 * t6460 * t6462 - t6459 * t6464 * t6466 + t6468 * 
     #t6466 * t6470 - t6468 * t6472 * t6462 + t6475 * t6472 * t6464 - t6
     #475 * t6460 * t6470
        t6481 = 0.1E1 / t6480
        t6482 = t6459 ** 2
        t6483 = t6472 ** 2
        t6484 = t6470 ** 2
        t6486 = t6481 * (t6482 + t6483 + t6484)
        t6493 = t63 + t6457 - dx * (t1651 / 0.2E1 - (t124 - t6486) * t94
     # / 0.2E1) / 0.8E1
        t6494 = t4 * t6493
        t6495 = t6494 * t571
        t6499 = (t4000 - t734) * t183
        t6501 = (t734 - t751) * t183
        t6503 = (t6499 - t6501) * t183
        t6505 = (t751 - t4264) * t183
        t6507 = (t6501 - t6505) * t183
        t6512 = t4 * t6481
        t6516 = t6459 * t6468 + t6472 * t6460 + t6470 * t6464
        t6517 = u(t6458,t180,k,n)
        t6518 = u(t6458,j,k,n)
        t6520 = (t6517 - t6518) * t183
        t6521 = u(t6458,t185,k,n)
        t6523 = (t6518 - t6521) * t183
        t6525 = t6520 / 0.2E1 + t6523 / 0.2E1
        t6214 = t6512 * t6516
        t6527 = t6214 * t6525
        t6529 = (t589 - t6527) * t94
        t6531 = (t591 - t6529) * t94
        t6533 = (t1846 - t6531) * t94
        t6539 = (t1682 - t269) * t236
        t6541 = (t269 - t272) * t236
        t6542 = t6539 - t6541
        t6543 = t6542 * t236
        t6544 = t873 * t6543
        t6546 = (t272 - t1687) * t236
        t6547 = t6541 - t6546
        t6548 = t6547 * t236
        t6549 = t882 * t6548
        t6552 = t5274 - t885
        t6553 = t6552 * t236
        t6554 = t885 - t5472
        t6555 = t6554 * t236
        t6563 = (t4083 / 0.2E1 - t836 / 0.2E1) * t183
        t6566 = (t834 / 0.2E1 - t4347 / 0.2E1) * t183
        t6230 = (t6563 - t6566) * t183
        t6570 = t822 * t6230
        t6573 = t715 * t1275
        t6575 = (t6570 - t6573) * t236
        t6578 = (t4098 / 0.2E1 - t853 / 0.2E1) * t183
        t6581 = (t851 / 0.2E1 - t4362 / 0.2E1) * t183
        t6236 = (t6578 - t6581) * t183
        t6585 = t837 * t6236
        t6587 = (t6573 - t6585) * t236
        t6594 = (t5192 / 0.2E1 - t722 / 0.2E1) * t236
        t6597 = (t719 / 0.2E1 - t5390 / 0.2E1) * t236
        t6245 = (t6594 - t6597) * t236
        t6601 = t708 * t6245
        t6604 = t715 * t1457
        t6606 = (t6601 - t6604) * t183
        t6609 = (t5204 / 0.2E1 - t745 / 0.2E1) * t236
        t6612 = (t742 / 0.2E1 - t5402 / 0.2E1) * t236
        t6252 = (t6609 - t6612) * t236
        t6616 = t731 * t6252
        t6618 = (t6604 - t6616) * t183
        t6623 = u(t6458,j,t233,n)
        t6625 = (t597 - t6623) * t94
        t6628 = (t458 / 0.2E1 - t6625 / 0.2E1) * t94
        t6258 = (t1930 - t6628) * t94
        t6632 = t771 * t6258
        t6634 = (t569 - t6518) * t94
        t6637 = (t171 / 0.2E1 - t6634 / 0.2E1) * t94
        t6262 = (t1940 - t6637) * t94
        t6641 = t265 * t6262
        t6643 = (t6632 - t6641) * t236
        t6644 = u(t6458,j,t238,n)
        t6646 = (t600 - t6644) * t94
        t6649 = (t499 / 0.2E1 - t6646 / 0.2E1) * t94
        t6268 = (t1955 - t6649) * t94
        t6653 = t809 * t6268
        t6655 = (t6641 - t6653) * t236
        t6662 = (t5137 / 0.2E1 - t602 / 0.2E1) * t236
        t6665 = (t599 / 0.2E1 - t5335 / 0.2E1) * t236
        t6274 = (t6662 - t6665) * t236
        t6669 = t590 * t6274
        t6671 = (t1694 - t6669) * t94
        t6677 = (t1409 - t218) * t183
        t6679 = (t218 - t221) * t183
        t6680 = t6677 - t6679
        t6681 = t6680 * t183
        t6682 = t700 * t6681
        t6684 = (t221 - t1415) * t183
        t6685 = t6679 - t6684
        t6686 = t6685 * t183
        t6687 = t709 * t6686
        t6690 = t3984 - t712
        t6691 = t6690 * t183
        t6692 = t712 - t4248
        t6693 = t6692 * t183
        t6699 = -t1327 * (t6421 / 0.2E1 + t6425 / 0.2E1) / 0.6E1 + (t644
     #2 - t6454) * t183 + (t1659 - t6495) * t94 - t1327 * (t6503 / 0.2E1
     # + t6507 / 0.2E1) / 0.6E1 - t1701 * (t1848 / 0.2E1 + t6533 / 0.2E1
     #) / 0.6E1 - t1228 * ((t6544 - t6549) * t236 + (t6553 - t6555) * t2
     #36) / 0.24E2 - t1327 * (t6575 / 0.2E1 + t6587 / 0.2E1) / 0.6E1 - t
     #1228 * (t6606 / 0.2E1 + t6618 / 0.2E1) / 0.6E1 - t1701 * (t6643 / 
     #0.2E1 + t6655 / 0.2E1) / 0.6E1 + t592 + t735 - t1228 * (t1696 / 0.
     #2E1 + t6671 / 0.2E1) / 0.6E1 - t1327 * ((t6682 - t6687) * t183 + (
     #t6691 - t6693) * t183) / 0.24E2 + t845 + t860
        t6701 = (t571 - t6634) * t94
        t6702 = t1713 - t6701
        t6703 = t6702 * t94
        t6704 = t568 * t6703
        t6709 = t4 * (t124 / 0.2E1 + t6486 / 0.2E1)
        t6710 = t6709 * t6634
        t6712 = (t572 - t6710) * t94
        t6713 = t574 - t6712
        t6714 = t6713 * t94
        t6721 = (t5247 - t790) * t236
        t6723 = (t790 - t827) * t236
        t6725 = (t6721 - t6723) * t236
        t6727 = (t827 - t5445) * t236
        t6729 = (t6723 - t6727) * t236
        t6735 = (t580 - t6517) * t94
        t6738 = (t311 / 0.2E1 - t6735 / 0.2E1) * t94
        t6351 = (t1973 - t6738) * t94
        t6742 = t629 * t6351
        t6745 = t214 * t6262
        t6747 = (t6742 - t6745) * t183
        t6749 = (t583 - t6521) * t94
        t6752 = (t354 / 0.2E1 - t6749 / 0.2E1) * t94
        t6357 = (t1990 - t6752) * t94
        t6756 = t669 * t6357
        t6758 = (t6745 - t6756) * t183
        t6766 = t6459 * t6475 + t6472 * t6466 + t6470 * t6462
        t6768 = (t6623 - t6518) * t236
        t6770 = (t6518 - t6644) * t236
        t6772 = t6768 / 0.2E1 + t6770 / 0.2E1
        t6366 = t6512 * t6766
        t6774 = t6366 * t6772
        t6776 = (t606 - t6774) * t94
        t6778 = (t608 - t6776) * t94
        t6780 = (t2049 - t6778) * t94
        t6786 = (t5262 - t844) * t236
        t6788 = (t844 - t859) * t236
        t6790 = (t6786 - t6788) * t236
        t6792 = (t859 - t5460) * t236
        t6794 = (t6788 - t6792) * t236
        t6799 = t865 / 0.2E1
        t6800 = t870 / 0.2E1
        t6804 = (t870 - t879) * t236
        t6809 = t6799 + t6800 - dz * ((t5268 - t865) * t236 / 0.2E1 - t6
     #804 / 0.2E1) / 0.8E1
        t6810 = t4 * t6809
        t6811 = t6810 * t269
        t6812 = t879 / 0.2E1
        t6814 = (t865 - t870) * t236
        t6821 = t6800 + t6812 - dz * (t6814 / 0.2E1 - (t879 - t5466) * t
     #236 / 0.2E1) / 0.8E1
        t6822 = t4 * t6821
        t6823 = t6822 * t272
        t6828 = (t3912 / 0.2E1 - t585 / 0.2E1) * t183
        t6831 = (t582 / 0.2E1 - t4176 / 0.2E1) * t183
        t6396 = (t6828 - t6831) * t183
        t6835 = t573 * t6396
        t6837 = (t1422 - t6835) * t94
        t6842 = t752 + t791 + t828 - t1701 * ((t1716 - t6704) * t94 + (t
     #1728 - t6714) * t94) / 0.24E2 + t609 + t650 + t687 - t1228 * (t672
     #5 / 0.2E1 + t6729 / 0.2E1) / 0.6E1 - t1701 * (t6747 / 0.2E1 + t675
     #8 / 0.2E1) / 0.6E1 - t1701 * (t2051 / 0.2E1 + t6780 / 0.2E1) / 0.6
     #E1 - t1228 * (t6790 / 0.2E1 + t6794 / 0.2E1) / 0.6E1 + (t6811 - t6
     #823) * t236 + t228 - t1327 * (t1424 / 0.2E1 + t6837 / 0.2E1) / 0.6
     #E1 + t279
        t6845 = dt * (t6699 + t6842) * t56
        t6846 = t1227 * t6845
        t6847 = t147 / 0.2E1
        t6848 = ut(t6458,j,k,n)
        t6850 = (t145 - t6848) * t94
        t6852 = (t147 - t6850) * t94
        t6853 = t149 - t6852
        t6854 = t6853 * t94
        t6857 = t1701 * (t2070 / 0.2E1 + t6854 / 0.2E1)
        t6858 = t6857 / 0.6E1
        t6861 = dx * (t2062 + t6847 - t6858) / 0.2E1
        t6863 = (t2571 - t961) * t236
        t6865 = (t961 - t964) * t236
        t6866 = t6863 - t6865
        t6867 = t6866 * t236
        t6868 = t873 * t6867
        t6870 = (t964 - t2576) * t236
        t6871 = t6865 - t6870
        t6872 = t6871 * t236
        t6873 = t882 * t6872
        t6876 = t5271 * t2571
        t6878 = (t6876 - t1211) * t236
        t6879 = t6878 - t1214
        t6880 = t6879 * t236
        t6881 = t5469 * t2576
        t6883 = (t1212 - t6881) * t236
        t6884 = t1214 - t6883
        t6885 = t6884 * t236
        t6891 = ut(t6458,j,t233,n)
        t6893 = (t6891 - t6848) * t236
        t6894 = ut(t6458,j,t238,n)
        t6896 = (t6848 - t6894) * t236
        t6898 = t6893 / 0.2E1 + t6896 / 0.2E1
        t6900 = t6366 * t6898
        t6902 = (t1109 - t6900) * t94
        t6904 = (t1111 - t6902) * t94
        t6906 = (t2520 - t6904) * t94
        t6911 = ut(t6458,t180,k,n)
        t6913 = (t1087 - t6911) * t94
        t6916 = (t975 / 0.2E1 - t6913 / 0.2E1) * t94
        t6485 = (t2474 - t6916) * t94
        t6920 = t629 * t6485
        t6923 = (t139 / 0.2E1 - t6850 / 0.2E1) * t94
        t6489 = (t2180 - t6923) * t94
        t6927 = t214 * t6489
        t6929 = (t6920 - t6927) * t183
        t6930 = ut(t6458,t185,k,n)
        t6932 = (t1090 - t6930) * t94
        t6935 = (t990 / 0.2E1 - t6932 / 0.2E1) * t94
        t6497 = (t2492 - t6935) * t94
        t6939 = t669 * t6497
        t6941 = (t6927 - t6939) * t183
        t6946 = ut(i,t180,t1229,n)
        t6948 = (t6946 - t1139) * t236
        t6951 = (t6948 / 0.2E1 - t1144 / 0.2E1) * t236
        t6952 = ut(i,t180,t1277,n)
        t6954 = (t1142 - t6952) * t236
        t6957 = (t1141 / 0.2E1 - t6954 / 0.2E1) * t236
        t6510 = (t6951 - t6957) * t236
        t6961 = t708 * t6510
        t6964 = t715 * t2387
        t6966 = (t6961 - t6964) * t183
        t6967 = ut(i,t185,t1229,n)
        t6969 = (t6967 - t1154) * t236
        t6972 = (t6969 / 0.2E1 - t1159 / 0.2E1) * t236
        t6973 = ut(i,t185,t1277,n)
        t6975 = (t1157 - t6973) * t236
        t6978 = (t1156 / 0.2E1 - t6975 / 0.2E1) * t236
        t6526 = (t6972 - t6978) * t236
        t6982 = t731 * t6526
        t6984 = (t6964 - t6982) * t183
        t6989 = ut(i,t1328,t233,n)
        t6991 = (t6989 - t1139) * t183
        t6994 = (t6991 / 0.2E1 - t1190 / 0.2E1) * t183
        t6995 = ut(i,t1335,t233,n)
        t6997 = (t1154 - t6995) * t183
        t7000 = (t1188 / 0.2E1 - t6997 / 0.2E1) * t183
        t6538 = (t6994 - t7000) * t183
        t7004 = t822 * t6538
        t7007 = t715 * t2399
        t7009 = (t7004 - t7007) * t236
        t7010 = ut(i,t1328,t238,n)
        t7012 = (t7010 - t1142) * t183
        t7015 = (t7012 / 0.2E1 - t1203 / 0.2E1) * t183
        t7016 = ut(i,t1335,t238,n)
        t7018 = (t1157 - t7016) * t183
        t7021 = (t1201 / 0.2E1 - t7018 / 0.2E1) * t183
        t6559 = (t7015 - t7021) * t183
        t7025 = t837 * t6559
        t7027 = (t7007 - t7025) * t236
        t7032 = ut(t96,t1328,k,n)
        t7034 = (t7032 - t1087) * t183
        t7037 = (t7034 / 0.2E1 - t1092 / 0.2E1) * t183
        t7038 = ut(t96,t1335,k,n)
        t7040 = (t1090 - t7038) * t183
        t7043 = (t1089 / 0.2E1 - t7040 / 0.2E1) * t183
        t6571 = (t7037 - t7043) * t183
        t7047 = t573 * t6571
        t7049 = (t2622 - t7047) * t94
        t7054 = t6494 * t147
        t7057 = ut(t96,j,t1229,n)
        t7059 = (t2210 - t7057) * t94
        t7061 = t2212 / 0.2E1 + t7059 / 0.2E1
        t7063 = t4872 * t7061
        t7065 = (t7063 - t1172) * t236
        t7067 = (t7065 - t1176) * t236
        t7069 = (t1176 - t1185) * t236
        t7071 = (t7067 - t7069) * t236
        t7072 = ut(t96,j,t1277,n)
        t7074 = (t2229 - t7072) * t94
        t7076 = t2231 / 0.2E1 + t7074 / 0.2E1
        t7078 = t5046 * t7076
        t7080 = (t1183 - t7078) * t236
        t7082 = (t1185 - t7080) * t236
        t7084 = (t7069 - t7082) * t236
        t7089 = -t1228 * ((t6868 - t6873) * t236 + (t6880 - t6885) * t23
     #6) / 0.24E2 - t1701 * (t2522 / 0.2E1 + t6906 / 0.2E1) / 0.6E1 - t1
     #701 * (t6929 / 0.2E1 + t6941 / 0.2E1) / 0.6E1 - t1228 * (t6966 / 0
     #.2E1 + t6984 / 0.2E1) / 0.6E1 + t1134 + t1177 + t1186 + t1199 + t1
     #210 - t1327 * (t7009 / 0.2E1 + t7027 / 0.2E1) / 0.6E1 + t935 - t13
     #27 * (t2624 / 0.2E1 + t7049 / 0.2E1) / 0.6E1 + (t2297 - t7054) * t
     #94 - t1228 * (t7071 / 0.2E1 + t7084 / 0.2E1) / 0.6E1 + t1153
        t7091 = (t2610 - t925) * t183
        t7093 = (t925 - t928) * t183
        t7094 = t7091 - t7093
        t7095 = t7094 * t183
        t7096 = t700 * t7095
        t7098 = (t928 - t2615) * t183
        t7099 = t7093 - t7098
        t7100 = t7099 * t183
        t7101 = t709 * t7100
        t7104 = t3981 * t2610
        t7106 = (t7104 - t1135) * t183
        t7107 = t7106 - t1138
        t7108 = t7107 * t183
        t7109 = t4245 * t2615
        t7111 = (t1136 - t7109) * t183
        t7112 = t1138 - t7111
        t7113 = t7112 * t183
        t7120 = (t6946 - t2210) * t183
        t7122 = (t2210 - t6967) * t183
        t7124 = t7120 / 0.2E1 + t7122 / 0.2E1
        t7126 = t4882 * t7124
        t7128 = (t7126 - t1194) * t236
        t7130 = (t7128 - t1198) * t236
        t7132 = (t1198 - t1209) * t236
        t7134 = (t7130 - t7132) * t236
        t7136 = (t6952 - t2229) * t183
        t7138 = (t2229 - t6973) * t183
        t7140 = t7136 / 0.2E1 + t7138 / 0.2E1
        t7142 = t5059 * t7140
        t7144 = (t1207 - t7142) * t236
        t7146 = (t1209 - t7144) * t236
        t7148 = (t7132 - t7146) * t236
        t7153 = t6810 * t961
        t7154 = t6822 * t964
        t7157 = t568 * t6854
        t7160 = t6709 * t6850
        t7162 = (t1084 - t7160) * t94
        t7163 = t1086 - t7162
        t7164 = t7163 * t94
        t7171 = (t7057 - t1100) * t236
        t7174 = (t7171 / 0.2E1 - t1105 / 0.2E1) * t236
        t7176 = (t1103 - t7072) * t236
        t7179 = (t1102 / 0.2E1 - t7176 / 0.2E1) * t236
        t6694 = (t7174 - t7179) * t236
        t7183 = t590 * t6694
        t7185 = (t2583 - t7183) * t94
        t7191 = (t6989 - t2127) * t236
        t7193 = (t2127 - t7010) * t236
        t7195 = t7191 / 0.2E1 + t7193 / 0.2E1
        t7197 = t3719 * t7195
        t7199 = (t7197 - t1148) * t183
        t7201 = (t7199 - t1152) * t183
        t7203 = (t1152 - t1165) * t183
        t7205 = (t7201 - t7203) * t183
        t7207 = (t6995 - t2145) * t236
        t7209 = (t2145 - t7016) * t236
        t7211 = t7207 / 0.2E1 + t7209 / 0.2E1
        t7213 = t3962 * t7211
        t7215 = (t1163 - t7213) * t183
        t7217 = (t1165 - t7215) * t183
        t7219 = (t7203 - t7217) * t183
        t7225 = (t1100 - t6891) * t94
        t7228 = (t1033 / 0.2E1 - t7225 / 0.2E1) * t94
        t6724 = (t2170 - t7228) * t94
        t7232 = t771 * t6724
        t7235 = t265 * t6489
        t7237 = (t7232 - t7235) * t236
        t7239 = (t1103 - t6894) * t94
        t7242 = (t1046 / 0.2E1 - t7239 / 0.2E1) * t94
        t6732 = (t2195 - t7242) * t94
        t7246 = t809 * t6732
        t7248 = (t7235 - t7246) * t236
        t7254 = (t2127 - t7032) * t94
        t7256 = t2129 / 0.2E1 + t7254 / 0.2E1
        t7258 = t3688 * t7256
        t7260 = (t7258 - t1118) * t183
        t7262 = (t7260 - t1124) * t183
        t7264 = (t1124 - t1133) * t183
        t7266 = (t7262 - t7264) * t183
        t7268 = (t2145 - t7038) * t94
        t7270 = t2147 / 0.2E1 + t7268 / 0.2E1
        t7272 = t3941 * t7270
        t7274 = (t1131 - t7272) * t183
        t7276 = (t1133 - t7274) * t183
        t7278 = (t7264 - t7276) * t183
        t7283 = t6441 * t925
        t7284 = t6453 * t928
        t7288 = (t6911 - t6848) * t183
        t7290 = (t6848 - t6930) * t183
        t7292 = t7288 / 0.2E1 + t7290 / 0.2E1
        t7294 = t6214 * t7292
        t7296 = (t1096 - t7294) * t94
        t7298 = (t1098 - t7296) * t94
        t7300 = (t2544 - t7298) * t94
        t7305 = t1166 - t1327 * ((t7096 - t7101) * t183 + (t7108 - t7113
     #) * t183) / 0.24E2 - t1228 * (t7134 / 0.2E1 + t7148 / 0.2E1) / 0.6
     #E1 + (t7153 - t7154) * t236 + t971 - t1701 * ((t2422 - t7157) * t9
     #4 + (t2430 - t7164) * t94) / 0.24E2 + t1112 + t1125 - t1228 * (t25
     #85 / 0.2E1 + t7185 / 0.2E1) / 0.6E1 - t1327 * (t7205 / 0.2E1 + t72
     #19 / 0.2E1) / 0.6E1 - t1701 * (t7237 / 0.2E1 + t7248 / 0.2E1) / 0.
     #6E1 - t1327 * (t7266 / 0.2E1 + t7278 / 0.2E1) / 0.6E1 + (t7283 - t
     #7284) * t183 - t1701 * (t2546 / 0.2E1 + t7300 / 0.2E1) / 0.6E1 + t
     #1099
        t7308 = t161 * (t7089 + t7305) * t56
        t7310 = t2079 * t7308 / 0.2E1
        t7311 = t6529 / 0.2E1
        t7312 = t6776 / 0.2E1
        t7314 = t639 / 0.2E1 + t6735 / 0.2E1
        t7316 = t3636 * t7314
        t7318 = t571 / 0.2E1 + t6634 / 0.2E1
        t7320 = t573 * t7318
        t7322 = (t7316 - t7320) * t183
        t7323 = t7322 / 0.2E1
        t7325 = t680 / 0.2E1 + t6749 / 0.2E1
        t7327 = t3883 * t7325
        t7329 = (t7320 - t7327) * t183
        t7330 = t7329 / 0.2E1
        t7331 = t3880 ** 2
        t7332 = t3872 ** 2
        t7333 = t3876 ** 2
        t7335 = t3893 * (t7331 + t7332 + t7333)
        t7336 = t106 ** 2
        t7337 = t98 ** 2
        t7338 = t102 ** 2
        t7340 = t119 * (t7336 + t7337 + t7338)
        t7343 = t4 * (t7335 / 0.2E1 + t7340 / 0.2E1)
        t7344 = t7343 * t582
        t7345 = t4144 ** 2
        t7346 = t4136 ** 2
        t7347 = t4140 ** 2
        t7349 = t4157 * (t7345 + t7346 + t7347)
        t7352 = t4 * (t7340 / 0.2E1 + t7349 / 0.2E1)
        t7353 = t7352 * t585
        t7355 = (t7344 - t7353) * t183
        t6869 = t3905 * (t3880 * t3887 + t3872 * t3878 + t3876 * t3874)
        t7361 = t6869 * t3931
        t6882 = t575 * (t106 * t113 + t98 * t104 + t102 * t100)
        t7367 = t6882 * t604
        t7369 = (t7361 - t7367) * t183
        t7370 = t7369 / 0.2E1
        t6890 = t4169 * (t4144 * t4151 + t4136 * t4142 + t4140 * t4138)
        t7376 = t6890 * t4195
        t7378 = (t7367 - t7376) * t183
        t7379 = t7378 / 0.2E1
        t7381 = t782 / 0.2E1 + t6625 / 0.2E1
        t7383 = t4813 * t7381
        t7385 = t590 * t7318
        t7387 = (t7383 - t7385) * t236
        t7388 = t7387 / 0.2E1
        t7390 = t821 / 0.2E1 + t6646 / 0.2E1
        t7392 = t4953 * t7390
        t7394 = (t7385 - t7392) * t236
        t7395 = t7394 / 0.2E1
        t6908 = t5115 * (t5090 * t5097 + t5082 * t5088 + t5086 * t5084)
        t7401 = t6908 * t5125
        t7403 = t6882 * t587
        t7405 = (t7401 - t7403) * t236
        t7406 = t7405 / 0.2E1
        t6915 = t5313 * (t5288 * t5295 + t5280 * t5286 + t5284 * t5282)
        t7412 = t6915 * t5323
        t7414 = (t7403 - t7412) * t236
        t7415 = t7414 / 0.2E1
        t7416 = t5097 ** 2
        t7417 = t5088 ** 2
        t7418 = t5084 ** 2
        t7420 = t5103 * (t7416 + t7417 + t7418)
        t7421 = t113 ** 2
        t7422 = t104 ** 2
        t7423 = t100 ** 2
        t7425 = t119 * (t7421 + t7422 + t7423)
        t7428 = t4 * (t7420 / 0.2E1 + t7425 / 0.2E1)
        t7429 = t7428 * t599
        t7430 = t5295 ** 2
        t7431 = t5286 ** 2
        t7432 = t5282 ** 2
        t7434 = t5301 * (t7430 + t7431 + t7432)
        t7437 = t4 * (t7425 / 0.2E1 + t7434 / 0.2E1)
        t7438 = t7437 * t602
        t7440 = (t7429 - t7438) * t236
        t7441 = t6712 + t592 + t7311 + t609 + t7312 + t7323 + t7330 + t7
     #355 + t7370 + t7379 + t7388 + t7395 + t7406 + t7415 + t7440
        t7442 = t7441 * t118
        t7443 = t887 - t7442
        t7444 = t7443 * t94
        t7446 = t2910 / 0.2E1 + t7444 / 0.2E1
        t7447 = t2635 * t7446
        t7449 = t1227 * t7447 / 0.2E1
        t7455 = t1701 * (t149 - dx * (t2070 - t6854) / 0.12E2) / 0.12E2
        t7456 = t568 * t7444
        t7459 = rx(t6458,t180,k,0,0)
        t7460 = rx(t6458,t180,k,1,1)
        t7462 = rx(t6458,t180,k,2,2)
        t7464 = rx(t6458,t180,k,1,2)
        t7466 = rx(t6458,t180,k,2,1)
        t7468 = rx(t6458,t180,k,1,0)
        t7470 = rx(t6458,t180,k,0,2)
        t7472 = rx(t6458,t180,k,0,1)
        t7475 = rx(t6458,t180,k,2,0)
        t7480 = t7459 * t7460 * t7462 - t7459 * t7464 * t7466 + t7468 * 
     #t7466 * t7470 - t7468 * t7472 * t7462 + t7475 * t7472 * t7464 - t7
     #475 * t7460 * t7470
        t7481 = 0.1E1 / t7480
        t7482 = t7459 ** 2
        t7483 = t7472 ** 2
        t7484 = t7470 ** 2
        t7486 = t7481 * (t7482 + t7483 + t7484)
        t7489 = t4 * (t3898 / 0.2E1 + t7486 / 0.2E1)
        t7490 = t7489 * t6735
        t7492 = (t3902 - t7490) * t94
        t7493 = t4 * t7481
        t7498 = u(t6458,t1328,k,n)
        t7500 = (t7498 - t6517) * t183
        t7502 = t7500 / 0.2E1 + t6520 / 0.2E1
        t6985 = t7493 * (t7459 * t7468 + t7472 * t7460 + t7470 * t7464)
        t7504 = t6985 * t7502
        t7506 = (t3916 - t7504) * t94
        t7507 = t7506 / 0.2E1
        t7512 = u(t6458,t180,t233,n)
        t7514 = (t7512 - t6517) * t236
        t7515 = u(t6458,t180,t238,n)
        t7517 = (t6517 - t7515) * t236
        t7519 = t7514 / 0.2E1 + t7517 / 0.2E1
        t6998 = t7493 * (t7459 * t7475 + t7472 * t7466 + t7470 * t7462)
        t7521 = t6998 * t7519
        t7523 = (t3933 - t7521) * t94
        t7524 = t7523 / 0.2E1
        t7525 = rx(t96,t1328,k,0,0)
        t7526 = rx(t96,t1328,k,1,1)
        t7528 = rx(t96,t1328,k,2,2)
        t7530 = rx(t96,t1328,k,1,2)
        t7532 = rx(t96,t1328,k,2,1)
        t7534 = rx(t96,t1328,k,1,0)
        t7536 = rx(t96,t1328,k,0,2)
        t7538 = rx(t96,t1328,k,0,1)
        t7541 = rx(t96,t1328,k,2,0)
        t7546 = t7525 * t7526 * t7528 - t7525 * t7530 * t7532 + t7534 * 
     #t7532 * t7536 - t7534 * t7538 * t7528 + t7541 * t7538 * t7530 - t7
     #541 * t7526 * t7536
        t7547 = 0.1E1 / t7546
        t7548 = t4 * t7547
        t7554 = (t3910 - t7498) * t94
        t7556 = t3966 / 0.2E1 + t7554 / 0.2E1
        t7031 = t7548 * (t7525 * t7534 + t7538 * t7526 + t7536 * t7530)
        t7558 = t7031 * t7556
        t7560 = (t7558 - t7316) * t183
        t7561 = t7560 / 0.2E1
        t7562 = t7534 ** 2
        t7563 = t7526 ** 2
        t7564 = t7530 ** 2
        t7566 = t7547 * (t7562 + t7563 + t7564)
        t7569 = t4 * (t7566 / 0.2E1 + t7335 / 0.2E1)
        t7570 = t7569 * t3912
        t7572 = (t7570 - t7344) * t183
        t7577 = u(t96,t1328,t233,n)
        t7579 = (t7577 - t3910) * t236
        t7580 = u(t96,t1328,t238,n)
        t7582 = (t3910 - t7580) * t236
        t7584 = t7579 / 0.2E1 + t7582 / 0.2E1
        t7052 = t7548 * (t7534 * t7541 + t7526 * t7532 + t7530 * t7528)
        t7586 = t7052 * t7584
        t7588 = (t7586 - t7361) * t183
        t7589 = t7588 / 0.2E1
        t7590 = rx(t96,t180,t233,0,0)
        t7591 = rx(t96,t180,t233,1,1)
        t7593 = rx(t96,t180,t233,2,2)
        t7595 = rx(t96,t180,t233,1,2)
        t7597 = rx(t96,t180,t233,2,1)
        t7599 = rx(t96,t180,t233,1,0)
        t7601 = rx(t96,t180,t233,0,2)
        t7603 = rx(t96,t180,t233,0,1)
        t7606 = rx(t96,t180,t233,2,0)
        t7611 = t7590 * t7591 * t7593 - t7590 * t7595 * t7597 + t7599 * 
     #t7597 * t7601 - t7599 * t7603 * t7593 + t7606 * t7603 * t7595 - t7
     #606 * t7591 * t7601
        t7612 = 0.1E1 / t7611
        t7613 = t4 * t7612
        t7619 = (t3924 - t7512) * t94
        t7621 = t4031 / 0.2E1 + t7619 / 0.2E1
        t7087 = t7613 * (t7590 * t7606 + t7603 * t7597 + t7601 * t7593)
        t7623 = t7087 * t7621
        t7625 = t3651 * t7314
        t7628 = (t7623 - t7625) * t236 / 0.2E1
        t7629 = rx(t96,t180,t238,0,0)
        t7630 = rx(t96,t180,t238,1,1)
        t7632 = rx(t96,t180,t238,2,2)
        t7634 = rx(t96,t180,t238,1,2)
        t7636 = rx(t96,t180,t238,2,1)
        t7638 = rx(t96,t180,t238,1,0)
        t7640 = rx(t96,t180,t238,0,2)
        t7642 = rx(t96,t180,t238,0,1)
        t7645 = rx(t96,t180,t238,2,0)
        t7650 = t7629 * t7630 * t7632 - t7629 * t7634 * t7636 + t7638 * 
     #t7636 * t7640 - t7638 * t7642 * t7632 + t7645 * t7642 * t7634 - t7
     #645 * t7630 * t7640
        t7651 = 0.1E1 / t7650
        t7652 = t4 * t7651
        t7658 = (t3927 - t7515) * t94
        t7660 = t4070 / 0.2E1 + t7658 / 0.2E1
        t7131 = t7652 * (t7629 * t7645 + t7642 * t7636 + t7640 * t7632)
        t7662 = t7131 * t7660
        t7665 = (t7625 - t7662) * t236 / 0.2E1
        t7671 = (t7577 - t3924) * t183
        t7673 = t7671 / 0.2E1 + t5121 / 0.2E1
        t7147 = t7613 * (t7599 * t7606 + t7591 * t7597 + t7595 * t7593)
        t7675 = t7147 * t7673
        t7677 = t6869 * t3914
        t7680 = (t7675 - t7677) * t236 / 0.2E1
        t7686 = (t7580 - t3927) * t183
        t7688 = t7686 / 0.2E1 + t5319 / 0.2E1
        t7159 = t7652 * (t7638 * t7645 + t7630 * t7636 + t7634 * t7632)
        t7690 = t7159 * t7688
        t7693 = (t7677 - t7690) * t236 / 0.2E1
        t7694 = t7606 ** 2
        t7695 = t7597 ** 2
        t7696 = t7593 ** 2
        t7698 = t7612 * (t7694 + t7695 + t7696)
        t7699 = t3887 ** 2
        t7700 = t3878 ** 2
        t7701 = t3874 ** 2
        t7703 = t3893 * (t7699 + t7700 + t7701)
        t7706 = t4 * (t7698 / 0.2E1 + t7703 / 0.2E1)
        t7707 = t7706 * t3926
        t7708 = t7645 ** 2
        t7709 = t7636 ** 2
        t7710 = t7632 ** 2
        t7712 = t7651 * (t7708 + t7709 + t7710)
        t7715 = t4 * (t7703 / 0.2E1 + t7712 / 0.2E1)
        t7716 = t7715 * t3929
        t7719 = t7492 + t3919 + t7507 + t3936 + t7524 + t7561 + t7323 + 
     #t7572 + t7589 + t7370 + t7628 + t7665 + t7680 + t7693 + (t7707 - t
     #7716) * t236
        t7720 = t7719 * t3892
        t7722 = (t7720 - t7442) * t183
        t7723 = rx(t6458,t185,k,0,0)
        t7724 = rx(t6458,t185,k,1,1)
        t7726 = rx(t6458,t185,k,2,2)
        t7728 = rx(t6458,t185,k,1,2)
        t7730 = rx(t6458,t185,k,2,1)
        t7732 = rx(t6458,t185,k,1,0)
        t7734 = rx(t6458,t185,k,0,2)
        t7736 = rx(t6458,t185,k,0,1)
        t7739 = rx(t6458,t185,k,2,0)
        t7744 = t7723 * t7724 * t7726 - t7723 * t7728 * t7730 + t7732 * 
     #t7730 * t7734 - t7732 * t7736 * t7726 + t7739 * t7736 * t7728 - t7
     #739 * t7724 * t7734
        t7745 = 0.1E1 / t7744
        t7746 = t7723 ** 2
        t7747 = t7736 ** 2
        t7748 = t7734 ** 2
        t7750 = t7745 * (t7746 + t7747 + t7748)
        t7753 = t4 * (t4162 / 0.2E1 + t7750 / 0.2E1)
        t7754 = t7753 * t6749
        t7756 = (t4166 - t7754) * t94
        t7757 = t4 * t7745
        t7762 = u(t6458,t1335,k,n)
        t7764 = (t6521 - t7762) * t183
        t7766 = t6523 / 0.2E1 + t7764 / 0.2E1
        t7218 = t7757 * (t7723 * t7732 + t7736 * t7724 + t7734 * t7728)
        t7768 = t7218 * t7766
        t7770 = (t4180 - t7768) * t94
        t7771 = t7770 / 0.2E1
        t7776 = u(t6458,t185,t233,n)
        t7778 = (t7776 - t6521) * t236
        t7779 = u(t6458,t185,t238,n)
        t7781 = (t6521 - t7779) * t236
        t7783 = t7778 / 0.2E1 + t7781 / 0.2E1
        t7230 = t7757 * (t7723 * t7739 + t7730 * t7736 + t7734 * t7726)
        t7785 = t7230 * t7783
        t7787 = (t4197 - t7785) * t94
        t7788 = t7787 / 0.2E1
        t7789 = rx(t96,t1335,k,0,0)
        t7790 = rx(t96,t1335,k,1,1)
        t7792 = rx(t96,t1335,k,2,2)
        t7794 = rx(t96,t1335,k,1,2)
        t7796 = rx(t96,t1335,k,2,1)
        t7798 = rx(t96,t1335,k,1,0)
        t7800 = rx(t96,t1335,k,0,2)
        t7802 = rx(t96,t1335,k,0,1)
        t7805 = rx(t96,t1335,k,2,0)
        t7810 = t7789 * t7790 * t7792 - t7789 * t7794 * t7796 + t7798 * 
     #t7796 * t7800 - t7798 * t7802 * t7792 + t7805 * t7802 * t7794 - t7
     #805 * t7790 * t7800
        t7811 = 0.1E1 / t7810
        t7812 = t4 * t7811
        t7818 = (t4174 - t7762) * t94
        t7820 = t4230 / 0.2E1 + t7818 / 0.2E1
        t7261 = t7812 * (t7789 * t7798 + t7802 * t7790 + t7800 * t7794)
        t7822 = t7261 * t7820
        t7824 = (t7327 - t7822) * t183
        t7825 = t7824 / 0.2E1
        t7826 = t7798 ** 2
        t7827 = t7790 ** 2
        t7828 = t7794 ** 2
        t7830 = t7811 * (t7826 + t7827 + t7828)
        t7833 = t4 * (t7349 / 0.2E1 + t7830 / 0.2E1)
        t7834 = t7833 * t4176
        t7836 = (t7353 - t7834) * t183
        t7841 = u(t96,t1335,t233,n)
        t7843 = (t7841 - t4174) * t236
        t7844 = u(t96,t1335,t238,n)
        t7846 = (t4174 - t7844) * t236
        t7848 = t7843 / 0.2E1 + t7846 / 0.2E1
        t7282 = t7812 * (t7805 * t7798 + t7790 * t7796 + t7794 * t7792)
        t7850 = t7282 * t7848
        t7852 = (t7376 - t7850) * t183
        t7853 = t7852 / 0.2E1
        t7854 = rx(t96,t185,t233,0,0)
        t7855 = rx(t96,t185,t233,1,1)
        t7857 = rx(t96,t185,t233,2,2)
        t7859 = rx(t96,t185,t233,1,2)
        t7861 = rx(t96,t185,t233,2,1)
        t7863 = rx(t96,t185,t233,1,0)
        t7865 = rx(t96,t185,t233,0,2)
        t7867 = rx(t96,t185,t233,0,1)
        t7870 = rx(t96,t185,t233,2,0)
        t7875 = t7854 * t7855 * t7857 - t7854 * t7859 * t7861 + t7863 * 
     #t7861 * t7865 - t7863 * t7867 * t7857 + t7870 * t7867 * t7859 - t7
     #870 * t7855 * t7865
        t7876 = 0.1E1 / t7875
        t7877 = t4 * t7876
        t7883 = (t4188 - t7776) * t94
        t7885 = t4295 / 0.2E1 + t7883 / 0.2E1
        t7317 = t7877 * (t7854 * t7870 + t7867 * t7861 + t7865 * t7857)
        t7887 = t7317 * t7885
        t7889 = t3899 * t7325
        t7892 = (t7887 - t7889) * t236 / 0.2E1
        t7893 = rx(t96,t185,t238,0,0)
        t7894 = rx(t96,t185,t238,1,1)
        t7896 = rx(t96,t185,t238,2,2)
        t7898 = rx(t96,t185,t238,1,2)
        t7900 = rx(t96,t185,t238,2,1)
        t7902 = rx(t96,t185,t238,1,0)
        t7904 = rx(t96,t185,t238,0,2)
        t7906 = rx(t96,t185,t238,0,1)
        t7909 = rx(t96,t185,t238,2,0)
        t7914 = t7893 * t7894 * t7896 - t7893 * t7898 * t7900 + t7902 * 
     #t7900 * t7904 - t7902 * t7906 * t7896 + t7909 * t7906 * t7898 - t7
     #909 * t7894 * t7904
        t7915 = 0.1E1 / t7914
        t7916 = t4 * t7915
        t7922 = (t4191 - t7779) * t94
        t7924 = t4334 / 0.2E1 + t7922 / 0.2E1
        t7364 = t7916 * (t7893 * t7909 + t7906 * t7900 + t7904 * t7896)
        t7926 = t7364 * t7924
        t7929 = (t7889 - t7926) * t236 / 0.2E1
        t7935 = (t4188 - t7841) * t183
        t7937 = t5123 / 0.2E1 + t7935 / 0.2E1
        t7380 = t7877 * (t7863 * t7870 + t7855 * t7861 + t7859 * t7857)
        t7939 = t7380 * t7937
        t7941 = t6890 * t4178
        t7944 = (t7939 - t7941) * t236 / 0.2E1
        t7950 = (t4191 - t7844) * t183
        t7952 = t5321 / 0.2E1 + t7950 / 0.2E1
        t7397 = t7916 * (t7902 * t7909 + t7894 * t7900 + t7898 * t7896)
        t7954 = t7397 * t7952
        t7957 = (t7941 - t7954) * t236 / 0.2E1
        t7958 = t7870 ** 2
        t7959 = t7861 ** 2
        t7960 = t7857 ** 2
        t7962 = t7876 * (t7958 + t7959 + t7960)
        t7963 = t4151 ** 2
        t7964 = t4142 ** 2
        t7965 = t4138 ** 2
        t7967 = t4157 * (t7963 + t7964 + t7965)
        t7970 = t4 * (t7962 / 0.2E1 + t7967 / 0.2E1)
        t7971 = t7970 * t4190
        t7972 = t7909 ** 2
        t7973 = t7900 ** 2
        t7974 = t7896 ** 2
        t7976 = t7915 * (t7972 + t7973 + t7974)
        t7979 = t4 * (t7967 / 0.2E1 + t7976 / 0.2E1)
        t7980 = t7979 * t4193
        t7983 = t7756 + t4183 + t7771 + t4200 + t7788 + t7330 + t7825 + 
     #t7836 + t7379 + t7853 + t7892 + t7929 + t7944 + t7957 + (t7971 - t
     #7980) * t236
        t7984 = t7983 * t4156
        t7986 = (t7442 - t7984) * t183
        t7988 = t7722 / 0.2E1 + t7986 / 0.2E1
        t7990 = t573 * t7988
        t7993 = (t4402 - t7990) * t94 / 0.2E1
        t7994 = rx(t6458,j,t233,0,0)
        t7995 = rx(t6458,j,t233,1,1)
        t7997 = rx(t6458,j,t233,2,2)
        t7999 = rx(t6458,j,t233,1,2)
        t8001 = rx(t6458,j,t233,2,1)
        t8003 = rx(t6458,j,t233,1,0)
        t8005 = rx(t6458,j,t233,0,2)
        t8007 = rx(t6458,j,t233,0,1)
        t8010 = rx(t6458,j,t233,2,0)
        t8015 = t7994 * t7995 * t7997 - t7994 * t7999 * t8001 + t8003 * 
     #t8001 * t8005 - t8003 * t8007 * t7997 + t8010 * t8007 * t7999 - t8
     #010 * t7995 * t8005
        t8016 = 0.1E1 / t8015
        t8017 = t7994 ** 2
        t8018 = t8007 ** 2
        t8019 = t8005 ** 2
        t8021 = t8016 * (t8017 + t8018 + t8019)
        t8024 = t4 * (t5108 / 0.2E1 + t8021 / 0.2E1)
        t8025 = t8024 * t6625
        t8027 = (t5112 - t8025) * t94
        t8028 = t4 * t8016
        t8034 = (t7512 - t6623) * t183
        t8036 = (t6623 - t7776) * t183
        t8038 = t8034 / 0.2E1 + t8036 / 0.2E1
        t7479 = t8028 * (t7994 * t8003 + t8007 * t7995 + t8005 * t7999)
        t8040 = t7479 * t8038
        t8042 = (t5127 - t8040) * t94
        t8043 = t8042 / 0.2E1
        t8048 = u(t6458,j,t1229,n)
        t8050 = (t8048 - t6623) * t236
        t8052 = t8050 / 0.2E1 + t6768 / 0.2E1
        t7496 = t8028 * (t7994 * t8010 + t8007 * t8001 + t8005 * t7997)
        t8054 = t7496 * t8052
        t8056 = (t5141 - t8054) * t94
        t8057 = t8056 / 0.2E1
        t7508 = t7613 * (t7590 * t7599 + t7603 * t7591 + t7601 * t7595)
        t8063 = t7508 * t7621
        t8065 = t4804 * t7381
        t8068 = (t8063 - t8065) * t183 / 0.2E1
        t7520 = t7877 * (t7854 * t7863 + t7867 * t7855 + t7865 * t7859)
        t8074 = t7520 * t7885
        t8077 = (t8065 - t8074) * t183 / 0.2E1
        t8078 = t7599 ** 2
        t8079 = t7591 ** 2
        t8080 = t7595 ** 2
        t8082 = t7612 * (t8078 + t8079 + t8080)
        t8083 = t5090 ** 2
        t8084 = t5082 ** 2
        t8085 = t5086 ** 2
        t8087 = t5103 * (t8083 + t8084 + t8085)
        t8090 = t4 * (t8082 / 0.2E1 + t8087 / 0.2E1)
        t8091 = t8090 * t5121
        t8092 = t7863 ** 2
        t8093 = t7855 ** 2
        t8094 = t7859 ** 2
        t8096 = t7876 * (t8092 + t8093 + t8094)
        t8099 = t4 * (t8087 / 0.2E1 + t8096 / 0.2E1)
        t8100 = t8099 * t5123
        t8103 = u(t96,t180,t1229,n)
        t8105 = (t8103 - t3924) * t236
        t8107 = t8105 / 0.2E1 + t3926 / 0.2E1
        t8109 = t7147 * t8107
        t8111 = t6908 * t5139
        t8114 = (t8109 - t8111) * t183 / 0.2E1
        t8115 = u(t96,t185,t1229,n)
        t8117 = (t8115 - t4188) * t236
        t8119 = t8117 / 0.2E1 + t4190 / 0.2E1
        t8121 = t7380 * t8119
        t8124 = (t8111 - t8121) * t183 / 0.2E1
        t8125 = rx(t96,j,t1229,0,0)
        t8126 = rx(t96,j,t1229,1,1)
        t8128 = rx(t96,j,t1229,2,2)
        t8130 = rx(t96,j,t1229,1,2)
        t8132 = rx(t96,j,t1229,2,1)
        t8134 = rx(t96,j,t1229,1,0)
        t8136 = rx(t96,j,t1229,0,2)
        t8138 = rx(t96,j,t1229,0,1)
        t8141 = rx(t96,j,t1229,2,0)
        t8146 = t8125 * t8126 * t8128 - t8125 * t8130 * t8132 + t8132 * 
     #t8134 * t8136 - t8134 * t8138 * t8128 + t8141 * t8138 * t8130 - t8
     #141 * t8126 * t8136
        t8147 = 0.1E1 / t8146
        t8148 = t4 * t8147
        t8154 = (t5135 - t8048) * t94
        t8156 = t5241 / 0.2E1 + t8154 / 0.2E1
        t7596 = t8148 * (t8125 * t8141 + t8138 * t8132 + t8136 * t8128)
        t8158 = t7596 * t8156
        t8160 = (t8158 - t7383) * t236
        t8161 = t8160 / 0.2E1
        t8167 = (t8103 - t5135) * t183
        t8169 = (t5135 - t8115) * t183
        t8171 = t8167 / 0.2E1 + t8169 / 0.2E1
        t7609 = t8148 * (t8134 * t8141 + t8126 * t8132 + t8130 * t8128)
        t8173 = t7609 * t8171
        t8175 = (t8173 - t7401) * t236
        t8176 = t8175 / 0.2E1
        t8177 = t8141 ** 2
        t8178 = t8132 ** 2
        t8179 = t8128 ** 2
        t8181 = t8147 * (t8177 + t8178 + t8179)
        t8184 = t4 * (t8181 / 0.2E1 + t7420 / 0.2E1)
        t8185 = t8184 * t5137
        t8187 = (t8185 - t7429) * t236
        t8188 = t8027 + t5130 + t8043 + t5144 + t8057 + t8068 + t8077 + 
     #(t8091 - t8100) * t183 + t8114 + t8124 + t8161 + t7388 + t8176 + t
     #7406 + t8187
        t8189 = t8188 * t5102
        t8191 = (t8189 - t7442) * t236
        t8192 = rx(t6458,j,t238,0,0)
        t8193 = rx(t6458,j,t238,1,1)
        t8195 = rx(t6458,j,t238,2,2)
        t8197 = rx(t6458,j,t238,1,2)
        t8199 = rx(t6458,j,t238,2,1)
        t8201 = rx(t6458,j,t238,1,0)
        t8203 = rx(t6458,j,t238,0,2)
        t8205 = rx(t6458,j,t238,0,1)
        t8208 = rx(t6458,j,t238,2,0)
        t8213 = t8192 * t8193 * t8195 - t8192 * t8197 * t8199 + t8201 * 
     #t8199 * t8203 - t8201 * t8205 * t8195 + t8208 * t8205 * t8197 - t8
     #208 * t8193 * t8203
        t8214 = 0.1E1 / t8213
        t8215 = t8192 ** 2
        t8216 = t8205 ** 2
        t8217 = t8203 ** 2
        t8219 = t8214 * (t8215 + t8216 + t8217)
        t8222 = t4 * (t5306 / 0.2E1 + t8219 / 0.2E1)
        t8223 = t8222 * t6646
        t8225 = (t5310 - t8223) * t94
        t8226 = t4 * t8214
        t8232 = (t7515 - t6644) * t183
        t8234 = (t6644 - t7779) * t183
        t8236 = t8232 / 0.2E1 + t8234 / 0.2E1
        t7666 = t8226 * (t8192 * t8201 + t8205 * t8193 + t8203 * t8197)
        t8238 = t7666 * t8236
        t8240 = (t5325 - t8238) * t94
        t8241 = t8240 / 0.2E1
        t8246 = u(t6458,j,t1277,n)
        t8248 = (t6644 - t8246) * t236
        t8250 = t6770 / 0.2E1 + t8248 / 0.2E1
        t7678 = t8226 * (t8192 * t8208 + t8205 * t8199 + t8203 * t8195)
        t8252 = t7678 * t8250
        t8254 = (t5339 - t8252) * t94
        t8255 = t8254 / 0.2E1
        t7685 = t7652 * (t7629 * t7638 + t7642 * t7630 + t7640 * t7634)
        t8261 = t7685 * t7660
        t8263 = t4931 * t7390
        t8266 = (t8261 - t8263) * t183 / 0.2E1
        t7702 = t7916 * (t7893 * t7902 + t7906 * t7894 + t7904 * t7898)
        t8272 = t7702 * t7924
        t8275 = (t8263 - t8272) * t183 / 0.2E1
        t8276 = t7638 ** 2
        t8277 = t7630 ** 2
        t8278 = t7634 ** 2
        t8280 = t7651 * (t8276 + t8277 + t8278)
        t8281 = t5288 ** 2
        t8282 = t5280 ** 2
        t8283 = t5284 ** 2
        t8285 = t5301 * (t8281 + t8282 + t8283)
        t8288 = t4 * (t8280 / 0.2E1 + t8285 / 0.2E1)
        t8289 = t8288 * t5319
        t8290 = t7902 ** 2
        t8291 = t7894 ** 2
        t8292 = t7898 ** 2
        t8294 = t7915 * (t8290 + t8291 + t8292)
        t8297 = t4 * (t8285 / 0.2E1 + t8294 / 0.2E1)
        t8298 = t8297 * t5321
        t8301 = u(t96,t180,t1277,n)
        t8303 = (t3927 - t8301) * t236
        t8305 = t3929 / 0.2E1 + t8303 / 0.2E1
        t8307 = t7159 * t8305
        t8309 = t6915 * t5337
        t8312 = (t8307 - t8309) * t183 / 0.2E1
        t8313 = u(t96,t185,t1277,n)
        t8315 = (t4191 - t8313) * t236
        t8317 = t4193 / 0.2E1 + t8315 / 0.2E1
        t8319 = t7397 * t8317
        t8322 = (t8309 - t8319) * t183 / 0.2E1
        t8323 = rx(t96,j,t1277,0,0)
        t8324 = rx(t96,j,t1277,1,1)
        t8326 = rx(t96,j,t1277,2,2)
        t8328 = rx(t96,j,t1277,1,2)
        t8330 = rx(t96,j,t1277,2,1)
        t8332 = rx(t96,j,t1277,1,0)
        t8334 = rx(t96,j,t1277,0,2)
        t8336 = rx(t96,j,t1277,0,1)
        t8339 = rx(t96,j,t1277,2,0)
        t8344 = t8323 * t8324 * t8326 - t8323 * t8328 * t8330 + t8332 * 
     #t8330 * t8334 - t8332 * t8336 * t8326 + t8339 * t8336 * t8328 - t8
     #339 * t8324 * t8334
        t8345 = 0.1E1 / t8344
        t8346 = t4 * t8345
        t8352 = (t5333 - t8246) * t94
        t8354 = t5439 / 0.2E1 + t8352 / 0.2E1
        t7777 = t8346 * (t8323 * t8339 + t8336 * t8330 + t8334 * t8326)
        t8356 = t7777 * t8354
        t8358 = (t7392 - t8356) * t236
        t8359 = t8358 / 0.2E1
        t8365 = (t8301 - t5333) * t183
        t8367 = (t5333 - t8313) * t183
        t8369 = t8365 / 0.2E1 + t8367 / 0.2E1
        t7797 = t8346 * (t8332 * t8339 + t8324 * t8330 + t8328 * t8326)
        t8371 = t7797 * t8369
        t8373 = (t7412 - t8371) * t236
        t8374 = t8373 / 0.2E1
        t8375 = t8339 ** 2
        t8376 = t8330 ** 2
        t8377 = t8326 ** 2
        t8379 = t8345 * (t8375 + t8376 + t8377)
        t8382 = t4 * (t7434 / 0.2E1 + t8379 / 0.2E1)
        t8383 = t8382 * t5335
        t8385 = (t7438 - t8383) * t236
        t8386 = t8225 + t5328 + t8241 + t5342 + t8255 + t8266 + t8275 + 
     #(t8289 - t8298) * t183 + t8312 + t8322 + t7395 + t8359 + t7415 + t
     #8374 + t8385
        t8387 = t8386 * t5300
        t8389 = (t7442 - t8387) * t236
        t8391 = t8191 / 0.2E1 + t8389 / 0.2E1
        t8393 = t590 * t8391
        t8396 = (t5480 - t8393) * t94 / 0.2E1
        t8398 = (t4132 - t7720) * t94
        t8400 = t5487 / 0.2E1 + t8398 / 0.2E1
        t8402 = t629 * t8400
        t8404 = t214 * t7446
        t8407 = (t8402 - t8404) * t183 / 0.2E1
        t8409 = (t4396 - t7984) * t94
        t8411 = t5500 / 0.2E1 + t8409 / 0.2E1
        t8413 = t669 * t8411
        t8416 = (t8404 - t8413) * t183 / 0.2E1
        t8417 = t700 * t4134
        t8418 = t709 * t4398
        t8421 = t7590 ** 2
        t8422 = t7603 ** 2
        t8423 = t7601 ** 2
        t8425 = t7612 * (t8421 + t8422 + t8423)
        t8428 = t4 * (t5530 / 0.2E1 + t8425 / 0.2E1)
        t8429 = t8428 * t4031
        t8433 = t7508 * t7673
        t8436 = (t5545 - t8433) * t94 / 0.2E1
        t8438 = t7087 * t8107
        t8441 = (t5557 - t8438) * t94 / 0.2E1
        t8442 = rx(i,t1328,t233,0,0)
        t8443 = rx(i,t1328,t233,1,1)
        t8445 = rx(i,t1328,t233,2,2)
        t8447 = rx(i,t1328,t233,1,2)
        t8449 = rx(i,t1328,t233,2,1)
        t8451 = rx(i,t1328,t233,1,0)
        t8453 = rx(i,t1328,t233,0,2)
        t8455 = rx(i,t1328,t233,0,1)
        t8458 = rx(i,t1328,t233,2,0)
        t8463 = t8442 * t8443 * t8445 - t8442 * t8447 * t8449 + t8451 * 
     #t8449 * t8453 - t8451 * t8455 * t8445 + t8458 * t8455 * t8447 - t8
     #458 * t8443 * t8453
        t8464 = 0.1E1 / t8463
        t8465 = t4 * t8464
        t8471 = (t3989 - t7577) * t94
        t8473 = t5592 / 0.2E1 + t8471 / 0.2E1
        t7884 = t8465 * (t8442 * t8451 + t8455 * t8443 + t8453 * t8447)
        t8475 = t7884 * t8473
        t8477 = (t8475 - t5150) * t183
        t8478 = t8477 / 0.2E1
        t8479 = t8451 ** 2
        t8480 = t8443 ** 2
        t8481 = t8447 ** 2
        t8483 = t8464 * (t8479 + t8480 + t8481)
        t8486 = t4 * (t8483 / 0.2E1 + t5169 / 0.2E1)
        t8487 = t8486 * t4083
        t8489 = (t8487 - t5178) * t183
        t8494 = u(i,t1328,t1229,n)
        t8496 = (t8494 - t3989) * t236
        t8498 = t8496 / 0.2E1 + t3991 / 0.2E1
        t7907 = t8465 * (t8451 * t8458 + t8443 * t8449 + t8447 * t8445)
        t8500 = t7907 * t8498
        t8502 = (t8500 - t5196) * t183
        t8503 = t8502 / 0.2E1
        t8504 = rx(i,t180,t1229,0,0)
        t8505 = rx(i,t180,t1229,1,1)
        t8507 = rx(i,t180,t1229,2,2)
        t8509 = rx(i,t180,t1229,1,2)
        t8511 = rx(i,t180,t1229,2,1)
        t8513 = rx(i,t180,t1229,1,0)
        t8515 = rx(i,t180,t1229,0,2)
        t8517 = rx(i,t180,t1229,0,1)
        t8520 = rx(i,t180,t1229,2,0)
        t8525 = t8504 * t8505 * t8507 - t8504 * t8509 * t8511 + t8513 * 
     #t8511 * t8515 - t8513 * t8517 * t8507 + t8520 * t8517 * t8509 - t8
     #520 * t8505 * t8515
        t8526 = 0.1E1 / t8525
        t8527 = t4 * t8526
        t8533 = (t5190 - t8103) * t94
        t8535 = t5656 / 0.2E1 + t8533 / 0.2E1
        t7938 = t8527 * (t8504 * t8520 + t8517 * t8511 + t8515 * t8507)
        t8537 = t7938 * t8535
        t8539 = (t8537 - t4035) * t236
        t8540 = t8539 / 0.2E1
        t8546 = (t8494 - t5190) * t183
        t8548 = t8546 / 0.2E1 + t5254 / 0.2E1
        t7949 = t8527 * (t8513 * t8520 + t8505 * t8511 + t8509 * t8507)
        t8550 = t7949 * t8548
        t8552 = (t8550 - t4087) * t236
        t8553 = t8552 / 0.2E1
        t8554 = t8520 ** 2
        t8555 = t8511 ** 2
        t8556 = t8507 ** 2
        t8558 = t8526 * (t8554 + t8555 + t8556)
        t8561 = t4 * (t8558 / 0.2E1 + t4110 / 0.2E1)
        t8562 = t8561 * t5192
        t8564 = (t8562 - t4119) * t236
        t8565 = (t5534 - t8429) * t94 + t5548 + t8436 + t5560 + t8441 + 
     #t8478 + t5155 + t8489 + t8503 + t5201 + t8540 + t4040 + t8553 + t4
     #092 + t8564
        t8566 = t8565 * t4023
        t8568 = (t8566 - t4132) * t236
        t8569 = t7629 ** 2
        t8570 = t7642 ** 2
        t8571 = t7640 ** 2
        t8573 = t7651 * (t8569 + t8570 + t8571)
        t8576 = t4 * (t5710 / 0.2E1 + t8573 / 0.2E1)
        t8577 = t8576 * t4070
        t8581 = t7685 * t7688
        t8584 = (t5725 - t8581) * t94 / 0.2E1
        t8586 = t7131 * t8305
        t8589 = (t5737 - t8586) * t94 / 0.2E1
        t8590 = rx(i,t1328,t238,0,0)
        t8591 = rx(i,t1328,t238,1,1)
        t8593 = rx(i,t1328,t238,2,2)
        t8595 = rx(i,t1328,t238,1,2)
        t8597 = rx(i,t1328,t238,2,1)
        t8599 = rx(i,t1328,t238,1,0)
        t8601 = rx(i,t1328,t238,0,2)
        t8603 = rx(i,t1328,t238,0,1)
        t8606 = rx(i,t1328,t238,2,0)
        t8611 = t8590 * t8591 * t8593 - t8590 * t8595 * t8597 + t8599 * 
     #t8597 * t8601 - t8599 * t8603 * t8593 + t8606 * t8603 * t8595 - t8
     #606 * t8591 * t8601
        t8612 = 0.1E1 / t8611
        t8613 = t4 * t8612
        t8619 = (t3992 - t7580) * t94
        t8621 = t5772 / 0.2E1 + t8619 / 0.2E1
        t8023 = t8613 * (t8590 * t8599 + t8603 * t8591 + t8601 * t8595)
        t8623 = t8023 * t8621
        t8625 = (t8623 - t5348) * t183
        t8626 = t8625 / 0.2E1
        t8627 = t8599 ** 2
        t8628 = t8591 ** 2
        t8629 = t8595 ** 2
        t8631 = t8612 * (t8627 + t8628 + t8629)
        t8634 = t4 * (t8631 / 0.2E1 + t5367 / 0.2E1)
        t8635 = t8634 * t4098
        t8637 = (t8635 - t5376) * t183
        t8642 = u(i,t1328,t1277,n)
        t8644 = (t3992 - t8642) * t236
        t8646 = t3994 / 0.2E1 + t8644 / 0.2E1
        t8045 = t8613 * (t8599 * t8606 + t8591 * t8597 + t8595 * t8593)
        t8648 = t8045 * t8646
        t8650 = (t8648 - t5394) * t183
        t8651 = t8650 / 0.2E1
        t8652 = rx(i,t180,t1277,0,0)
        t8653 = rx(i,t180,t1277,1,1)
        t8655 = rx(i,t180,t1277,2,2)
        t8657 = rx(i,t180,t1277,1,2)
        t8659 = rx(i,t180,t1277,2,1)
        t8661 = rx(i,t180,t1277,1,0)
        t8663 = rx(i,t180,t1277,0,2)
        t8665 = rx(i,t180,t1277,0,1)
        t8668 = rx(i,t180,t1277,2,0)
        t8673 = t8652 * t8653 * t8655 - t8652 * t8657 * t8659 + t8661 * 
     #t8659 * t8663 - t8661 * t8665 * t8655 + t8668 * t8665 * t8657 - t8
     #668 * t8653 * t8663
        t8674 = 0.1E1 / t8673
        t8675 = t4 * t8674
        t8681 = (t5388 - t8301) * t94
        t8683 = t5836 / 0.2E1 + t8681 / 0.2E1
        t8081 = t8675 * (t8652 * t8668 + t8665 * t8659 + t8663 * t8655)
        t8685 = t8081 * t8683
        t8687 = (t4074 - t8685) * t236
        t8688 = t8687 / 0.2E1
        t8694 = (t8642 - t5388) * t183
        t8696 = t8694 / 0.2E1 + t5452 / 0.2E1
        t8101 = t8675 * (t8661 * t8668 + t8653 * t8659 + t8657 * t8655)
        t8698 = t8101 * t8696
        t8700 = (t4102 - t8698) * t236
        t8701 = t8700 / 0.2E1
        t8702 = t8668 ** 2
        t8703 = t8659 ** 2
        t8704 = t8655 ** 2
        t8706 = t8674 * (t8702 + t8703 + t8704)
        t8709 = t4 * (t4124 / 0.2E1 + t8706 / 0.2E1)
        t8710 = t8709 * t5390
        t8712 = (t4128 - t8710) * t236
        t8713 = (t5714 - t8577) * t94 + t5728 + t8584 + t5740 + t8589 + 
     #t8626 + t5353 + t8637 + t8651 + t5399 + t4077 + t8688 + t4105 + t8
     #701 + t8712
        t8714 = t8713 * t4062
        t8716 = (t4132 - t8714) * t236
        t8718 = t8568 / 0.2E1 + t8716 / 0.2E1
        t8720 = t708 * t8718
        t8722 = t715 * t5478
        t8725 = (t8720 - t8722) * t183 / 0.2E1
        t8726 = t7854 ** 2
        t8727 = t7867 ** 2
        t8728 = t7865 ** 2
        t8730 = t7876 * (t8726 + t8727 + t8728)
        t8733 = t4 * (t5899 / 0.2E1 + t8730 / 0.2E1)
        t8734 = t8733 * t4295
        t8738 = t7520 * t7937
        t8741 = (t5914 - t8738) * t94 / 0.2E1
        t8743 = t7317 * t8119
        t8746 = (t5926 - t8743) * t94 / 0.2E1
        t8747 = rx(i,t1335,t233,0,0)
        t8748 = rx(i,t1335,t233,1,1)
        t8750 = rx(i,t1335,t233,2,2)
        t8752 = rx(i,t1335,t233,1,2)
        t8754 = rx(i,t1335,t233,2,1)
        t8756 = rx(i,t1335,t233,1,0)
        t8758 = rx(i,t1335,t233,0,2)
        t8760 = rx(i,t1335,t233,0,1)
        t8763 = rx(i,t1335,t233,2,0)
        t8768 = t8747 * t8748 * t8750 - t8747 * t8752 * t8754 + t8756 * 
     #t8754 * t8758 - t8756 * t8760 * t8750 + t8763 * t8760 * t8752 - t8
     #763 * t8748 * t8758
        t8769 = 0.1E1 / t8768
        t8770 = t4 * t8769
        t8776 = (t4253 - t7841) * t94
        t8778 = t5961 / 0.2E1 + t8776 / 0.2E1
        t8168 = t8770 * (t8747 * t8756 + t8760 * t8748 + t8758 * t8752)
        t8780 = t8168 * t8778
        t8782 = (t5161 - t8780) * t183
        t8783 = t8782 / 0.2E1
        t8784 = t8756 ** 2
        t8785 = t8748 ** 2
        t8786 = t8752 ** 2
        t8788 = t8769 * (t8784 + t8785 + t8786)
        t8791 = t4 * (t5183 / 0.2E1 + t8788 / 0.2E1)
        t8792 = t8791 * t4347
        t8794 = (t5187 - t8792) * t183
        t8799 = u(i,t1335,t1229,n)
        t8801 = (t8799 - t4253) * t236
        t8803 = t8801 / 0.2E1 + t4255 / 0.2E1
        t8198 = t8770 * (t8756 * t8763 + t8748 * t8754 + t8752 * t8750)
        t8805 = t8198 * t8803
        t8807 = (t5208 - t8805) * t183
        t8808 = t8807 / 0.2E1
        t8809 = rx(i,t185,t1229,0,0)
        t8810 = rx(i,t185,t1229,1,1)
        t8812 = rx(i,t185,t1229,2,2)
        t8814 = rx(i,t185,t1229,1,2)
        t8816 = rx(i,t185,t1229,2,1)
        t8818 = rx(i,t185,t1229,1,0)
        t8820 = rx(i,t185,t1229,0,2)
        t8822 = rx(i,t185,t1229,0,1)
        t8825 = rx(i,t185,t1229,2,0)
        t8830 = t8809 * t8810 * t8812 - t8809 * t8814 * t8816 + t8818 * 
     #t8816 * t8820 - t8818 * t8822 * t8812 + t8825 * t8822 * t8814 - t8
     #825 * t8810 * t8820
        t8831 = 0.1E1 / t8830
        t8832 = t4 * t8831
        t8838 = (t5202 - t8115) * t94
        t8840 = t6025 / 0.2E1 + t8838 / 0.2E1
        t8235 = t8832 * (t8809 * t8825 + t8822 * t8816 + t8820 * t8812)
        t8842 = t8235 * t8840
        t8844 = (t8842 - t4299) * t236
        t8845 = t8844 / 0.2E1
        t8851 = (t5202 - t8799) * t183
        t8853 = t5256 / 0.2E1 + t8851 / 0.2E1
        t8249 = t8832 * (t8818 * t8825 + t8816 * t8810 + t8814 * t8812)
        t8855 = t8249 * t8853
        t8857 = (t8855 - t4351) * t236
        t8858 = t8857 / 0.2E1
        t8859 = t8825 ** 2
        t8860 = t8816 ** 2
        t8861 = t8812 ** 2
        t8863 = t8831 * (t8859 + t8860 + t8861)
        t8866 = t4 * (t8863 / 0.2E1 + t4374 / 0.2E1)
        t8867 = t8866 * t5204
        t8869 = (t8867 - t4383) * t236
        t8870 = (t5903 - t8734) * t94 + t5917 + t8741 + t5929 + t8746 + 
     #t5164 + t8783 + t8794 + t5211 + t8808 + t8845 + t4304 + t8858 + t4
     #356 + t8869
        t8871 = t8870 * t4287
        t8873 = (t8871 - t4396) * t236
        t8874 = t7893 ** 2
        t8875 = t7906 ** 2
        t8876 = t7904 ** 2
        t8878 = t7915 * (t8874 + t8875 + t8876)
        t8881 = t4 * (t6079 / 0.2E1 + t8878 / 0.2E1)
        t8882 = t8881 * t4334
        t8886 = t7702 * t7952
        t8889 = (t6094 - t8886) * t94 / 0.2E1
        t8891 = t7364 * t8317
        t8894 = (t6106 - t8891) * t94 / 0.2E1
        t8895 = rx(i,t1335,t238,0,0)
        t8896 = rx(i,t1335,t238,1,1)
        t8898 = rx(i,t1335,t238,2,2)
        t8900 = rx(i,t1335,t238,1,2)
        t8902 = rx(i,t1335,t238,2,1)
        t8904 = rx(i,t1335,t238,1,0)
        t8906 = rx(i,t1335,t238,0,2)
        t8908 = rx(i,t1335,t238,0,1)
        t8911 = rx(i,t1335,t238,2,0)
        t8916 = t8895 * t8896 * t8898 - t8895 * t8900 * t8902 + t8904 * 
     #t8902 * t8906 - t8904 * t8908 * t8898 + t8911 * t8908 * t8900 - t8
     #911 * t8896 * t8906
        t8917 = 0.1E1 / t8916
        t8918 = t4 * t8917
        t8924 = (t4256 - t7844) * t94
        t8926 = t6141 / 0.2E1 + t8924 / 0.2E1
        t8318 = t8918 * (t8895 * t8904 + t8908 * t8896 + t8906 * t8900)
        t8928 = t8318 * t8926
        t8930 = (t5359 - t8928) * t183
        t8931 = t8930 / 0.2E1
        t8932 = t8904 ** 2
        t8933 = t8896 ** 2
        t8934 = t8900 ** 2
        t8936 = t8917 * (t8932 + t8933 + t8934)
        t8939 = t4 * (t5381 / 0.2E1 + t8936 / 0.2E1)
        t8940 = t8939 * t4362
        t8942 = (t5385 - t8940) * t183
        t8947 = u(i,t1335,t1277,n)
        t8949 = (t4256 - t8947) * t236
        t8951 = t4258 / 0.2E1 + t8949 / 0.2E1
        t8340 = t8918 * (t8904 * t8911 + t8896 * t8902 + t8900 * t8898)
        t8953 = t8340 * t8951
        t8955 = (t5406 - t8953) * t183
        t8956 = t8955 / 0.2E1
        t8957 = rx(i,t185,t1277,0,0)
        t8958 = rx(i,t185,t1277,1,1)
        t8960 = rx(i,t185,t1277,2,2)
        t8962 = rx(i,t185,t1277,1,2)
        t8964 = rx(i,t185,t1277,2,1)
        t8966 = rx(i,t185,t1277,1,0)
        t8968 = rx(i,t185,t1277,0,2)
        t8970 = rx(i,t185,t1277,0,1)
        t8973 = rx(i,t185,t1277,2,0)
        t8978 = t8957 * t8958 * t8960 - t8957 * t8962 * t8964 + t8966 * 
     #t8964 * t8968 - t8966 * t8970 * t8960 + t8973 * t8970 * t8962 - t8
     #973 * t8958 * t8968
        t8979 = 0.1E1 / t8978
        t8980 = t4 * t8979
        t8986 = (t5400 - t8313) * t94
        t8988 = t6205 / 0.2E1 + t8986 / 0.2E1
        t8378 = t8980 * (t8957 * t8973 + t8970 * t8964 + t8968 * t8960)
        t8990 = t8378 * t8988
        t8992 = (t4338 - t8990) * t236
        t8993 = t8992 / 0.2E1
        t8999 = (t5400 - t8947) * t183
        t9001 = t5454 / 0.2E1 + t8999 / 0.2E1
        t8394 = t8980 * (t8966 * t8973 + t8958 * t8964 + t8962 * t8960)
        t9003 = t8394 * t9001
        t9005 = (t4366 - t9003) * t236
        t9006 = t9005 / 0.2E1
        t9007 = t8973 ** 2
        t9008 = t8964 ** 2
        t9009 = t8960 ** 2
        t9011 = t8979 * (t9007 + t9008 + t9009)
        t9014 = t4 * (t4388 / 0.2E1 + t9011 / 0.2E1)
        t9015 = t9014 * t5402
        t9017 = (t4392 - t9015) * t236
        t9018 = (t6083 - t8882) * t94 + t6097 + t8889 + t6109 + t8894 + 
     #t5362 + t8931 + t8942 + t5409 + t8956 + t4341 + t8993 + t4369 + t9
     #006 + t9017
        t9019 = t9018 * t4326
        t9021 = (t4396 - t9019) * t236
        t9023 = t8873 / 0.2E1 + t9021 / 0.2E1
        t9025 = t731 * t9023
        t9028 = (t8722 - t9025) * t183 / 0.2E1
        t9030 = (t5276 - t8189) * t94
        t9032 = t6251 / 0.2E1 + t9030 / 0.2E1
        t9034 = t771 * t9032
        t9036 = t265 * t7446
        t9039 = (t9034 - t9036) * t236 / 0.2E1
        t9041 = (t5474 - t8387) * t94
        t9043 = t6264 / 0.2E1 + t9041 / 0.2E1
        t9045 = t809 * t9043
        t9048 = (t9036 - t9045) * t236 / 0.2E1
        t9050 = (t8566 - t5276) * t183
        t9052 = (t5276 - t8871) * t183
        t9054 = t9050 / 0.2E1 + t9052 / 0.2E1
        t9056 = t822 * t9054
        t9058 = t715 * t4400
        t9061 = (t9056 - t9058) * t236 / 0.2E1
        t9063 = (t8714 - t5474) * t183
        t9065 = (t5474 - t9019) * t183
        t9067 = t9063 / 0.2E1 + t9065 / 0.2E1
        t9069 = t837 * t9067
        t9072 = (t9058 - t9069) * t236 / 0.2E1
        t9073 = t873 * t5278
        t9074 = t882 * t5476
        t9077 = (t2925 - t7456) * t94 + t4405 + t7993 + t5483 + t8396 + 
     #t8407 + t8416 + (t8417 - t8418) * t183 + t8725 + t9028 + t9039 + t
     #9048 + t9061 + t9072 + (t9073 - t9074) * t236
        t9079 = t895 * t9077 * t56
        t9081 = t2923 * t9079 / 0.6E1
        t9082 = t7296 / 0.2E1
        t9083 = t6902 / 0.2E1
        t9085 = t1114 / 0.2E1 + t6913 / 0.2E1
        t9087 = t3636 * t9085
        t9089 = t147 / 0.2E1 + t6850 / 0.2E1
        t9091 = t573 * t9089
        t9093 = (t9087 - t9091) * t183
        t9094 = t9093 / 0.2E1
        t9096 = t1127 / 0.2E1 + t6932 / 0.2E1
        t9098 = t3883 * t9096
        t9100 = (t9091 - t9098) * t183
        t9101 = t9100 / 0.2E1
        t9102 = t7343 * t1089
        t9103 = t7352 * t1092
        t9105 = (t9102 - t9103) * t183
        t9106 = ut(t96,t180,t233,n)
        t9108 = (t9106 - t1087) * t236
        t9109 = ut(t96,t180,t238,n)
        t9111 = (t1087 - t9109) * t236
        t9113 = t9108 / 0.2E1 + t9111 / 0.2E1
        t9115 = t6869 * t9113
        t9117 = t6882 * t1107
        t9119 = (t9115 - t9117) * t183
        t9120 = t9119 / 0.2E1
        t9121 = ut(t96,t185,t233,n)
        t9123 = (t9121 - t1090) * t236
        t9124 = ut(t96,t185,t238,n)
        t9126 = (t1090 - t9124) * t236
        t9128 = t9123 / 0.2E1 + t9126 / 0.2E1
        t9130 = t6890 * t9128
        t9132 = (t9117 - t9130) * t183
        t9133 = t9132 / 0.2E1
        t9135 = t1168 / 0.2E1 + t7225 / 0.2E1
        t9137 = t4813 * t9135
        t9139 = t590 * t9089
        t9141 = (t9137 - t9139) * t236
        t9142 = t9141 / 0.2E1
        t9144 = t1179 / 0.2E1 + t7239 / 0.2E1
        t9146 = t4953 * t9144
        t9148 = (t9139 - t9146) * t236
        t9149 = t9148 / 0.2E1
        t9151 = (t9106 - t1100) * t183
        t9153 = (t1100 - t9121) * t183
        t9155 = t9151 / 0.2E1 + t9153 / 0.2E1
        t9157 = t6908 * t9155
        t9159 = t6882 * t1094
        t9161 = (t9157 - t9159) * t236
        t9162 = t9161 / 0.2E1
        t9164 = (t9109 - t1103) * t183
        t9166 = (t1103 - t9124) * t183
        t9168 = t9164 / 0.2E1 + t9166 / 0.2E1
        t9170 = t6915 * t9168
        t9172 = (t9159 - t9170) * t236
        t9173 = t9172 / 0.2E1
        t9174 = t7428 * t1102
        t9175 = t7437 * t1105
        t9177 = (t9174 - t9175) * t236
        t9178 = t7162 + t1099 + t9082 + t1112 + t9083 + t9094 + t9101 + 
     #t9105 + t9120 + t9133 + t9142 + t9149 + t9162 + t9173 + t9177
        t9179 = t9178 * t118
        t9180 = t1216 - t9179
        t9181 = t9180 * t94
        t9184 = t6305 * (t6406 / 0.2E1 + t9181 / 0.2E1)
        t9186 = t2079 * t9184 / 0.4E1
        t9188 = t2635 * (t2910 - t7444)
        t9190 = t1227 * t9188 / 0.12E2
        t9191 = t137 + t1227 * t2059 - t2077 + t2079 * t2632 / 0.2E1 - t
     #1227 * t2913 / 0.2E1 + t2921 + t2923 * t6302 / 0.6E1 - t2079 * t64
     #09 / 0.4E1 + t1227 * t6413 / 0.12E2 - t2 - t6846 - t6861 - t7310 -
     # t7449 - t7455 - t9081 - t9186 - t9190
        t9195 = 0.8E1 * t58
        t9196 = 0.8E1 * t59
        t9197 = 0.8E1 * t60
        t9207 = sqrt(0.8E1 * t29 + 0.8E1 * t30 + 0.8E1 * t31 + t9195 + t
     #9196 + t9197 - 0.2E1 * dx * ((t88 + t89 + t90 - t29 - t30 - t31) *
     # t94 / 0.2E1 - (t58 + t59 + t60 - t120 - t121 - t122) * t94 / 0.2E
     #1))
        t9208 = 0.1E1 / t9207
        t9212 = 0.1E1 / 0.2E1 - t134
        t9213 = t9212 * dt
        t9215 = t132 * t9213 * t153
        t9216 = t9212 ** 2
        t9219 = t158 * t9216 * t890 / 0.2E1
        t9220 = t9216 * t9212
        t9223 = t158 * t9220 * t1219 / 0.6E1
        t9225 = t9213 * t1223 / 0.24E2
        t9226 = beta * t9212
        t9228 = t2078 * t9216
        t9233 = t2922 * t9220
        t9240 = t9226 * t6845
        t9242 = t9228 * t7308 / 0.2E1
        t9244 = t9226 * t7447 / 0.2E1
        t9246 = t9233 * t9079 / 0.6E1
        t9248 = t9228 * t9184 / 0.4E1
        t9250 = t9226 * t9188 / 0.12E2
        t9251 = t137 + t9226 * t2059 - t2077 + t9228 * t2632 / 0.2E1 - t
     #9226 * t2913 / 0.2E1 + t2921 + t9233 * t6302 / 0.6E1 - t9228 * t64
     #09 / 0.4E1 + t9226 * t6413 / 0.12E2 - t2 - t9240 - t6861 - t9242 -
     # t9244 - t7455 - t9246 - t9248 - t9250
        t9254 = 0.2E1 * t1226 * t9251 * t9208
        t9256 = (t132 * t136 * t153 + t158 * t159 * t890 / 0.2E1 + t158 
     #* t893 * t1219 / 0.6E1 - t136 * t1223 / 0.24E2 + 0.2E1 * t1226 * t
     #9191 * t9208 - t9215 - t9219 - t9223 + t9225 - t9254) * t133
        t9262 = t132 * (t171 - dx * t1714 / 0.24E2)
        t9264 = dx * t1727 / 0.24E2
        t9269 = t28 * t197
        t9271 = t57 * t215
        t9272 = t9271 / 0.2E1
        t9276 = t119 * t579
        t9284 = t4 * (t9269 / 0.2E1 + t9272 - dx * ((t87 * t179 - t9269)
     # * t94 / 0.2E1 - (t9271 - t9276) * t94 / 0.2E1) / 0.8E1)
        t9289 = t1327 * (t2309 / 0.2E1 + t2314 / 0.2E1)
        t9291 = t925 / 0.4E1
        t9292 = t928 / 0.4E1
        t9295 = t1327 * (t7095 / 0.2E1 + t7100 / 0.2E1)
        t9296 = t9295 / 0.12E2
        t9302 = (t902 - t905) * t183
        t9313 = t912 / 0.2E1
        t9314 = t915 / 0.2E1
        t9315 = t9289 / 0.6E1
        t9318 = t925 / 0.2E1
        t9319 = t928 / 0.2E1
        t9320 = t9295 / 0.6E1
        t9321 = t1089 / 0.2E1
        t9322 = t1092 / 0.2E1
        t9326 = (t1089 - t1092) * t183
        t9328 = ((t7034 - t1089) * t183 - t9326) * t183
        t9332 = (t9326 - (t1092 - t7040) * t183) * t183
        t9335 = t1327 * (t9328 / 0.2E1 + t9332 / 0.2E1)
        t9336 = t9335 / 0.6E1
        t9343 = t912 / 0.4E1 + t915 / 0.4E1 - t9289 / 0.12E2 + t9291 + t
     #9292 - t9296 - dx * ((t902 / 0.2E1 + t905 / 0.2E1 - t1327 * (((t25
     #91 - t902) * t183 - t9302) * t183 / 0.2E1 + (t9302 - (t905 - t2596
     #) * t183) * t183 / 0.2E1) / 0.6E1 - t9313 - t9314 + t9315) * t94 /
     # 0.2E1 - (t9318 + t9319 - t9320 - t9321 - t9322 + t9336) * t94 / 0
     #.2E1) / 0.8E1
        t9348 = t4 * (t9269 / 0.2E1 + t9271 / 0.2E1)
        t9349 = t159 * t161
        t9351 = t3705 / 0.4E1 + t3863 / 0.4E1 + t4134 / 0.4E1 + t4398 / 
     #0.4E1
        t9355 = t893 * t895
        t9357 = t3555 * t975
        t9359 = (t2972 * t973 - t9357) * t94
        t9365 = t2261 / 0.2E1 + t912 / 0.2E1
        t9367 = t306 * t9365
        t9369 = (t2519 * (t2591 / 0.2E1 + t902 / 0.2E1) - t9367) * t94
        t9372 = t2610 / 0.2E1 + t925 / 0.2E1
        t9374 = t629 * t9372
        t9376 = (t9367 - t9374) * t94
        t9377 = t9376 / 0.2E1
        t9381 = t2765 * t1009
        t9383 = (t2758 * t6337 - t9381) * t94
        t9386 = t3377 * t1146
        t9388 = (t9381 - t9386) * t94
        t9389 = t9388 / 0.2E1
        t9393 = (t6330 - t1002) * t94
        t9395 = (t1002 - t1139) * t94
        t9397 = t9393 / 0.2E1 + t9395 / 0.2E1
        t9401 = t2765 * t977
        t9406 = (t6333 - t1005) * t94
        t9408 = (t1005 - t1142) * t94
        t9410 = t9406 / 0.2E1 + t9408 / 0.2E1
        t9417 = t2247 / 0.2E1 + t1055 / 0.2E1
        t9421 = t388 * t9365
        t9426 = t2277 / 0.2E1 + t1068 / 0.2E1
        t9436 = t9359 + t9369 / 0.2E1 + t9377 + t9383 / 0.2E1 + t9389 + 
     #t2135 / 0.2E1 + t986 + t2320 + t2092 / 0.2E1 + t1016 + (t3402 * t9
     #397 - t9401) * t236 / 0.2E1 + (t9401 - t3433 * t9410) * t236 / 0.2
     #E1 + (t3443 * t9417 - t9421) * t236 / 0.2E1 + (t9421 - t3456 * t94
     #26) * t236 / 0.2E1 + (t3689 * t1004 - t3698 * t1007) * t236
        t9437 = t9436 * t301
        t9441 = t3713 * t990
        t9443 = (t3280 * t988 - t9441) * t94
        t9449 = t915 / 0.2E1 + t2266 / 0.2E1
        t9451 = t348 * t9449
        t9453 = (t2543 * (t905 / 0.2E1 + t2596 / 0.2E1) - t9451) * t94
        t9456 = t928 / 0.2E1 + t2615 / 0.2E1
        t9458 = t669 * t9456
        t9460 = (t9451 - t9458) * t94
        t9461 = t9460 / 0.2E1
        t9465 = t3101 * t1024
        t9467 = (t3095 * t6352 - t9465) * t94
        t9470 = t3487 * t1161
        t9472 = (t9465 - t9470) * t94
        t9473 = t9472 / 0.2E1
        t9477 = (t6345 - t1017) * t94
        t9479 = (t1017 - t1154) * t94
        t9481 = t9477 / 0.2E1 + t9479 / 0.2E1
        t9485 = t3101 * t992
        t9490 = (t6348 - t1020) * t94
        t9492 = (t1020 - t1157) * t94
        t9494 = t9490 / 0.2E1 + t9492 / 0.2E1
        t9501 = t1057 / 0.2E1 + t2252 / 0.2E1
        t9505 = t411 * t9449
        t9510 = t1070 / 0.2E1 + t2282 / 0.2E1
        t9520 = t9443 + t9453 / 0.2E1 + t9461 + t9467 / 0.2E1 + t9473 + 
     #t997 + t2153 / 0.2E1 + t2325 + t1029 + t2111 / 0.2E1 + (t3511 * t9
     #481 - t9485) * t236 / 0.2E1 + (t9485 - t3543 * t9494) * t236 / 0.2
     #E1 + (t3554 * t9501 - t9505) * t236 / 0.2E1 + (t9505 - t3568 * t95
     #10) * t236 / 0.2E1 + (t3847 * t1019 - t3856 * t1022) * t236
        t9521 = t9520 * t344
        t9524 = t3901 * t1114
        t9526 = (t9357 - t9524) * t94
        t9528 = t7034 / 0.2E1 + t1089 / 0.2E1
        t9530 = t3636 * t9528
        t9532 = (t9374 - t9530) * t94
        t9533 = t9532 / 0.2E1
        t9535 = t3651 * t9113
        t9537 = (t9386 - t9535) * t94
        t9538 = t9537 / 0.2E1
        t9539 = t7260 / 0.2E1
        t9540 = t7199 / 0.2E1
        t9542 = (t1139 - t9106) * t94
        t9544 = t9395 / 0.2E1 + t9542 / 0.2E1
        t9546 = t3759 * t9544
        t9548 = t3377 * t1116
        t9550 = (t9546 - t9548) * t236
        t9551 = t9550 / 0.2E1
        t9553 = (t1142 - t9109) * t94
        t9555 = t9408 / 0.2E1 + t9553 / 0.2E1
        t9557 = t3793 * t9555
        t9559 = (t9548 - t9557) * t236
        t9560 = t9559 / 0.2E1
        t9562 = t6991 / 0.2E1 + t1188 / 0.2E1
        t9564 = t3806 * t9562
        t9566 = t708 * t9372
        t9568 = (t9564 - t9566) * t236
        t9569 = t9568 / 0.2E1
        t9571 = t7012 / 0.2E1 + t1201 / 0.2E1
        t9573 = t3817 * t9571
        t9575 = (t9566 - t9573) * t236
        t9576 = t9575 / 0.2E1
        t9577 = t4118 * t1141
        t9578 = t4127 * t1144
        t9580 = (t9577 - t9578) * t236
        t9581 = t9526 + t9377 + t9533 + t9389 + t9538 + t9539 + t1125 + 
     #t7106 + t9540 + t1153 + t9551 + t9560 + t9569 + t9576 + t9580
        t9582 = t9581 * t631
        t9583 = t9582 - t1216
        t9584 = t9583 * t183
        t9585 = t4165 * t1127
        t9587 = (t9441 - t9585) * t94
        t9589 = t1092 / 0.2E1 + t7040 / 0.2E1
        t9591 = t3883 * t9589
        t9593 = (t9458 - t9591) * t94
        t9594 = t9593 / 0.2E1
        t9596 = t3899 * t9128
        t9598 = (t9470 - t9596) * t94
        t9599 = t9598 / 0.2E1
        t9600 = t7274 / 0.2E1
        t9601 = t7215 / 0.2E1
        t9603 = (t1154 - t9121) * t94
        t9605 = t9479 / 0.2E1 + t9603 / 0.2E1
        t9607 = t4006 * t9605
        t9609 = t3487 * t1129
        t9611 = (t9607 - t9609) * t236
        t9612 = t9611 / 0.2E1
        t9614 = (t1157 - t9124) * t94
        t9616 = t9492 / 0.2E1 + t9614 / 0.2E1
        t9618 = t4043 * t9616
        t9620 = (t9609 - t9618) * t236
        t9621 = t9620 / 0.2E1
        t9623 = t1190 / 0.2E1 + t6997 / 0.2E1
        t9625 = t4056 * t9623
        t9627 = t731 * t9456
        t9629 = (t9625 - t9627) * t236
        t9630 = t9629 / 0.2E1
        t9632 = t1203 / 0.2E1 + t7018 / 0.2E1
        t9634 = t4067 * t9632
        t9636 = (t9627 - t9634) * t236
        t9637 = t9636 / 0.2E1
        t9638 = t4382 * t1156
        t9639 = t4391 * t1159
        t9641 = (t9638 - t9639) * t236
        t9642 = t9587 + t9461 + t9594 + t9473 + t9599 + t1134 + t9600 + 
     #t7111 + t1166 + t9601 + t9612 + t9621 + t9630 + t9637 + t9641
        t9643 = t9642 * t672
        t9644 = t1216 - t9643
        t9645 = t9644 * t183
        t9647 = (t9437 - t1083) * t183 / 0.4E1 + (t1083 - t9521) * t183 
     #/ 0.4E1 + t9584 / 0.4E1 + t9645 / 0.4E1
        t9653 = dx * (t921 / 0.2E1 - t1098 / 0.2E1)
        t9657 = t9284 * t9213 * t9343
        t9658 = t9216 * t161
        t9661 = t9348 * t9658 * t9351 / 0.2E1
        t9662 = t9220 * t895
        t9665 = t9348 * t9662 * t9647 / 0.6E1
        t9667 = t9213 * t9653 / 0.24E2
        t9669 = (t9284 * t136 * t9343 + t9348 * t9349 * t9351 / 0.2E1 + 
     #t9348 * t9355 * t9647 / 0.6E1 - t136 * t9653 / 0.24E2 - t9657 - t9
     #661 - t9665 + t9667) * t133
        t9676 = t1327 * (t1524 / 0.2E1 + t1529 / 0.2E1)
        t9678 = t218 / 0.4E1
        t9679 = t221 / 0.4E1
        t9682 = t1327 * (t6681 / 0.2E1 + t6686 / 0.2E1)
        t9683 = t9682 / 0.12E2
        t9689 = (t184 - t188) * t183
        t9700 = t200 / 0.2E1
        t9701 = t203 / 0.2E1
        t9702 = t9676 / 0.6E1
        t9705 = t218 / 0.2E1
        t9706 = t221 / 0.2E1
        t9707 = t9682 / 0.6E1
        t9708 = t582 / 0.2E1
        t9709 = t585 / 0.2E1
        t9713 = (t582 - t585) * t183
        t9715 = ((t3912 - t582) * t183 - t9713) * t183
        t9719 = (t9713 - (t585 - t4176) * t183) * t183
        t9722 = t1327 * (t9715 / 0.2E1 + t9719 / 0.2E1)
        t9723 = t9722 / 0.6E1
        t9731 = t9284 * (t200 / 0.4E1 + t203 / 0.4E1 - t9676 / 0.12E2 + 
     #t9678 + t9679 - t9683 - dx * ((t184 / 0.2E1 + t188 / 0.2E1 - t1327
     # * (((t1388 - t184) * t183 - t9689) * t183 / 0.2E1 + (t9689 - (t18
     #8 - t1394) * t183) * t183 / 0.2E1) / 0.6E1 - t9700 - t9701 + t9702
     #) * t94 / 0.2E1 - (t9705 + t9706 - t9707 - t9708 - t9709 + t9723) 
     #* t94 / 0.2E1) / 0.8E1)
        t9735 = dx * (t209 / 0.2E1 - t591 / 0.2E1) / 0.24E2
        t9740 = t28 * t249
        t9742 = t57 * t266
        t9743 = t9742 / 0.2E1
        t9747 = t119 * t596
        t9755 = t4 * (t9740 / 0.2E1 + t9743 - dx * ((t87 * t232 - t9740)
     # * t94 / 0.2E1 - (t9742 - t9747) * t94 / 0.2E1) / 0.8E1)
        t9760 = t1228 * (t2442 / 0.2E1 + t2447 / 0.2E1)
        t9762 = t961 / 0.4E1
        t9763 = t964 / 0.4E1
        t9766 = t1228 * (t6867 / 0.2E1 + t6872 / 0.2E1)
        t9767 = t9766 / 0.12E2
        t9773 = (t938 - t941) * t236
        t9784 = t948 / 0.2E1
        t9785 = t951 / 0.2E1
        t9786 = t9760 / 0.6E1
        t9789 = t961 / 0.2E1
        t9790 = t964 / 0.2E1
        t9791 = t9766 / 0.6E1
        t9792 = t1102 / 0.2E1
        t9793 = t1105 / 0.2E1
        t9797 = (t1102 - t1105) * t236
        t9799 = ((t7171 - t1102) * t236 - t9797) * t236
        t9803 = (t9797 - (t1105 - t7176) * t236) * t236
        t9806 = t1228 * (t9799 / 0.2E1 + t9803 / 0.2E1)
        t9807 = t9806 / 0.6E1
        t9814 = t948 / 0.4E1 + t951 / 0.4E1 - t9760 / 0.12E2 + t9762 + t
     #9763 - t9767 - dx * ((t938 / 0.2E1 + t941 / 0.2E1 - t1228 * (((t25
     #52 - t938) * t236 - t9773) * t236 / 0.2E1 + (t9773 - (t941 - t2557
     #) * t236) * t236 / 0.2E1) / 0.6E1 - t9784 - t9785 + t9786) * t94 /
     # 0.2E1 - (t9789 + t9790 - t9791 - t9792 - t9793 + t9807) * t94 / 0
     #.2E1) / 0.8E1
        t9819 = t4 * (t9740 / 0.2E1 + t9742 / 0.2E1)
        t9821 = t4979 / 0.4E1 + t5073 / 0.4E1 + t5278 / 0.4E1 + t5476 / 
     #0.4E1
        t9826 = t4893 * t1033
        t9828 = (t4450 * t1031 - t9826) * t94
        t9832 = t4153 * t1059
        t9834 = (t4147 * t6379 - t9832) * t94
        t9837 = t4670 * t1192
        t9839 = (t9832 - t9837) * t94
        t9840 = t9839 / 0.2E1
        t9846 = t2350 / 0.2E1 + t948 / 0.2E1
        t9848 = t452 * t9846
        t9850 = (t2605 * (t2552 / 0.2E1 + t938 / 0.2E1) - t9848) * t94
        t9853 = t2571 / 0.2E1 + t961 / 0.2E1
        t9855 = t771 * t9853
        t9857 = (t9848 - t9855) * t94
        t9858 = t9857 / 0.2E1
        t9862 = t4153 * t1035
        t9876 = t2335 / 0.2E1 + t1004 / 0.2E1
        t9880 = t507 * t9846
        t9885 = t2367 / 0.2E1 + t1019 / 0.2E1
        t9893 = t9828 + t9834 / 0.2E1 + t9840 + t9850 / 0.2E1 + t9858 + 
     #(t4684 * t9397 - t9862) * t183 / 0.2E1 + (t9862 - t4694 * t9481) *
     # t183 / 0.2E1 + (t4945 * t1055 - t4954 * t1057) * t183 + (t3443 * 
     #t9876 - t9880) * t183 / 0.2E1 + (t9880 - t3554 * t9885) * t183 / 0
     #.2E1 + t2218 / 0.2E1 + t1042 + t2396 / 0.2E1 + t1066 + t2453
        t9894 = t9893 * t448
        t9898 = t4987 * t1046
        t9900 = (t4688 * t1044 - t9898) * t94
        t9904 = t4468 * t1072
        t9906 = (t4464 * t6392 - t9904) * t94
        t9909 = t4726 * t1205
        t9911 = (t9904 - t9909) * t94
        t9912 = t9911 / 0.2E1
        t9918 = t951 / 0.2E1 + t2355 / 0.2E1
        t9920 = t492 * t9918
        t9922 = (t2634 * (t941 / 0.2E1 + t2557 / 0.2E1) - t9920) * t94
        t9925 = t964 / 0.2E1 + t2576 / 0.2E1
        t9927 = t809 * t9925
        t9929 = (t9920 - t9927) * t94
        t9930 = t9929 / 0.2E1
        t9934 = t4468 * t1048
        t9948 = t1007 / 0.2E1 + t2341 / 0.2E1
        t9952 = t521 * t9918
        t9957 = t1022 / 0.2E1 + t2373 / 0.2E1
        t9965 = t9900 + t9906 / 0.2E1 + t9912 + t9922 / 0.2E1 + t9930 + 
     #(t4734 * t9410 - t9934) * t183 / 0.2E1 + (t9934 - t4743 * t9494) *
     # t183 / 0.2E1 + (t5039 * t1068 - t5048 * t1070) * t183 + (t3456 * 
     #t9948 - t9952) * t183 / 0.2E1 + (t9952 - t3568 * t9957) * t183 / 0
     #.2E1 + t1053 + t2237 / 0.2E1 + t1077 + t2412 / 0.2E1 + t2458
        t9966 = t9965 * t489
        t9969 = t5111 * t1168
        t9971 = (t9826 - t9969) * t94
        t9973 = t4804 * t9155
        t9975 = (t9837 - t9973) * t94
        t9976 = t9975 / 0.2E1
        t9978 = t7171 / 0.2E1 + t1102 / 0.2E1
        t9980 = t4813 * t9978
        t9982 = (t9855 - t9980) * t94
        t9983 = t9982 / 0.2E1
        t9985 = t4823 * t9544
        t9987 = t4670 * t1170
        t9989 = (t9985 - t9987) * t183
        t9990 = t9989 / 0.2E1
        t9992 = t4832 * t9605
        t9994 = (t9987 - t9992) * t183
        t9995 = t9994 / 0.2E1
        t9996 = t5177 * t1188
        t9997 = t5186 * t1190
        t9999 = (t9996 - t9997) * t183
        t10001 = t6948 / 0.2E1 + t1141 / 0.2E1
        t10003 = t3806 * t10001
        t10005 = t822 * t9853
        t10007 = (t10003 - t10005) * t183
        t10008 = t10007 / 0.2E1
        t10010 = t6969 / 0.2E1 + t1156 / 0.2E1
        t10012 = t4056 * t10010
        t10014 = (t10005 - t10012) * t183
        t10015 = t10014 / 0.2E1
        t10016 = t7065 / 0.2E1
        t10017 = t7128 / 0.2E1
        t10018 = t9971 + t9840 + t9976 + t9858 + t9983 + t9990 + t9995 +
     # t9999 + t10008 + t10015 + t10016 + t1177 + t10017 + t1199 + t6878
        t10019 = t10018 * t774
        t10020 = t10019 - t1216
        t10021 = t10020 * t236
        t10022 = t5309 * t1179
        t10024 = (t9898 - t10022) * t94
        t10026 = t4931 * t9168
        t10028 = (t9909 - t10026) * t94
        t10029 = t10028 / 0.2E1
        t10031 = t1105 / 0.2E1 + t7176 / 0.2E1
        t10033 = t4953 * t10031
        t10035 = (t9927 - t10033) * t94
        t10036 = t10035 / 0.2E1
        t10038 = t4962 * t9555
        t10040 = t4726 * t1181
        t10042 = (t10038 - t10040) * t183
        t10043 = t10042 / 0.2E1
        t10045 = t4971 * t9616
        t10047 = (t10040 - t10045) * t183
        t10048 = t10047 / 0.2E1
        t10049 = t5375 * t1201
        t10050 = t5384 * t1203
        t10052 = (t10049 - t10050) * t183
        t10054 = t1144 / 0.2E1 + t6954 / 0.2E1
        t10056 = t3817 * t10054
        t10058 = t837 * t9925
        t10060 = (t10056 - t10058) * t183
        t10061 = t10060 / 0.2E1
        t10063 = t1159 / 0.2E1 + t6975 / 0.2E1
        t10065 = t4067 * t10063
        t10067 = (t10058 - t10065) * t183
        t10068 = t10067 / 0.2E1
        t10069 = t7080 / 0.2E1
        t10070 = t7144 / 0.2E1
        t10071 = t10024 + t9912 + t10029 + t9930 + t10036 + t10043 + t10
     #048 + t10052 + t10061 + t10068 + t1186 + t10069 + t1210 + t10070 +
     # t6883
        t10072 = t10071 * t813
        t10073 = t1216 - t10072
        t10074 = t10073 * t236
        t10076 = (t9894 - t1083) * t236 / 0.4E1 + (t1083 - t9966) * t236
     # / 0.4E1 + t10021 / 0.4E1 + t10074 / 0.4E1
        t10082 = dx * (t957 / 0.2E1 - t1111 / 0.2E1)
        t10086 = t9755 * t9213 * t9814
        t10089 = t9819 * t9658 * t9821 / 0.2E1
        t10092 = t9819 * t9662 * t10076 / 0.6E1
        t10094 = t9213 * t10082 / 0.24E2
        t10096 = (t9755 * t136 * t9814 + t9819 * t9349 * t9821 / 0.2E1 +
     # t9819 * t9355 * t10076 / 0.6E1 - t136 * t10082 / 0.24E2 - t10086 
     #- t10089 - t10092 + t10094) * t133
        t10103 = t1228 * (t1782 / 0.2E1 + t1787 / 0.2E1)
        t10105 = t269 / 0.4E1
        t10106 = t272 / 0.4E1
        t10109 = t1228 * (t6543 / 0.2E1 + t6548 / 0.2E1)
        t10110 = t10109 / 0.12E2
        t10116 = (t237 - t241) * t236
        t10127 = t252 / 0.2E1
        t10128 = t255 / 0.2E1
        t10129 = t10103 / 0.6E1
        t10132 = t269 / 0.2E1
        t10133 = t272 / 0.2E1
        t10134 = t10109 / 0.6E1
        t10135 = t599 / 0.2E1
        t10136 = t602 / 0.2E1
        t10140 = (t599 - t602) * t236
        t10142 = ((t5137 - t599) * t236 - t10140) * t236
        t10146 = (t10140 - (t602 - t5335) * t236) * t236
        t10149 = t1228 * (t10142 / 0.2E1 + t10146 / 0.2E1)
        t10150 = t10149 / 0.6E1
        t10158 = t9755 * (t252 / 0.4E1 + t255 / 0.4E1 - t10103 / 0.12E2 
     #+ t10105 + t10106 - t10110 - dx * ((t237 / 0.2E1 + t241 / 0.2E1 - 
     #t1228 * (((t1663 - t237) * t236 - t10116) * t236 / 0.2E1 + (t10116
     # - (t241 - t1668) * t236) * t236 / 0.2E1) / 0.6E1 - t10127 - t1012
     #8 + t10129) * t94 / 0.2E1 - (t10132 + t10133 - t10134 - t10135 - t
     #10136 + t10150) * t94 / 0.2E1) / 0.8E1)
        t10162 = dx * (t261 / 0.2E1 - t608 / 0.2E1) / 0.24E2
        t10169 = t147 - dx * t6853 / 0.24E2
        t10174 = t161 * t7443 * t94
        t10179 = t895 * t9180 * t94
        t10182 = dx * t7163
        t10185 = cc * t6493
        t10189 = (t7405 - t7414) * t236
        t10214 = i - 3
        t10215 = rx(t10214,j,k,0,0)
        t10216 = rx(t10214,j,k,1,1)
        t10218 = rx(t10214,j,k,2,2)
        t10220 = rx(t10214,j,k,1,2)
        t10222 = rx(t10214,j,k,2,1)
        t10224 = rx(t10214,j,k,1,0)
        t10226 = rx(t10214,j,k,0,2)
        t10228 = rx(t10214,j,k,0,1)
        t10231 = rx(t10214,j,k,2,0)
        t10237 = 0.1E1 / (t10215 * t10216 * t10218 - t10215 * t10220 * t
     #10222 + t10224 * t10222 * t10226 - t10224 * t10228 * t10218 + t102
     #31 * t10228 * t10220 - t10231 * t10216 * t10226)
        t10238 = t10215 ** 2
        t10239 = t10228 ** 2
        t10240 = t10226 ** 2
        t10242 = t10237 * (t10238 + t10239 + t10240)
        t10250 = t4 * (t6457 + t6486 / 0.2E1 - dx * (t126 / 0.2E1 - (t64
     #86 - t10242) * t94 / 0.2E1) / 0.8E1)
        t10254 = t7330 + t7323 + t7370 + t7379 + t7388 + t7406 + t7395 +
     # t7415 + t7311 - t1228 * (((t8175 - t7405) * t236 - t10189) * t236
     # / 0.2E1 + (t10189 - (t7414 - t8373) * t236) * t236 / 0.2E1) / 0.6
     #E1 + t592 - t1228 * ((t7428 * t10142 - t7437 * t10146) * t236 + ((
     #t8187 - t7440) * t236 - (t7440 - t8385) * t236) * t236) / 0.24E2 +
     # (t6495 - t10250 * t6634) * t94 + t609 + t7312
        t10267 = t6882 * t6396
        t10286 = u(t10214,j,k,n)
        t10288 = (t6518 - t10286) * t94
        t10298 = t4 * (t6486 / 0.2E1 + t10242 / 0.2E1)
        t10301 = (t6710 - t10298 * t10288) * t94
        t10325 = (t7369 - t7378) * t183
        t10337 = t7425 / 0.2E1
        t10347 = t4 * (t7420 / 0.2E1 + t10337 - dz * ((t8181 - t7420) * 
     #t236 / 0.2E1 - (t7425 - t7434) * t236 / 0.2E1) / 0.8E1)
        t10359 = t4 * (t10337 + t7434 / 0.2E1 - dz * ((t7420 - t7425) * 
     #t236 / 0.2E1 - (t7434 - t8379) * t236 / 0.2E1) / 0.8E1)
        t10391 = t6882 * t6274
        t10411 = t7340 / 0.2E1
        t10421 = t4 * (t7335 / 0.2E1 + t10411 - dy * ((t7566 - t7335) * 
     #t183 / 0.2E1 - (t7340 - t7349) * t183 / 0.2E1) / 0.8E1)
        t10433 = t4 * (t10411 + t7349 / 0.2E1 - dy * ((t7335 - t7340) * 
     #t183 / 0.2E1 - (t7349 - t7830) * t183 / 0.2E1) / 0.8E1)
        t10440 = (t7322 - t7329) * t183
        t10451 = u(t10214,j,t233,n)
        t10453 = (t6623 - t10451) * t94
        t9746 = (t6637 - (t571 / 0.2E1 - t10288 / 0.2E1) * t94) * t94
        t10467 = t590 * t9746
        t10470 = u(t10214,j,t238,n)
        t10472 = (t6644 - t10470) * t94
        t10486 = t4 * t10237
        t10492 = (t10451 - t10286) * t236
        t10494 = (t10286 - t10470) * t236
        t9754 = t10486 * (t10215 * t10231 + t10228 * t10222 + t10226 * t
     #10218)
        t10500 = (t6774 - t9754 * (t10492 / 0.2E1 + t10494 / 0.2E1)) * t
     #94
        t10513 = u(t10214,t180,k,n)
        t10515 = (t10513 - t10286) * t183
        t10516 = u(t10214,t185,k,n)
        t10518 = (t10286 - t10516) * t183
        t9770 = t10486 * (t10215 * t10224 + t10228 * t10216 + t10226 * t
     #10220)
        t10524 = (t6527 - t9770 * (t10515 / 0.2E1 + t10518 / 0.2E1)) * t
     #94
        t10536 = (t7387 - t7394) * t236
        t10548 = (t6517 - t10513) * t94
        t10558 = t573 * t9746
        t10562 = (t6521 - t10516) * t94
        t9779 = ((t7671 / 0.2E1 - t5123 / 0.2E1) * t183 - (t5121 / 0.2E1
     # - t7935 / 0.2E1) * t183) * t183
        t9783 = ((t7686 / 0.2E1 - t5321 / 0.2E1) * t183 - (t5319 / 0.2E1
     # - t7950 / 0.2E1) * t183) * t183
        t9882 = ((t8105 / 0.2E1 - t3929 / 0.2E1) * t236 - (t3926 / 0.2E1
     # - t8303 / 0.2E1) * t236) * t236
        t9887 = ((t8117 / 0.2E1 - t4193 / 0.2E1) * t236 - (t4190 / 0.2E1
     # - t8315 / 0.2E1) * t236) * t236
        t10592 = -t1327 * ((t6908 * t9779 - t10267) * t236 / 0.2E1 + (t1
     #0267 - t6915 * t9783) * t236 / 0.2E1) / 0.6E1 - t1701 * ((t6704 - 
     #t6709 * (t6701 - (t6634 - t10288) * t94) * t94) * t94 + (t6714 - (
     #t6712 - t10301) * t94) * t94) / 0.24E2 - t1327 * ((t7343 * t9715 -
     # t7352 * t9719) * t183 + ((t7572 - t7355) * t183 - (t7355 - t7836)
     # * t183) * t183) / 0.24E2 - t1327 * (((t7588 - t7369) * t183 - t10
     #325) * t183 / 0.2E1 + (t10325 - (t7378 - t7852) * t183) * t183 / 0
     #.2E1) / 0.6E1 + (t10347 * t599 - t10359 * t602) * t236 - t1327 * (
     #t6837 / 0.2E1 + (t6835 - t6214 * ((t7500 / 0.2E1 - t6523 / 0.2E1) 
     #* t183 - (t6520 / 0.2E1 - t7764 / 0.2E1) * t183) * t183) * t94 / 0
     #.2E1) / 0.6E1 - t1228 * ((t6869 * t9882 - t10391) * t183 / 0.2E1 +
     # (t10391 - t6890 * t9887) * t183 / 0.2E1) / 0.6E1 + (t10421 * t582
     # - t10433 * t585) * t183 - t1327 * (((t7560 - t7322) * t183 - t104
     #40) * t183 / 0.2E1 + (t10440 - (t7329 - t7824) * t183) * t183 / 0.
     #2E1) / 0.6E1 - t1701 * ((t4813 * (t6628 - (t782 / 0.2E1 - t10453 /
     # 0.2E1) * t94) * t94 - t10467) * t236 / 0.2E1 + (t10467 - t4953 * 
     #(t6649 - (t821 / 0.2E1 - t10472 / 0.2E1) * t94) * t94) * t236 / 0.
     #2E1) / 0.6E1 - t1701 * (t6780 / 0.2E1 + (t6778 - (t6776 - t10500) 
     #* t94) * t94 / 0.2E1) / 0.6E1 - t1701 * (t6533 / 0.2E1 + (t6531 - 
     #(t6529 - t10524) * t94) * t94 / 0.2E1) / 0.6E1 - t1228 * (((t8160 
     #- t7387) * t236 - t10536) * t236 / 0.2E1 + (t10536 - (t7394 - t835
     #8) * t236) * t236 / 0.2E1) / 0.6E1 - t1701 * ((t3636 * (t6738 - (t
     #639 / 0.2E1 - t10548 / 0.2E1) * t94) * t94 - t10558) * t183 / 0.2E
     #1 + (t10558 - t3883 * (t6752 - (t680 / 0.2E1 - t10562 / 0.2E1) * t
     #94) * t94) * t183 / 0.2E1) / 0.6E1 - t1228 * (t6671 / 0.2E1 + (t66
     #69 - t6366 * ((t8050 / 0.2E1 - t6770 / 0.2E1) * t236 - (t6768 / 0.
     #2E1 - t8248 / 0.2E1) * t236) * t236) * t94 / 0.2E1) / 0.6E1
        t10595 = dt * (t10254 + t10592) * t118
        t10598 = ut(t10214,j,k,n)
        t10600 = (t6848 - t10598) * t94
        t10604 = (t6852 - (t6850 - t10600) * t94) * t94
        t10611 = dx * (t6847 + t6850 / 0.2E1 - t1701 * (t6854 / 0.2E1 + 
     #t10604 / 0.2E1) / 0.6E1) / 0.2E1
        t10612 = ut(t96,t180,t1229,n)
        t10614 = (t10612 - t9106) * t236
        t10618 = ut(t96,t180,t1277,n)
        t10620 = (t9109 - t10618) * t236
        t10630 = t6882 * t6694
        t10633 = ut(t96,t185,t1229,n)
        t10635 = (t10633 - t9121) * t236
        t10639 = ut(t96,t185,t1277,n)
        t10641 = (t9124 - t10639) * t236
        t10661 = (t7569 * t7034 - t9102) * t183
        t10666 = (t9103 - t7833 * t7040) * t183
        t10678 = ut(t6458,t1328,k,n)
        t10680 = (t7032 - t10678) * t94
        t10686 = (t7031 * (t7254 / 0.2E1 + t10680 / 0.2E1) - t9087) * t1
     #83
        t10690 = (t9093 - t9100) * t183
        t10693 = ut(t6458,t1335,k,n)
        t10695 = (t7038 - t10693) * t94
        t10701 = (t9098 - t7261 * (t7268 / 0.2E1 + t10695 / 0.2E1)) * t1
     #83
        t10710 = ut(t96,t1328,t233,n)
        t10713 = ut(t96,t1328,t238,n)
        t10717 = (t10710 - t7032) * t236 / 0.2E1 + (t7032 - t10713) * t2
     #36 / 0.2E1
        t10721 = (t7052 * t10717 - t9115) * t183
        t10725 = (t9119 - t9132) * t183
        t10728 = ut(t96,t1335,t233,n)
        t10731 = ut(t96,t1335,t238,n)
        t10735 = (t10728 - t7038) * t236 / 0.2E1 + (t7038 - t10731) * t2
     #36 / 0.2E1
        t10739 = (t9130 - t7282 * t10735) * t183
        t10748 = ut(t10214,j,t233,n)
        t10750 = (t6891 - t10748) * t94
        t10177 = (t6923 - (t147 / 0.2E1 - t10600 / 0.2E1) * t94) * t94
        t10764 = t590 * t10177
        t10767 = ut(t10214,j,t238,n)
        t10769 = (t6894 - t10767) * t94
        t10792 = (t10612 - t7057) * t183 / 0.2E1 + (t7057 - t10633) * t1
     #83 / 0.2E1
        t10796 = (t7609 * t10792 - t9157) * t236
        t10800 = (t9161 - t9172) * t236
        t10808 = (t10618 - t7072) * t183 / 0.2E1 + (t7072 - t10639) * t1
     #83 / 0.2E1
        t10812 = (t9170 - t7797 * t10808) * t236
        t10822 = (t10710 - t9106) * t183
        t10827 = (t9121 - t10728) * t183
        t10837 = t6882 * t6571
        t10841 = (t10713 - t9109) * t183
        t10846 = (t9124 - t10731) * t183
        t10869 = (t6900 - t9754 * ((t10748 - t10598) * t236 / 0.2E1 + (t
     #10598 - t10767) * t236 / 0.2E1)) * t94
        t10884 = (t8184 * t7171 - t9174) * t236
        t10889 = (t9175 - t8382 * t7176) * t236
        t10236 = ((t10614 / 0.2E1 - t9111 / 0.2E1) * t236 - (t9108 / 0.2
     #E1 - t10620 / 0.2E1) * t236) * t236
        t10245 = ((t10635 / 0.2E1 - t9126 / 0.2E1) * t236 - (t9123 / 0.2
     #E1 - t10641 / 0.2E1) * t236) * t236
        t10341 = ((t10822 / 0.2E1 - t9153 / 0.2E1) * t183 - (t9151 / 0.2
     #E1 - t10827 / 0.2E1) * t183) * t183
        t10345 = ((t10841 / 0.2E1 - t9166 / 0.2E1) * t183 - (t9164 / 0.2
     #E1 - t10846 / 0.2E1) * t183) * t183
        t10897 = t9083 - t1228 * ((t6869 * t10236 - t10630) * t183 / 0.2
     #E1 + (t10630 - t6890 * t10245) * t183 / 0.2E1) / 0.6E1 - t1327 * (
     #(t7343 * t9328 - t7352 * t9332) * t183 + ((t10661 - t9105) * t183 
     #- (t9105 - t10666) * t183) * t183) / 0.24E2 + (t10421 * t1089 - t1
     #0433 * t1092) * t183 - t1327 * (((t10686 - t9093) * t183 - t10690)
     # * t183 / 0.2E1 + (t10690 - (t9100 - t10701) * t183) * t183 / 0.2E
     #1) / 0.6E1 - t1327 * (((t10721 - t9119) * t183 - t10725) * t183 / 
     #0.2E1 + (t10725 - (t9132 - t10739) * t183) * t183 / 0.2E1) / 0.6E1
     # - t1701 * ((t4813 * (t7228 - (t1168 / 0.2E1 - t10750 / 0.2E1) * t
     #94) * t94 - t10764) * t236 / 0.2E1 + (t10764 - t4953 * (t7242 - (t
     #1179 / 0.2E1 - t10769 / 0.2E1) * t94) * t94) * t236 / 0.2E1) / 0.6
     #E1 + (t10347 * t1102 - t10359 * t1105) * t236 - t1228 * (((t10796 
     #- t9161) * t236 - t10800) * t236 / 0.2E1 + (t10800 - (t9172 - t108
     #12) * t236) * t236 / 0.2E1) / 0.6E1 - t1327 * ((t6908 * t10341 - t
     #10837) * t236 / 0.2E1 + (t10837 - t6915 * t10345) * t236 / 0.2E1) 
     #/ 0.6E1 - t1701 * (t6906 / 0.2E1 + (t6904 - (t6902 - t10869) * t94
     #) * t94 / 0.2E1) / 0.6E1 - t1228 * ((t7428 * t9799 - t7437 * t9803
     #) * t236 + ((t10884 - t9177) * t236 - (t9177 - t10889) * t236) * t
     #236) / 0.24E2 + t9142 + t9149 + t9162
        t10898 = ut(t10214,t180,k,n)
        t10901 = ut(t10214,t185,k,n)
        t10909 = (t7294 - t9770 * ((t10898 - t10598) * t183 / 0.2E1 + (t
     #10598 - t10901) * t183 / 0.2E1)) * t94
        t10919 = (t10678 - t6911) * t183
        t10924 = (t6930 - t10693) * t183
        t10946 = (t7160 - t10298 * t10600) * t94
        t10954 = ut(t6458,j,t1229,n)
        t10956 = (t7057 - t10954) * t94
        t10962 = (t7596 * (t7059 / 0.2E1 + t10956 / 0.2E1) - t9137) * t2
     #36
        t10966 = (t9141 - t9148) * t236
        t10969 = ut(t6458,j,t1277,n)
        t10971 = (t7072 - t10969) * t94
        t10977 = (t9146 - t7777 * (t7074 / 0.2E1 + t10971 / 0.2E1)) * t2
     #36
        t10987 = (t6911 - t10898) * t94
        t10997 = t573 * t10177
        t11001 = (t6930 - t10901) * t94
        t11016 = (t10954 - t6891) * t236
        t11021 = (t6894 - t10969) * t236
        t11035 = t9173 + t1112 + t9120 + t9133 + t9082 + t9094 + t9101 -
     # t1701 * (t7300 / 0.2E1 + (t7298 - (t7296 - t10909) * t94) * t94 /
     # 0.2E1) / 0.6E1 - t1327 * (t7049 / 0.2E1 + (t7047 - t6214 * ((t109
     #19 / 0.2E1 - t7290 / 0.2E1) * t183 - (t7288 / 0.2E1 - t10924 / 0.2
     #E1) * t183) * t183) * t94 / 0.2E1) / 0.6E1 + (t7054 - t10250 * t68
     #50) * t94 - t1701 * ((t7157 - t6709 * t10604) * t94 + (t7164 - (t7
     #162 - t10946) * t94) * t94) / 0.24E2 - t1228 * (((t10962 - t9141) 
     #* t236 - t10966) * t236 / 0.2E1 + (t10966 - (t9148 - t10977) * t23
     #6) * t236 / 0.2E1) / 0.6E1 - t1701 * ((t3636 * (t6916 - (t1114 / 0
     #.2E1 - t10987 / 0.2E1) * t94) * t94 - t10997) * t183 / 0.2E1 + (t1
     #0997 - t3883 * (t6935 - (t1127 / 0.2E1 - t11001 / 0.2E1) * t94) * 
     #t94) * t183 / 0.2E1) / 0.6E1 - t1228 * (t7185 / 0.2E1 + (t7183 - t
     #6366 * ((t11016 / 0.2E1 - t6896 / 0.2E1) * t236 - (t6893 / 0.2E1 -
     # t11021 / 0.2E1) * t236) * t236) * t94 / 0.2E1) / 0.6E1 + t1099
        t11038 = t161 * (t10897 + t11035) * t118
        t11044 = t6735 / 0.2E1 + t10548 / 0.2E1
        t11046 = t6985 * t11044
        t11048 = t6634 / 0.2E1 + t10288 / 0.2E1
        t11050 = t6214 * t11048
        t11053 = (t11046 - t11050) * t183 / 0.2E1
        t11055 = t6749 / 0.2E1 + t10562 / 0.2E1
        t11057 = t7218 * t11055
        t11060 = (t11050 - t11057) * t183 / 0.2E1
        t11061 = t7468 ** 2
        t11062 = t7460 ** 2
        t11063 = t7464 ** 2
        t11065 = t7481 * (t11061 + t11062 + t11063)
        t11066 = t6468 ** 2
        t11067 = t6460 ** 2
        t11068 = t6464 ** 2
        t11070 = t6481 * (t11066 + t11067 + t11068)
        t11073 = t4 * (t11065 / 0.2E1 + t11070 / 0.2E1)
        t11074 = t11073 * t6520
        t11075 = t7732 ** 2
        t11076 = t7724 ** 2
        t11077 = t7728 ** 2
        t11079 = t7745 * (t11075 + t11076 + t11077)
        t11082 = t4 * (t11070 / 0.2E1 + t11079 / 0.2E1)
        t11083 = t11082 * t6523
        t10529 = t7493 * (t7468 * t7475 + t7460 * t7466 + t7464 * t7462)
        t11091 = t10529 * t7519
        t10533 = t6512 * (t6468 * t6475 + t6460 * t6466 + t6464 * t6462)
        t11097 = t10533 * t6772
        t11100 = (t11091 - t11097) * t183 / 0.2E1
        t10540 = t7757 * (t7732 * t7739 + t7724 * t7730 + t7728 * t7726)
        t11106 = t10540 * t7783
        t11109 = (t11097 - t11106) * t183 / 0.2E1
        t11111 = t6625 / 0.2E1 + t10453 / 0.2E1
        t11113 = t7496 * t11111
        t11115 = t6366 * t11048
        t11118 = (t11113 - t11115) * t236 / 0.2E1
        t11120 = t6646 / 0.2E1 + t10472 / 0.2E1
        t11122 = t7678 * t11120
        t11125 = (t11115 - t11122) * t236 / 0.2E1
        t10553 = t8028 * (t8003 * t8010 + t7995 * t8001 + t7999 * t7997)
        t11131 = t10553 * t8038
        t11133 = t10533 * t6525
        t11136 = (t11131 - t11133) * t236 / 0.2E1
        t10560 = t8226 * (t8201 * t8208 + t8193 * t8199 + t8197 * t8195)
        t11142 = t10560 * t8236
        t11145 = (t11133 - t11142) * t236 / 0.2E1
        t11146 = t8010 ** 2
        t11147 = t8001 ** 2
        t11148 = t7997 ** 2
        t11150 = t8016 * (t11146 + t11147 + t11148)
        t11151 = t6475 ** 2
        t11152 = t6466 ** 2
        t11153 = t6462 ** 2
        t11155 = t6481 * (t11151 + t11152 + t11153)
        t11158 = t4 * (t11150 / 0.2E1 + t11155 / 0.2E1)
        t11159 = t11158 * t6768
        t11160 = t8208 ** 2
        t11161 = t8199 ** 2
        t11162 = t8195 ** 2
        t11164 = t8214 * (t11160 + t11161 + t11162)
        t11167 = t4 * (t11155 / 0.2E1 + t11164 / 0.2E1)
        t11168 = t11167 * t6770
        t11171 = t10301 + t7311 + t10524 / 0.2E1 + t7312 + t10500 / 0.2E
     #1 + t11053 + t11060 + (t11074 - t11083) * t183 + t11100 + t11109 +
     # t11118 + t11125 + t11136 + t11145 + (t11159 - t11168) * t236
        t11172 = t11171 * t6480
        t11174 = (t7442 - t11172) * t94
        t11176 = t7444 / 0.2E1 + t11174 / 0.2E1
        t11177 = t2635 * t11176
        t11185 = t1701 * (t6852 - dx * (t6854 - t10604) / 0.12E2) / 0.12
     #E2
        t11189 = rx(t10214,t180,k,0,0)
        t11190 = rx(t10214,t180,k,1,1)
        t11192 = rx(t10214,t180,k,2,2)
        t11194 = rx(t10214,t180,k,1,2)
        t11196 = rx(t10214,t180,k,2,1)
        t11198 = rx(t10214,t180,k,1,0)
        t11200 = rx(t10214,t180,k,0,2)
        t11202 = rx(t10214,t180,k,0,1)
        t11205 = rx(t10214,t180,k,2,0)
        t11211 = 0.1E1 / (t11189 * t11190 * t11192 - t11189 * t11194 * t
     #11196 + t11198 * t11196 * t11200 - t11198 * t11202 * t11192 + t112
     #05 * t11202 * t11194 - t11205 * t11190 * t11200)
        t11212 = t11189 ** 2
        t11213 = t11202 ** 2
        t11214 = t11200 ** 2
        t11223 = t4 * t11211
        t11228 = u(t10214,t1328,k,n)
        t11242 = u(t10214,t180,t233,n)
        t11245 = u(t10214,t180,t238,n)
        t11255 = rx(t6458,t1328,k,0,0)
        t11256 = rx(t6458,t1328,k,1,1)
        t11258 = rx(t6458,t1328,k,2,2)
        t11260 = rx(t6458,t1328,k,1,2)
        t11262 = rx(t6458,t1328,k,2,1)
        t11264 = rx(t6458,t1328,k,1,0)
        t11266 = rx(t6458,t1328,k,0,2)
        t11268 = rx(t6458,t1328,k,0,1)
        t11271 = rx(t6458,t1328,k,2,0)
        t11277 = 0.1E1 / (t11255 * t11256 * t11258 - t11255 * t11260 * t
     #11262 + t11264 * t11262 * t11266 - t11264 * t11268 * t11258 + t112
     #71 * t11268 * t11260 - t11271 * t11256 * t11266)
        t11278 = t4 * t11277
        t11292 = t11264 ** 2
        t11293 = t11256 ** 2
        t11294 = t11260 ** 2
        t11307 = u(t6458,t1328,t233,n)
        t11310 = u(t6458,t1328,t238,n)
        t11314 = (t11307 - t7498) * t236 / 0.2E1 + (t7498 - t11310) * t2
     #36 / 0.2E1
        t11320 = rx(t6458,t180,t233,0,0)
        t11321 = rx(t6458,t180,t233,1,1)
        t11323 = rx(t6458,t180,t233,2,2)
        t11325 = rx(t6458,t180,t233,1,2)
        t11327 = rx(t6458,t180,t233,2,1)
        t11329 = rx(t6458,t180,t233,1,0)
        t11331 = rx(t6458,t180,t233,0,2)
        t11333 = rx(t6458,t180,t233,0,1)
        t11336 = rx(t6458,t180,t233,2,0)
        t11342 = 0.1E1 / (t11320 * t11321 * t11323 - t11320 * t11325 * t
     #11327 + t11329 * t11327 * t11331 - t11329 * t11333 * t11323 + t113
     #36 * t11333 * t11325 - t11336 * t11321 * t11331)
        t11343 = t4 * t11342
        t11351 = t7619 / 0.2E1 + (t7512 - t11242) * t94 / 0.2E1
        t11355 = t6998 * t11044
        t11359 = rx(t6458,t180,t238,0,0)
        t11360 = rx(t6458,t180,t238,1,1)
        t11362 = rx(t6458,t180,t238,2,2)
        t11364 = rx(t6458,t180,t238,1,2)
        t11366 = rx(t6458,t180,t238,2,1)
        t11368 = rx(t6458,t180,t238,1,0)
        t11370 = rx(t6458,t180,t238,0,2)
        t11372 = rx(t6458,t180,t238,0,1)
        t11375 = rx(t6458,t180,t238,2,0)
        t11381 = 0.1E1 / (t11359 * t11360 * t11362 - t11366 * t11364 * t
     #11359 + t11368 * t11366 * t11370 - t11368 * t11372 * t11362 + t113
     #75 * t11372 * t11364 - t11375 * t11360 * t11370)
        t11382 = t4 * t11381
        t11390 = t7658 / 0.2E1 + (t7515 - t11245) * t94 / 0.2E1
        t11403 = (t11307 - t7512) * t183 / 0.2E1 + t8034 / 0.2E1
        t11407 = t10529 * t7502
        t11418 = (t11310 - t7515) * t183 / 0.2E1 + t8232 / 0.2E1
        t11424 = t11336 ** 2
        t11425 = t11327 ** 2
        t11426 = t11323 ** 2
        t11429 = t7475 ** 2
        t11430 = t7466 ** 2
        t11431 = t7462 ** 2
        t11433 = t7481 * (t11429 + t11430 + t11431)
        t11438 = t11375 ** 2
        t11439 = t11366 ** 2
        t11440 = t11362 ** 2
        t10740 = t11278 * (t11255 * t11264 + t11268 * t11256 + t11266 * 
     #t11260)
        t10772 = t11343 * (t11320 * t11336 + t11333 * t11327 + t11331 * 
     #t11323)
        t10777 = t11382 * (t11359 * t11375 + t11372 * t11366 + t11370 * 
     #t11362)
        t10782 = t11343 * (t11329 * t11336 + t11321 * t11327 + t11325 * 
     #t11323)
        t10787 = t11382 * (t11368 * t11375 + t11360 * t11366 + t11364 * 
     #t11362)
        t11449 = (t7490 - t4 * (t7486 / 0.2E1 + t11211 * (t11212 + t1121
     #3 + t11214) / 0.2E1) * t10548) * t94 + t7507 + (t7504 - t11223 * (
     #t11189 * t11198 + t11202 * t11190 + t11200 * t11194) * ((t11228 - 
     #t10513) * t183 / 0.2E1 + t10515 / 0.2E1)) * t94 / 0.2E1 + t7524 + 
     #(t7521 - t11223 * (t11189 * t11205 + t11202 * t11196 + t11200 * t1
     #1192) * ((t11242 - t10513) * t236 / 0.2E1 + (t10513 - t11245) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + (t10740 * (t7554 / 0.2E1 + (t7498 - t
     #11228) * t94 / 0.2E1) - t11046) * t183 / 0.2E1 + t11053 + (t4 * (t
     #11277 * (t11292 + t11293 + t11294) / 0.2E1 + t11065 / 0.2E1) * t75
     #00 - t11074) * t183 + (t11278 * (t11264 * t11271 + t11256 * t11262
     # + t11260 * t11258) * t11314 - t11091) * t183 / 0.2E1 + t11100 + (
     #t10772 * t11351 - t11355) * t236 / 0.2E1 + (t11355 - t10777 * t113
     #90) * t236 / 0.2E1 + (t10782 * t11403 - t11407) * t236 / 0.2E1 + (
     #t11407 - t10787 * t11418) * t236 / 0.2E1 + (t4 * (t11342 * (t11424
     # + t11425 + t11426) / 0.2E1 + t11433 / 0.2E1) * t7514 - t4 * (t114
     #33 / 0.2E1 + t11381 * (t11438 + t11439 + t11440) / 0.2E1) * t7517)
     # * t236
        t11450 = t11449 * t7480
        t11453 = rx(t10214,t185,k,0,0)
        t11454 = rx(t10214,t185,k,1,1)
        t11456 = rx(t10214,t185,k,2,2)
        t11458 = rx(t10214,t185,k,1,2)
        t11460 = rx(t10214,t185,k,2,1)
        t11462 = rx(t10214,t185,k,1,0)
        t11464 = rx(t10214,t185,k,0,2)
        t11466 = rx(t10214,t185,k,0,1)
        t11469 = rx(t10214,t185,k,2,0)
        t11475 = 0.1E1 / (t11453 * t11454 * t11456 - t11453 * t11458 * t
     #11460 + t11462 * t11460 * t11464 - t11462 * t11466 * t11456 + t114
     #69 * t11466 * t11458 - t11469 * t11454 * t11464)
        t11476 = t11453 ** 2
        t11477 = t11466 ** 2
        t11478 = t11464 ** 2
        t11487 = t4 * t11475
        t11492 = u(t10214,t1335,k,n)
        t11506 = u(t10214,t185,t233,n)
        t11509 = u(t10214,t185,t238,n)
        t11519 = rx(t6458,t1335,k,0,0)
        t11520 = rx(t6458,t1335,k,1,1)
        t11522 = rx(t6458,t1335,k,2,2)
        t11524 = rx(t6458,t1335,k,1,2)
        t11526 = rx(t6458,t1335,k,2,1)
        t11528 = rx(t6458,t1335,k,1,0)
        t11530 = rx(t6458,t1335,k,0,2)
        t11532 = rx(t6458,t1335,k,0,1)
        t11535 = rx(t6458,t1335,k,2,0)
        t11541 = 0.1E1 / (t11519 * t11520 * t11522 - t11519 * t11524 * t
     #11526 + t11528 * t11526 * t11530 - t11528 * t11532 * t11522 + t115
     #35 * t11532 * t11524 - t11535 * t11520 * t11530)
        t11542 = t4 * t11541
        t11556 = t11528 ** 2
        t11557 = t11520 ** 2
        t11558 = t11524 ** 2
        t11571 = u(t6458,t1335,t233,n)
        t11574 = u(t6458,t1335,t238,n)
        t11578 = (t11571 - t7762) * t236 / 0.2E1 + (t7762 - t11574) * t2
     #36 / 0.2E1
        t11584 = rx(t6458,t185,t233,0,0)
        t11585 = rx(t6458,t185,t233,1,1)
        t11587 = rx(t6458,t185,t233,2,2)
        t11589 = rx(t6458,t185,t233,1,2)
        t11591 = rx(t6458,t185,t233,2,1)
        t11593 = rx(t6458,t185,t233,1,0)
        t11595 = rx(t6458,t185,t233,0,2)
        t11597 = rx(t6458,t185,t233,0,1)
        t11600 = rx(t6458,t185,t233,2,0)
        t11606 = 0.1E1 / (t11584 * t11585 * t11587 - t11584 * t11589 * t
     #11591 + t11593 * t11591 * t11595 - t11593 * t11597 * t11587 + t116
     #00 * t11597 * t11589 - t11600 * t11585 * t11595)
        t11607 = t4 * t11606
        t11615 = t7883 / 0.2E1 + (t7776 - t11506) * t94 / 0.2E1
        t11619 = t7230 * t11055
        t11623 = rx(t6458,t185,t238,0,0)
        t11624 = rx(t6458,t185,t238,1,1)
        t11626 = rx(t6458,t185,t238,2,2)
        t11628 = rx(t6458,t185,t238,1,2)
        t11630 = rx(t6458,t185,t238,2,1)
        t11632 = rx(t6458,t185,t238,1,0)
        t11634 = rx(t6458,t185,t238,0,2)
        t11636 = rx(t6458,t185,t238,0,1)
        t11639 = rx(t6458,t185,t238,2,0)
        t11645 = 0.1E1 / (t11623 * t11624 * t11626 - t11623 * t11628 * t
     #11630 + t11632 * t11630 * t11634 - t11632 * t11636 * t11626 + t116
     #39 * t11636 * t11628 - t11639 * t11624 * t11634)
        t11646 = t4 * t11645
        t11654 = t7922 / 0.2E1 + (t7779 - t11509) * t94 / 0.2E1
        t11667 = t8036 / 0.2E1 + (t7776 - t11571) * t183 / 0.2E1
        t11671 = t10540 * t7766
        t11682 = t8234 / 0.2E1 + (t7779 - t11574) * t183 / 0.2E1
        t11688 = t11600 ** 2
        t11689 = t11591 ** 2
        t11690 = t11587 ** 2
        t11693 = t7739 ** 2
        t11694 = t7730 ** 2
        t11695 = t7726 ** 2
        t11697 = t7745 * (t11693 + t11694 + t11695)
        t11702 = t11639 ** 2
        t11703 = t11630 ** 2
        t11704 = t11626 ** 2
        t10949 = t11542 * (t11519 * t11528 + t11532 * t11520 + t11530 * 
     #t11524)
        t10982 = t11607 * (t11584 * t11600 + t11597 * t11591 + t11595 * 
     #t11587)
        t10988 = t11646 * (t11623 * t11639 + t11636 * t11630 + t11634 * 
     #t11626)
        t10993 = t11607 * (t11593 * t11600 + t11585 * t11591 + t11589 * 
     #t11587)
        t10999 = t11646 * (t11632 * t11639 + t11624 * t11630 + t11628 * 
     #t11626)
        t11713 = (t7754 - t4 * (t7750 / 0.2E1 + t11475 * (t11476 + t1147
     #7 + t11478) / 0.2E1) * t10562) * t94 + t7771 + (t7768 - t11487 * (
     #t11453 * t11462 + t11466 * t11454 + t11464 * t11458) * (t10518 / 0
     #.2E1 + (t10516 - t11492) * t183 / 0.2E1)) * t94 / 0.2E1 + t7788 + 
     #(t7785 - t11487 * (t11453 * t11469 + t11466 * t11460 + t11464 * t1
     #1456) * ((t11506 - t10516) * t236 / 0.2E1 + (t10516 - t11509) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + t11060 + (t11057 - t10949 * (t7818 / 
     #0.2E1 + (t7762 - t11492) * t94 / 0.2E1)) * t183 / 0.2E1 + (t11083 
     #- t4 * (t11079 / 0.2E1 + t11541 * (t11556 + t11557 + t11558) / 0.2
     #E1) * t7764) * t183 + t11109 + (t11106 - t11542 * (t11528 * t11535
     # + t11520 * t11526 + t11524 * t11522) * t11578) * t183 / 0.2E1 + (
     #t10982 * t11615 - t11619) * t236 / 0.2E1 + (t11619 - t10988 * t116
     #54) * t236 / 0.2E1 + (t10993 * t11667 - t11671) * t236 / 0.2E1 + (
     #t11671 - t10999 * t11682) * t236 / 0.2E1 + (t4 * (t11606 * (t11688
     # + t11689 + t11690) / 0.2E1 + t11697 / 0.2E1) * t7778 - t4 * (t116
     #97 / 0.2E1 + t11645 * (t11702 + t11703 + t11704) / 0.2E1) * t7781)
     # * t236
        t11714 = t11713 * t7744
        t11724 = rx(t10214,j,t233,0,0)
        t11725 = rx(t10214,j,t233,1,1)
        t11727 = rx(t10214,j,t233,2,2)
        t11729 = rx(t10214,j,t233,1,2)
        t11731 = rx(t10214,j,t233,2,1)
        t11733 = rx(t10214,j,t233,1,0)
        t11735 = rx(t10214,j,t233,0,2)
        t11737 = rx(t10214,j,t233,0,1)
        t11740 = rx(t10214,j,t233,2,0)
        t11746 = 0.1E1 / (t11724 * t11725 * t11727 - t11724 * t11729 * t
     #11731 + t11733 * t11731 * t11735 - t11733 * t11737 * t11727 + t117
     #40 * t11737 * t11729 - t11740 * t11725 * t11735)
        t11747 = t11724 ** 2
        t11748 = t11737 ** 2
        t11749 = t11735 ** 2
        t11758 = t4 * t11746
        t11778 = u(t10214,j,t1229,n)
        t11795 = t7479 * t11111
        t11808 = t11329 ** 2
        t11809 = t11321 ** 2
        t11810 = t11325 ** 2
        t11813 = t8003 ** 2
        t11814 = t7995 ** 2
        t11815 = t7999 ** 2
        t11817 = t8016 * (t11813 + t11814 + t11815)
        t11822 = t11593 ** 2
        t11823 = t11585 ** 2
        t11824 = t11589 ** 2
        t11833 = u(t6458,t180,t1229,n)
        t11837 = (t11833 - t7512) * t236 / 0.2E1 + t7514 / 0.2E1
        t11841 = t10553 * t8052
        t11845 = u(t6458,t185,t1229,n)
        t11849 = (t11845 - t7776) * t236 / 0.2E1 + t7778 / 0.2E1
        t11855 = rx(t6458,j,t1229,0,0)
        t11856 = rx(t6458,j,t1229,1,1)
        t11858 = rx(t6458,j,t1229,2,2)
        t11860 = rx(t6458,j,t1229,1,2)
        t11862 = rx(t6458,j,t1229,2,1)
        t11864 = rx(t6458,j,t1229,1,0)
        t11866 = rx(t6458,j,t1229,0,2)
        t11868 = rx(t6458,j,t1229,0,1)
        t11871 = rx(t6458,j,t1229,2,0)
        t11877 = 0.1E1 / (t11855 * t11856 * t11858 - t11855 * t11860 * t
     #11862 + t11864 * t11862 * t11866 - t11864 * t11868 * t11858 + t118
     #71 * t11868 * t11860 - t11871 * t11856 * t11866)
        t11878 = t4 * t11877
        t11901 = (t11833 - t8048) * t183 / 0.2E1 + (t8048 - t11845) * t1
     #83 / 0.2E1
        t11907 = t11871 ** 2
        t11908 = t11862 ** 2
        t11909 = t11858 ** 2
        t11166 = t11343 * (t11320 * t11329 + t11333 * t11321 + t11331 * 
     #t11325)
        t11178 = t11607 * (t11584 * t11593 + t11597 * t11585 + t11595 * 
     #t11589)
        t11219 = t11878 * (t11855 * t11871 + t11868 * t11862 + t11866 * 
     #t11858)
        t11918 = (t8025 - t4 * (t8021 / 0.2E1 + t11746 * (t11747 + t1174
     #8 + t11749) / 0.2E1) * t10453) * t94 + t8043 + (t8040 - t11758 * (
     #t11724 * t11733 + t11737 * t11725 + t11735 * t11729) * ((t11242 - 
     #t10451) * t183 / 0.2E1 + (t10451 - t11506) * t183 / 0.2E1)) * t94 
     #/ 0.2E1 + t8057 + (t8054 - t11758 * (t11724 * t11740 + t11737 * t1
     #1731 + t11735 * t11727) * ((t11778 - t10451) * t236 / 0.2E1 + t104
     #92 / 0.2E1)) * t94 / 0.2E1 + (t11166 * t11351 - t11795) * t183 / 0
     #.2E1 + (t11795 - t11178 * t11615) * t183 / 0.2E1 + (t4 * (t11342 *
     # (t11808 + t11809 + t11810) / 0.2E1 + t11817 / 0.2E1) * t8034 - t4
     # * (t11817 / 0.2E1 + t11606 * (t11822 + t11823 + t11824) / 0.2E1) 
     #* t8036) * t183 + (t10782 * t11837 - t11841) * t183 / 0.2E1 + (t11
     #841 - t10993 * t11849) * t183 / 0.2E1 + (t11219 * (t8154 / 0.2E1 +
     # (t8048 - t11778) * t94 / 0.2E1) - t11113) * t236 / 0.2E1 + t11118
     # + (t11878 * (t11864 * t11871 + t11856 * t11862 + t11860 * t11858)
     # * t11901 - t11131) * t236 / 0.2E1 + t11136 + (t4 * (t11877 * (t11
     #907 + t11908 + t11909) / 0.2E1 + t11150 / 0.2E1) * t8050 - t11159)
     # * t236
        t11919 = t11918 * t8015
        t11922 = rx(t10214,j,t238,0,0)
        t11923 = rx(t10214,j,t238,1,1)
        t11925 = rx(t10214,j,t238,2,2)
        t11927 = rx(t10214,j,t238,1,2)
        t11929 = rx(t10214,j,t238,2,1)
        t11931 = rx(t10214,j,t238,1,0)
        t11933 = rx(t10214,j,t238,0,2)
        t11935 = rx(t10214,j,t238,0,1)
        t11938 = rx(t10214,j,t238,2,0)
        t11944 = 0.1E1 / (t11922 * t11923 * t11925 - t11922 * t11927 * t
     #11929 + t11931 * t11929 * t11933 - t11931 * t11935 * t11925 + t119
     #38 * t11935 * t11927 - t11938 * t11923 * t11933)
        t11945 = t11922 ** 2
        t11946 = t11935 ** 2
        t11947 = t11933 ** 2
        t11956 = t4 * t11944
        t11976 = u(t10214,j,t1277,n)
        t11993 = t7666 * t11120
        t12006 = t11368 ** 2
        t12007 = t11360 ** 2
        t12008 = t11364 ** 2
        t12011 = t8201 ** 2
        t12012 = t8193 ** 2
        t12013 = t8197 ** 2
        t12015 = t8214 * (t12011 + t12012 + t12013)
        t12020 = t11632 ** 2
        t12021 = t11624 ** 2
        t12022 = t11628 ** 2
        t12031 = u(t6458,t180,t1277,n)
        t12035 = t7517 / 0.2E1 + (t7515 - t12031) * t236 / 0.2E1
        t12039 = t10560 * t8250
        t12043 = u(t6458,t185,t1277,n)
        t12047 = t7781 / 0.2E1 + (t7779 - t12043) * t236 / 0.2E1
        t12053 = rx(t6458,j,t1277,0,0)
        t12054 = rx(t6458,j,t1277,1,1)
        t12056 = rx(t6458,j,t1277,2,2)
        t12058 = rx(t6458,j,t1277,1,2)
        t12060 = rx(t6458,j,t1277,2,1)
        t12062 = rx(t6458,j,t1277,1,0)
        t12064 = rx(t6458,j,t1277,0,2)
        t12066 = rx(t6458,j,t1277,0,1)
        t12069 = rx(t6458,j,t1277,2,0)
        t12075 = 0.1E1 / (t12053 * t12054 * t12056 - t12053 * t12058 * t
     #12060 + t12062 * t12060 * t12064 - t12062 * t12066 * t12056 + t120
     #69 * t12066 * t12058 - t12069 * t12054 * t12064)
        t12076 = t4 * t12075
        t12099 = (t12031 - t8246) * t183 / 0.2E1 + (t8246 - t12043) * t1
     #83 / 0.2E1
        t12105 = t12069 ** 2
        t12106 = t12060 ** 2
        t12107 = t12056 ** 2
        t11371 = t11382 * (t11359 * t11368 + t11372 * t11360 + t11370 * 
     #t11364)
        t11378 = t11646 * (t11623 * t11632 + t11636 * t11624 + t11634 * 
     #t11628)
        t11412 = t12076 * (t12053 * t12069 + t12066 * t12060 + t12064 * 
     #t12056)
        t12116 = (t8223 - t4 * (t8219 / 0.2E1 + t11944 * (t11945 + t1194
     #6 + t11947) / 0.2E1) * t10472) * t94 + t8241 + (t8238 - t11956 * (
     #t11922 * t11931 + t11935 * t11923 + t11927 * t11933) * ((t11245 - 
     #t10470) * t183 / 0.2E1 + (t10470 - t11509) * t183 / 0.2E1)) * t94 
     #/ 0.2E1 + t8255 + (t8252 - t11956 * (t11922 * t11938 + t11935 * t1
     #1929 + t11933 * t11925) * (t10494 / 0.2E1 + (t10470 - t11976) * t2
     #36 / 0.2E1)) * t94 / 0.2E1 + (t11371 * t11390 - t11993) * t183 / 0
     #.2E1 + (t11993 - t11378 * t11654) * t183 / 0.2E1 + (t4 * (t11381 *
     # (t12006 + t12007 + t12008) / 0.2E1 + t12015 / 0.2E1) * t8232 - t4
     # * (t12015 / 0.2E1 + t11645 * (t12020 + t12021 + t12022) / 0.2E1) 
     #* t8234) * t183 + (t10787 * t12035 - t12039) * t183 / 0.2E1 + (t12
     #039 - t10999 * t12047) * t183 / 0.2E1 + t11125 + (t11122 - t11412 
     #* (t8352 / 0.2E1 + (t8246 - t11976) * t94 / 0.2E1)) * t236 / 0.2E1
     # + t11145 + (t11142 - t12076 * (t12062 * t12069 + t12054 * t12060 
     #+ t12058 * t12056) * t12099) * t236 / 0.2E1 + (t11168 - t4 * (t111
     #64 / 0.2E1 + t12075 * (t12105 + t12106 + t12107) / 0.2E1) * t8248)
     # * t236
        t12117 = t12116 * t8213
        t12134 = t573 * t11176
        t12151 = t11320 ** 2
        t12152 = t11333 ** 2
        t12153 = t11331 ** 2
        t12172 = rx(t96,t1328,t233,0,0)
        t12173 = rx(t96,t1328,t233,1,1)
        t12175 = rx(t96,t1328,t233,2,2)
        t12177 = rx(t96,t1328,t233,1,2)
        t12179 = rx(t96,t1328,t233,2,1)
        t12181 = rx(t96,t1328,t233,1,0)
        t12183 = rx(t96,t1328,t233,0,2)
        t12185 = rx(t96,t1328,t233,0,1)
        t12188 = rx(t96,t1328,t233,2,0)
        t12194 = 0.1E1 / (t12172 * t12173 * t12175 - t12172 * t12177 * t
     #12179 + t12181 * t12179 * t12183 - t12181 * t12185 * t12175 + t121
     #88 * t12185 * t12177 - t12188 * t12173 * t12183)
        t12195 = t4 * t12194
        t12203 = t8471 / 0.2E1 + (t7577 - t11307) * t94 / 0.2E1
        t12209 = t12181 ** 2
        t12210 = t12173 ** 2
        t12211 = t12177 ** 2
        t12224 = u(t96,t1328,t1229,n)
        t12228 = (t12224 - t7577) * t236 / 0.2E1 + t7579 / 0.2E1
        t12234 = rx(t96,t180,t1229,0,0)
        t12235 = rx(t96,t180,t1229,1,1)
        t12237 = rx(t96,t180,t1229,2,2)
        t12239 = rx(t96,t180,t1229,1,2)
        t12241 = rx(t96,t180,t1229,2,1)
        t12243 = rx(t96,t180,t1229,1,0)
        t12245 = rx(t96,t180,t1229,0,2)
        t12247 = rx(t96,t180,t1229,0,1)
        t12250 = rx(t96,t180,t1229,2,0)
        t12256 = 0.1E1 / (t12234 * t12235 * t12237 - t12234 * t12239 * t
     #12241 + t12243 * t12241 * t12245 - t12243 * t12247 * t12237 + t122
     #50 * t12247 * t12239 - t12250 * t12235 * t12245)
        t12257 = t4 * t12256
        t12265 = t8533 / 0.2E1 + (t8103 - t11833) * t94 / 0.2E1
        t12278 = (t12224 - t8103) * t183 / 0.2E1 + t8167 / 0.2E1
        t12284 = t12250 ** 2
        t12285 = t12241 ** 2
        t12286 = t12237 ** 2
        t11547 = t12195 * (t12181 * t12172 + t12185 * t12173 + t12183 * 
     #t12177)
        t11563 = t12195 * (t12181 * t12188 + t12173 * t12179 + t12177 * 
     #t12175)
        t11568 = t12257 * (t12234 * t12250 + t12247 * t12241 + t12237 * 
     #t12245)
        t11575 = t12257 * (t12243 * t12250 + t12241 * t12235 + t12239 * 
     #t12237)
        t12295 = (t8429 - t4 * (t8425 / 0.2E1 + t11342 * (t12151 + t1215
     #2 + t12153) / 0.2E1) * t7619) * t94 + t8436 + (t8433 - t11166 * t1
     #1403) * t94 / 0.2E1 + t8441 + (t8438 - t10772 * t11837) * t94 / 0.
     #2E1 + (t11547 * t12203 - t8063) * t183 / 0.2E1 + t8068 + (t4 * (t1
     #2194 * (t12209 + t12210 + t12211) / 0.2E1 + t8082 / 0.2E1) * t7671
     # - t8091) * t183 + (t11563 * t12228 - t8109) * t183 / 0.2E1 + t811
     #4 + (t11568 * t12265 - t7623) * t236 / 0.2E1 + t7628 + (t11575 * t
     #12278 - t7675) * t236 / 0.2E1 + t7680 + (t4 * (t12256 * (t12284 + 
     #t12285 + t12286) / 0.2E1 + t7698 / 0.2E1) * t8105 - t7707) * t236
        t12296 = t12295 * t7611
        t12299 = t11359 ** 2
        t12300 = t11372 ** 2
        t12301 = t11370 ** 2
        t12320 = rx(t96,t1328,t238,0,0)
        t12321 = rx(t96,t1328,t238,1,1)
        t12323 = rx(t96,t1328,t238,2,2)
        t12325 = rx(t96,t1328,t238,1,2)
        t12327 = rx(t96,t1328,t238,2,1)
        t12329 = rx(t96,t1328,t238,1,0)
        t12331 = rx(t96,t1328,t238,0,2)
        t12333 = rx(t96,t1328,t238,0,1)
        t12336 = rx(t96,t1328,t238,2,0)
        t12342 = 0.1E1 / (t12320 * t12321 * t12323 - t12320 * t12325 * t
     #12327 + t12327 * t12329 * t12331 - t12329 * t12333 * t12323 + t123
     #36 * t12333 * t12325 - t12336 * t12321 * t12331)
        t12343 = t4 * t12342
        t12351 = t8619 / 0.2E1 + (t7580 - t11310) * t94 / 0.2E1
        t12357 = t12329 ** 2
        t12358 = t12321 ** 2
        t12359 = t12325 ** 2
        t12372 = u(t96,t1328,t1277,n)
        t12376 = t7582 / 0.2E1 + (t7580 - t12372) * t236 / 0.2E1
        t12382 = rx(t96,t180,t1277,0,0)
        t12383 = rx(t96,t180,t1277,1,1)
        t12385 = rx(t96,t180,t1277,2,2)
        t12387 = rx(t96,t180,t1277,1,2)
        t12389 = rx(t96,t180,t1277,2,1)
        t12391 = rx(t96,t180,t1277,1,0)
        t12393 = rx(t96,t180,t1277,0,2)
        t12395 = rx(t96,t180,t1277,0,1)
        t12398 = rx(t96,t180,t1277,2,0)
        t12404 = 0.1E1 / (t12382 * t12383 * t12385 - t12382 * t12387 * t
     #12389 + t12391 * t12389 * t12393 - t12391 * t12395 * t12385 + t123
     #98 * t12395 * t12387 - t12398 * t12383 * t12393)
        t12405 = t4 * t12404
        t12413 = t8681 / 0.2E1 + (t8301 - t12031) * t94 / 0.2E1
        t12426 = (t12372 - t8301) * t183 / 0.2E1 + t8365 / 0.2E1
        t12432 = t12398 ** 2
        t12433 = t12389 ** 2
        t12434 = t12385 ** 2
        t11687 = t12343 * (t12320 * t12329 + t12333 * t12321 + t12331 * 
     #t12325)
        t11710 = t12343 * (t12329 * t12336 + t12321 * t12327 + t12325 * 
     #t12323)
        t11717 = t12405 * (t12382 * t12398 + t12395 * t12389 + t12393 * 
     #t12385)
        t11722 = t12405 * (t12391 * t12398 + t12383 * t12389 + t12387 * 
     #t12385)
        t12443 = (t8577 - t4 * (t8573 / 0.2E1 + t11381 * (t12299 + t1230
     #0 + t12301) / 0.2E1) * t7658) * t94 + t8584 + (t8581 - t11371 * t1
     #1418) * t94 / 0.2E1 + t8589 + (t8586 - t10777 * t12035) * t94 / 0.
     #2E1 + (t11687 * t12351 - t8261) * t183 / 0.2E1 + t8266 + (t4 * (t1
     #2342 * (t12357 + t12358 + t12359) / 0.2E1 + t8280 / 0.2E1) * t7686
     # - t8289) * t183 + (t11710 * t12376 - t8307) * t183 / 0.2E1 + t831
     #2 + t7665 + (t7662 - t11717 * t12413) * t236 / 0.2E1 + t7693 + (t7
     #690 - t11722 * t12426) * t236 / 0.2E1 + (t7716 - t4 * (t7712 / 0.2
     #E1 + t12404 * (t12432 + t12433 + t12434) / 0.2E1) * t8303) * t236
        t12444 = t12443 * t7650
        t12448 = (t12296 - t7720) * t236 / 0.2E1 + (t7720 - t12444) * t2
     #36 / 0.2E1
        t12452 = t6882 * t8391
        t12456 = t11584 ** 2
        t12457 = t11597 ** 2
        t12458 = t11595 ** 2
        t12477 = rx(t96,t1335,t233,0,0)
        t12478 = rx(t96,t1335,t233,1,1)
        t12480 = rx(t96,t1335,t233,2,2)
        t12482 = rx(t96,t1335,t233,1,2)
        t12484 = rx(t96,t1335,t233,2,1)
        t12486 = rx(t96,t1335,t233,1,0)
        t12488 = rx(t96,t1335,t233,0,2)
        t12490 = rx(t96,t1335,t233,0,1)
        t12493 = rx(t96,t1335,t233,2,0)
        t12499 = 0.1E1 / (t12478 * t12477 * t12480 - t12477 * t12482 * t
     #12484 + t12486 * t12484 * t12488 - t12486 * t12490 * t12480 + t124
     #93 * t12490 * t12482 - t12493 * t12478 * t12488)
        t12500 = t4 * t12499
        t12508 = t8776 / 0.2E1 + (t7841 - t11571) * t94 / 0.2E1
        t12514 = t12486 ** 2
        t12515 = t12478 ** 2
        t12516 = t12482 ** 2
        t12529 = u(t96,t1335,t1229,n)
        t12533 = (t12529 - t7841) * t236 / 0.2E1 + t7843 / 0.2E1
        t12539 = rx(t96,t185,t1229,0,0)
        t12540 = rx(t96,t185,t1229,1,1)
        t12542 = rx(t96,t185,t1229,2,2)
        t12544 = rx(t96,t185,t1229,1,2)
        t12546 = rx(t96,t185,t1229,2,1)
        t12548 = rx(t96,t185,t1229,1,0)
        t12550 = rx(t96,t185,t1229,0,2)
        t12552 = rx(t96,t185,t1229,0,1)
        t12555 = rx(t96,t185,t1229,2,0)
        t12561 = 0.1E1 / (t12539 * t12540 * t12542 - t12539 * t12544 * t
     #12546 + t12548 * t12546 * t12550 - t12548 * t12552 * t12542 + t125
     #55 * t12552 * t12544 - t12555 * t12540 * t12550)
        t12562 = t4 * t12561
        t12570 = t8838 / 0.2E1 + (t8115 - t11845) * t94 / 0.2E1
        t12583 = t8169 / 0.2E1 + (t8115 - t12529) * t183 / 0.2E1
        t12589 = t12555 ** 2
        t12590 = t12546 ** 2
        t12591 = t12542 ** 2
        t11835 = t12500 * (t12477 * t12486 + t12490 * t12478 + t12488 * 
     #t12482)
        t11852 = t12500 * (t12486 * t12493 + t12478 * t12484 + t12482 * 
     #t12480)
        t11861 = t12562 * (t12539 * t12555 + t12552 * t12546 + t12550 * 
     #t12542)
        t11870 = t12562 * (t12548 * t12555 + t12546 * t12540 + t12544 * 
     #t12542)
        t12600 = (t8734 - t4 * (t8730 / 0.2E1 + t11606 * (t12456 + t1245
     #7 + t12458) / 0.2E1) * t7883) * t94 + t8741 + (t8738 - t11178 * t1
     #1667) * t94 / 0.2E1 + t8746 + (t8743 - t10982 * t11849) * t94 / 0.
     #2E1 + t8077 + (t8074 - t11835 * t12508) * t183 / 0.2E1 + (t8100 - 
     #t4 * (t8096 / 0.2E1 + t12499 * (t12514 + t12515 + t12516) / 0.2E1)
     # * t7935) * t183 + t8124 + (t8121 - t11852 * t12533) * t183 / 0.2E
     #1 + (t11861 * t12570 - t7887) * t236 / 0.2E1 + t7892 + (t11870 * t
     #12583 - t7939) * t236 / 0.2E1 + t7944 + (t4 * (t12561 * (t12589 + 
     #t12590 + t12591) / 0.2E1 + t7962 / 0.2E1) * t8117 - t7971) * t236
        t12601 = t12600 * t7875
        t12604 = t11623 ** 2
        t12605 = t11636 ** 2
        t12606 = t11634 ** 2
        t12625 = rx(t96,t1335,t238,0,0)
        t12626 = rx(t96,t1335,t238,1,1)
        t12628 = rx(t96,t1335,t238,2,2)
        t12630 = rx(t96,t1335,t238,1,2)
        t12632 = rx(t96,t1335,t238,2,1)
        t12634 = rx(t96,t1335,t238,1,0)
        t12636 = rx(t96,t1335,t238,0,2)
        t12638 = rx(t96,t1335,t238,0,1)
        t12641 = rx(t96,t1335,t238,2,0)
        t12647 = 0.1E1 / (t12625 * t12626 * t12628 - t12625 * t12630 * t
     #12632 + t12634 * t12632 * t12636 - t12634 * t12638 * t12628 + t126
     #41 * t12638 * t12630 - t12641 * t12626 * t12636)
        t12648 = t4 * t12647
        t12656 = t8924 / 0.2E1 + (t7844 - t11574) * t94 / 0.2E1
        t12662 = t12634 ** 2
        t12663 = t12626 ** 2
        t12664 = t12630 ** 2
        t12677 = u(t96,t1335,t1277,n)
        t12681 = t7846 / 0.2E1 + (t7844 - t12677) * t236 / 0.2E1
        t12687 = rx(t96,t185,t1277,0,0)
        t12688 = rx(t96,t185,t1277,1,1)
        t12690 = rx(t96,t185,t1277,2,2)
        t12692 = rx(t96,t185,t1277,1,2)
        t12694 = rx(t96,t185,t1277,2,1)
        t12696 = rx(t96,t185,t1277,1,0)
        t12698 = rx(t96,t185,t1277,0,2)
        t12700 = rx(t96,t185,t1277,0,1)
        t12703 = rx(t96,t185,t1277,2,0)
        t12709 = 0.1E1 / (t12687 * t12688 * t12690 - t12687 * t12692 * t
     #12694 + t12696 * t12694 * t12698 - t12696 * t12700 * t12690 + t127
     #03 * t12700 * t12692 - t12703 * t12688 * t12698)
        t12710 = t4 * t12709
        t12718 = t8986 / 0.2E1 + (t8313 - t12043) * t94 / 0.2E1
        t12731 = t8367 / 0.2E1 + (t8313 - t12677) * t183 / 0.2E1
        t12737 = t12703 ** 2
        t12738 = t12694 ** 2
        t12739 = t12690 ** 2
        t11973 = t12648 * (t12625 * t12634 + t12638 * t12626 + t12636 * 
     #t12630)
        t11987 = t12648 * (t12634 * t12641 + t12626 * t12632 + t12630 * 
     #t12628)
        t11994 = t12710 * (t12687 * t12703 + t12700 * t12694 + t12698 * 
     #t12690)
        t11999 = t12710 * (t12703 * t12696 + t12688 * t12694 + t12692 * 
     #t12690)
        t12748 = (t8882 - t4 * (t8878 / 0.2E1 + t11645 * (t12604 + t1260
     #5 + t12606) / 0.2E1) * t7922) * t94 + t8889 + (t8886 - t11378 * t1
     #1682) * t94 / 0.2E1 + t8894 + (t8891 - t10988 * t12047) * t94 / 0.
     #2E1 + t8275 + (t8272 - t11973 * t12656) * t183 / 0.2E1 + (t8298 - 
     #t4 * (t8294 / 0.2E1 + t12647 * (t12662 + t12663 + t12664) / 0.2E1)
     # * t7950) * t183 + t8322 + (t8319 - t11987 * t12681) * t183 / 0.2E
     #1 + t7929 + (t7926 - t11994 * t12718) * t236 / 0.2E1 + t7957 + (t7
     #954 - t11999 * t12731) * t236 / 0.2E1 + (t7980 - t4 * (t7976 / 0.2
     #E1 + t12709 * (t12737 + t12738 + t12739) / 0.2E1) * t8315) * t236
        t12749 = t12748 * t7914
        t12753 = (t12601 - t7984) * t236 / 0.2E1 + (t7984 - t12749) * t2
     #36 / 0.2E1
        t12766 = t590 * t11176
        t12784 = (t12296 - t8189) * t183 / 0.2E1 + (t8189 - t12601) * t1
     #83 / 0.2E1
        t12788 = t6882 * t7988
        t12797 = (t12444 - t8387) * t183 / 0.2E1 + (t8387 - t12749) * t1
     #83 / 0.2E1
        t12807 = (t7456 - t6709 * t11174) * t94 + t7993 + (t7990 - t6214
     # * ((t11450 - t11172) * t183 / 0.2E1 + (t11172 - t11714) * t183 / 
     #0.2E1)) * t94 / 0.2E1 + t8396 + (t8393 - t6366 * ((t11919 - t11172
     #) * t236 / 0.2E1 + (t11172 - t12117) * t236 / 0.2E1)) * t94 / 0.2E
     #1 + (t3636 * (t8398 / 0.2E1 + (t7720 - t11450) * t94 / 0.2E1) - t1
     #2134) * t183 / 0.2E1 + (t12134 - t3883 * (t8409 / 0.2E1 + (t7984 -
     # t11714) * t94 / 0.2E1)) * t183 / 0.2E1 + (t7343 * t7722 - t7352 *
     # t7986) * t183 + (t6869 * t12448 - t12452) * t183 / 0.2E1 + (t1245
     #2 - t6890 * t12753) * t183 / 0.2E1 + (t4813 * (t9030 / 0.2E1 + (t8
     #189 - t11919) * t94 / 0.2E1) - t12766) * t236 / 0.2E1 + (t12766 - 
     #t4953 * (t9041 / 0.2E1 + (t8387 - t12117) * t94 / 0.2E1)) * t236 /
     # 0.2E1 + (t6908 * t12784 - t12788) * t236 / 0.2E1 + (t12788 - t691
     #5 * t12797) * t236 / 0.2E1 + (t7428 * t8191 - t7437 * t8389) * t23
     #6
        t12809 = t895 * t12807 * t118
        t12819 = t6850 / 0.2E1 + t10600 / 0.2E1
        t12821 = t6214 * t12819
        t12836 = ut(t6458,t180,t233,n)
        t12839 = ut(t6458,t180,t238,n)
        t12843 = (t12836 - t6911) * t236 / 0.2E1 + (t6911 - t12839) * t2
     #36 / 0.2E1
        t12847 = t10533 * t6898
        t12851 = ut(t6458,t185,t233,n)
        t12854 = ut(t6458,t185,t238,n)
        t12858 = (t12851 - t6930) * t236 / 0.2E1 + (t6930 - t12854) * t2
     #36 / 0.2E1
        t12869 = t6366 * t12819
        t12885 = (t12836 - t6891) * t183 / 0.2E1 + (t6891 - t12851) * t1
     #83 / 0.2E1
        t12889 = t10533 * t7292
        t12898 = (t12839 - t6894) * t183 / 0.2E1 + (t6894 - t12854) * t1
     #83 / 0.2E1
        t12908 = t10946 + t9082 + t10909 / 0.2E1 + t9083 + t10869 / 0.2E
     #1 + (t6985 * (t6913 / 0.2E1 + t10987 / 0.2E1) - t12821) * t183 / 0
     #.2E1 + (t12821 - t7218 * (t6932 / 0.2E1 + t11001 / 0.2E1)) * t183 
     #/ 0.2E1 + (t11073 * t7288 - t11082 * t7290) * t183 + (t10529 * t12
     #843 - t12847) * t183 / 0.2E1 + (t12847 - t10540 * t12858) * t183 /
     # 0.2E1 + (t7496 * (t7225 / 0.2E1 + t10750 / 0.2E1) - t12869) * t23
     #6 / 0.2E1 + (t12869 - t7678 * (t7239 / 0.2E1 + t10769 / 0.2E1)) * 
     #t236 / 0.2E1 + (t10553 * t12885 - t12889) * t236 / 0.2E1 + (t12889
     # - t10560 * t12898) * t236 / 0.2E1 + (t11158 * t6893 - t11167 * t6
     #896) * t236
        t12914 = t6305 * (t9181 / 0.2E1 + (t9179 - t12908 * t6480) * t94
     # / 0.2E1)
        t12918 = t2635 * (t7444 - t11174)
        t12921 = t2 + t6846 - t6861 + t7310 - t7449 + t7455 + t9081 - t9
     #186 + t9190 - t145 - t1227 * t10595 - t10611 - t2079 * t11038 / 0.
     #2E1 - t1227 * t11177 / 0.2E1 - t11185 - t2923 * t12809 / 0.6E1 - t
     #2079 * t12914 / 0.4E1 - t1227 * t12918 / 0.12E2
        t12934 = sqrt(t9195 + t9196 + t9197 + 0.8E1 * t120 + 0.8E1 * t12
     #1 + 0.8E1 * t122 - 0.2E1 * dx * ((t29 + t30 + t31 - t58 - t59 - t6
     #0) * t94 / 0.2E1 - (t120 + t121 + t122 - t6482 - t6483 - t6484) * 
     #t94 / 0.2E1))
        t12935 = 0.1E1 / t12934
        t12940 = t6494 * t9213 * t10169
        t12943 = t568 * t9216 * t10174 / 0.2E1
        t12946 = t568 * t9220 * t10179 / 0.6E1
        t12948 = t9213 * t10182 / 0.24E2
        t12960 = t2 + t9240 - t6861 + t9242 - t9244 + t7455 + t9246 - t9
     #248 + t9250 - t145 - t9226 * t10595 - t10611 - t9228 * t11038 / 0.
     #2E1 - t9226 * t11177 / 0.2E1 - t11185 - t9233 * t12809 / 0.6E1 - t
     #9228 * t12914 / 0.4E1 - t9226 * t12918 / 0.12E2
        t12963 = 0.2E1 * t10185 * t12960 * t12935
        t12965 = (t6494 * t136 * t10169 + t568 * t159 * t10174 / 0.2E1 +
     # t568 * t893 * t10179 / 0.6E1 - t136 * t10182 / 0.24E2 + 0.2E1 * t
     #10185 * t12921 * t12935 - t12940 - t12943 - t12946 + t12948 - t129
     #63) * t133
        t12971 = t6494 * (t571 - dx * t6702 / 0.24E2)
        t12973 = dx * t6713 / 0.24E2
        t12989 = t4 * (t9272 + t9276 / 0.2E1 - dx * ((t9269 - t9271) * t
     #94 / 0.2E1 - (t9276 - t6481 * t6516) * t94 / 0.2E1) / 0.8E1)
        t13000 = (t7288 - t7290) * t183
        t13017 = t9291 + t9292 - t9296 + t1089 / 0.4E1 + t1092 / 0.4E1 -
     # t9335 / 0.12E2 - dx * ((t9313 + t9314 - t9315 - t9318 - t9319 + t
     #9320) * t94 / 0.2E1 - (t9321 + t9322 - t9336 - t7288 / 0.2E1 - t72
     #90 / 0.2E1 + t1327 * (((t10919 - t7288) * t183 - t13000) * t183 / 
     #0.2E1 + (t13000 - (t7290 - t10924) * t183) * t183 / 0.2E1) / 0.6E1
     #) * t94 / 0.2E1) / 0.8E1
        t13022 = t4 * (t9271 / 0.2E1 + t9276 / 0.2E1)
        t13024 = t4134 / 0.4E1 + t4398 / 0.4E1 + t7722 / 0.4E1 + t7986 /
     # 0.4E1
        t13030 = (t9524 - t7489 * t6913) * t94
        t13036 = (t9530 - t6985 * (t10919 / 0.2E1 + t7288 / 0.2E1)) * t9
     #4
        t13041 = (t9535 - t6998 * t12843) * t94
        t13046 = (t9106 - t12836) * t94
        t13048 = t9542 / 0.2E1 + t13046 / 0.2E1
        t13052 = t3651 * t9085
        t13057 = (t9109 - t12839) * t94
        t13059 = t9553 / 0.2E1 + t13057 / 0.2E1
        t13066 = t10822 / 0.2E1 + t9151 / 0.2E1
        t13070 = t6869 * t9528
        t13075 = t10841 / 0.2E1 + t9164 / 0.2E1
        t13085 = t13030 + t9533 + t13036 / 0.2E1 + t9538 + t13041 / 0.2E
     #1 + t10686 / 0.2E1 + t9094 + t10661 + t10721 / 0.2E1 + t9120 + (t7
     #087 * t13048 - t13052) * t236 / 0.2E1 + (t13052 - t7131 * t13059) 
     #* t236 / 0.2E1 + (t7147 * t13066 - t13070) * t236 / 0.2E1 + (t1307
     #0 - t7159 * t13075) * t236 / 0.2E1 + (t7706 * t9108 - t7715 * t911
     #1) * t236
        t13086 = t13085 * t3892
        t13091 = (t9585 - t7753 * t6932) * t94
        t13097 = (t9591 - t7218 * (t7290 / 0.2E1 + t10924 / 0.2E1)) * t9
     #4
        t13102 = (t9596 - t7230 * t12858) * t94
        t13107 = (t9121 - t12851) * t94
        t13109 = t9603 / 0.2E1 + t13107 / 0.2E1
        t13113 = t3899 * t9096
        t13118 = (t9124 - t12854) * t94
        t13120 = t9614 / 0.2E1 + t13118 / 0.2E1
        t13127 = t9153 / 0.2E1 + t10827 / 0.2E1
        t13131 = t6890 * t9589
        t13136 = t9166 / 0.2E1 + t10846 / 0.2E1
        t13146 = t13091 + t9594 + t13097 / 0.2E1 + t9599 + t13102 / 0.2E
     #1 + t9101 + t10701 / 0.2E1 + t10666 + t9133 + t10739 / 0.2E1 + (t7
     #317 * t13109 - t13113) * t236 / 0.2E1 + (t13113 - t7364 * t13120) 
     #* t236 / 0.2E1 + (t7380 * t13127 - t13131) * t236 / 0.2E1 + (t1313
     #1 - t7397 * t13136) * t236 / 0.2E1 + (t7970 * t9123 - t7979 * t912
     #6) * t236
        t13147 = t13146 * t4156
        t13151 = t9584 / 0.4E1 + t9645 / 0.4E1 + (t13086 - t9179) * t183
     # / 0.4E1 + (t9179 - t13147) * t183 / 0.4E1
        t13157 = dx * (t934 / 0.2E1 - t7296 / 0.2E1)
        t13161 = t12989 * t9213 * t13017
        t13164 = t13022 * t9658 * t13024 / 0.2E1
        t13167 = t13022 * t9662 * t13151 / 0.6E1
        t13169 = t9213 * t13157 / 0.24E2
        t13171 = (t12989 * t136 * t13017 + t13022 * t9349 * t13024 / 0.2
     #E1 + t13022 * t9355 * t13151 / 0.6E1 - t136 * t13157 / 0.24E2 - t1
     #3161 - t13164 - t13167 + t13169) * t133
        t13184 = (t6520 - t6523) * t183
        t13202 = t12989 * (t9678 + t9679 - t9683 + t582 / 0.4E1 + t585 /
     # 0.4E1 - t9722 / 0.12E2 - dx * ((t9700 + t9701 - t9702 - t9705 - t
     #9706 + t9707) * t94 / 0.2E1 - (t9708 + t9709 - t9723 - t6520 / 0.2
     #E1 - t6523 / 0.2E1 + t1327 * (((t7500 - t6520) * t183 - t13184) * 
     #t183 / 0.2E1 + (t13184 - (t6523 - t7764) * t183) * t183 / 0.2E1) /
     # 0.6E1) * t94 / 0.2E1) / 0.8E1)
        t13206 = dx * (t227 / 0.2E1 - t6529 / 0.2E1) / 0.24E2
        t13222 = t4 * (t9743 + t9747 / 0.2E1 - dx * ((t9740 - t9742) * t
     #94 / 0.2E1 - (t9747 - t6481 * t6766) * t94 / 0.2E1) / 0.8E1)
        t13233 = (t6893 - t6896) * t236
        t13250 = t9762 + t9763 - t9767 + t1102 / 0.4E1 + t1105 / 0.4E1 -
     # t9806 / 0.12E2 - dx * ((t9784 + t9785 - t9786 - t9789 - t9790 + t
     #9791) * t94 / 0.2E1 - (t9792 + t9793 - t9807 - t6893 / 0.2E1 - t68
     #96 / 0.2E1 + t1228 * (((t11016 - t6893) * t236 - t13233) * t236 / 
     #0.2E1 + (t13233 - (t6896 - t11021) * t236) * t236 / 0.2E1) / 0.6E1
     #) * t94 / 0.2E1) / 0.8E1
        t13255 = t4 * (t9742 / 0.2E1 + t9747 / 0.2E1)
        t13257 = t5278 / 0.4E1 + t5476 / 0.4E1 + t8191 / 0.4E1 + t8389 /
     # 0.4E1
        t13263 = (t9969 - t8024 * t7225) * t94
        t13267 = (t9973 - t7479 * t12885) * t94
        t13274 = (t9980 - t7496 * (t11016 / 0.2E1 + t6893 / 0.2E1)) * t9
     #4
        t13279 = t4804 * t9135
        t13293 = t10614 / 0.2E1 + t9108 / 0.2E1
        t13297 = t6908 * t9978
        t13302 = t10635 / 0.2E1 + t9123 / 0.2E1
        t13310 = t13263 + t9976 + t13267 / 0.2E1 + t9983 + t13274 / 0.2E
     #1 + (t7508 * t13048 - t13279) * t183 / 0.2E1 + (t13279 - t7520 * t
     #13109) * t183 / 0.2E1 + (t8090 * t9151 - t8099 * t9153) * t183 + (
     #t7147 * t13293 - t13297) * t183 / 0.2E1 + (t13297 - t7380 * t13302
     #) * t183 / 0.2E1 + t10962 / 0.2E1 + t9142 + t10796 / 0.2E1 + t9162
     # + t10884
        t13311 = t13310 * t5102
        t13316 = (t10022 - t8222 * t7239) * t94
        t13320 = (t10026 - t7666 * t12898) * t94
        t13327 = (t10033 - t7678 * (t6896 / 0.2E1 + t11021 / 0.2E1)) * t
     #94
        t13332 = t4931 * t9144
        t13346 = t9111 / 0.2E1 + t10620 / 0.2E1
        t13350 = t6915 * t10031
        t13355 = t9126 / 0.2E1 + t10641 / 0.2E1
        t13363 = t13316 + t10029 + t13320 / 0.2E1 + t10036 + t13327 / 0.
     #2E1 + (t7685 * t13059 - t13332) * t183 / 0.2E1 + (t13332 - t7702 *
     # t13120) * t183 / 0.2E1 + (t8288 * t9164 - t8297 * t9166) * t183 +
     # (t7159 * t13346 - t13350) * t183 / 0.2E1 + (t13350 - t7397 * t133
     #55) * t183 / 0.2E1 + t9149 + t10977 / 0.2E1 + t9173 + t10812 / 0.2
     #E1 + t10889
        t13364 = t13363 * t5300
        t13368 = t10021 / 0.4E1 + t10074 / 0.4E1 + (t13311 - t9179) * t2
     #36 / 0.4E1 + (t9179 - t13364) * t236 / 0.4E1
        t13374 = dx * (t970 / 0.2E1 - t6902 / 0.2E1)
        t13378 = t13222 * t9213 * t13250
        t13381 = t13255 * t9658 * t13257 / 0.2E1
        t13384 = t13255 * t9662 * t13368 / 0.6E1
        t13386 = t9213 * t13374 / 0.24E2
        t13388 = (t13222 * t136 * t13250 + t13255 * t9349 * t13257 / 0.2
     #E1 + t13255 * t9355 * t13368 / 0.6E1 - t136 * t13374 / 0.24E2 - t1
     #3378 - t13381 - t13384 + t13386) * t133
        t13401 = (t6768 - t6770) * t236
        t13419 = t13222 * (t10105 + t10106 - t10110 + t599 / 0.4E1 + t60
     #2 / 0.4E1 - t10149 / 0.12E2 - dx * ((t10127 + t10128 - t10129 - t1
     #0132 - t10133 + t10134) * t94 / 0.2E1 - (t10135 + t10136 - t10150 
     #- t6768 / 0.2E1 - t6770 / 0.2E1 + t1228 * (((t8050 - t6768) * t236
     # - t13401) * t236 / 0.2E1 + (t13401 - (t6770 - t8248) * t236) * t2
     #36 / 0.2E1) / 0.6E1) * t94 / 0.2E1) / 0.8E1)
        t13423 = dx * (t278 / 0.2E1 - t6776 / 0.2E1) / 0.24E2
        t13428 = t9256 * t161 / 0.6E1 + (t9262 + t9215 + t9219 - t9264 +
     # t9223 - t9225 + t9254 - t9256 * t9212) * t161 / 0.2E1 + t9669 * t
     #161 / 0.6E1 + (t9731 + t9657 + t9661 - t9735 + t9665 - t9667 - t96
     #69 * t9212) * t161 / 0.2E1 + t10096 * t161 / 0.6E1 + (t10158 + t10
     #086 + t10089 - t10162 + t10092 - t10094 - t10096 * t9212) * t161 /
     # 0.2E1 - t12965 * t161 / 0.6E1 - (t12971 + t12940 + t12943 - t1297
     #3 + t12946 - t12948 + t12963 - t12965 * t9212) * t161 / 0.2E1 - t1
     #3171 * t161 / 0.6E1 - (t13202 + t13161 + t13164 - t13206 + t13167 
     #- t13169 - t13171 * t9212) * t161 / 0.2E1 - t13388 * t161 / 0.6E1 
     #- (t13419 + t13378 + t13381 - t13423 + t13384 - t13386 - t13388 * 
     #t9212) * t161 / 0.2E1
        t13431 = t632 * t637
        t13436 = t673 * t678
        t13444 = t4 * (t13431 / 0.2E1 + t9272 - dy * ((t3959 * t3964 - t
     #13431) * t183 / 0.2E1 - (t9271 - t13436) * t183 / 0.2E1) / 0.8E1)
        t13450 = (t975 - t1114) * t94
        t13452 = ((t973 - t975) * t94 - t13450) * t94
        t13456 = (t13450 - (t1114 - t6913) * t94) * t94
        t13459 = t1701 * (t13452 / 0.2E1 + t13456 / 0.2E1)
        t13461 = t139 / 0.4E1
        t13462 = t147 / 0.4E1
        t13463 = t6857 / 0.12E2
        t13469 = (t2129 - t7254) * t94
        t13480 = t975 / 0.2E1
        t13481 = t1114 / 0.2E1
        t13482 = t13459 / 0.6E1
        t13485 = t990 / 0.2E1
        t13486 = t1127 / 0.2E1
        t13490 = (t990 - t1127) * t94
        t13492 = ((t988 - t990) * t94 - t13490) * t94
        t13496 = (t13490 - (t1127 - t6932) * t94) * t94
        t13499 = t1701 * (t13492 / 0.2E1 + t13496 / 0.2E1)
        t13500 = t13499 / 0.6E1
        t13507 = t975 / 0.4E1 + t1114 / 0.4E1 - t13459 / 0.12E2 + t13461
     # + t13462 - t13463 - dy * ((t2129 / 0.2E1 + t7254 / 0.2E1 - t1701 
     #* (((t2126 - t2129) * t94 - t13469) * t94 / 0.2E1 + (t13469 - (t72
     #54 - t10680) * t94) * t94 / 0.2E1) / 0.6E1 - t13480 - t13481 + t13
     #482) * t183 / 0.2E1 - (t2062 + t6847 - t6858 - t13485 - t13486 + t
     #13500) * t183 / 0.2E1) / 0.8E1
        t13512 = t4 * (t13431 / 0.2E1 + t9271 / 0.2E1)
        t13514 = t5487 / 0.4E1 + t8398 / 0.4E1 + t2910 / 0.4E1 + t7444 /
     # 0.4E1
        t13523 = (t9437 - t9582) * t94 / 0.4E1 + (t9582 - t13086) * t94 
     #/ 0.4E1 + t6406 / 0.4E1 + t9181 / 0.4E1
        t13529 = dy * (t7260 / 0.2E1 - t1133 / 0.2E1)
        t13533 = t13444 * t9213 * t13507
        t13536 = t13512 * t9658 * t13514 / 0.2E1
        t13539 = t13512 * t9662 * t13523 / 0.6E1
        t13541 = t9213 * t13529 / 0.24E2
        t13543 = (t13444 * t136 * t13507 + t13512 * t9349 * t13514 / 0.2
     #E1 + t13512 * t9355 * t13523 / 0.6E1 - t136 * t13529 / 0.24E2 - t1
     #3533 - t13536 - t13539 + t13541) * t133
        t13551 = (t311 - t639) * t94
        t13553 = ((t309 - t311) * t94 - t13551) * t94
        t13557 = (t13551 - (t639 - t6735) * t94) * t94
        t13560 = t1701 * (t13553 / 0.2E1 + t13557 / 0.2E1)
        t13562 = t171 / 0.4E1
        t13563 = t571 / 0.4E1
        t13566 = t1701 * (t1715 / 0.2E1 + t6703 / 0.2E1)
        t13567 = t13566 / 0.12E2
        t13573 = (t1860 - t3966) * t94
        t13584 = t311 / 0.2E1
        t13585 = t639 / 0.2E1
        t13586 = t13560 / 0.6E1
        t13589 = t171 / 0.2E1
        t13590 = t571 / 0.2E1
        t13591 = t13566 / 0.6E1
        t13592 = t354 / 0.2E1
        t13593 = t680 / 0.2E1
        t13597 = (t354 - t680) * t94
        t13599 = ((t352 - t354) * t94 - t13597) * t94
        t13603 = (t13597 - (t680 - t6749) * t94) * t94
        t13606 = t1701 * (t13599 / 0.2E1 + t13603 / 0.2E1)
        t13607 = t13606 / 0.6E1
        t13615 = t13444 * (t311 / 0.4E1 + t639 / 0.4E1 - t13560 / 0.12E2
     # + t13562 + t13563 - t13567 - dy * ((t1860 / 0.2E1 + t3966 / 0.2E1
     # - t1701 * (((t1858 - t1860) * t94 - t13573) * t94 / 0.2E1 + (t135
     #73 - (t3966 - t7554) * t94) * t94 / 0.2E1) / 0.6E1 - t13584 - t135
     #85 + t13586) * t183 / 0.2E1 - (t13589 + t13590 - t13591 - t13592 -
     # t13593 + t13607) * t183 / 0.2E1) / 0.8E1)
        t13619 = dy * (t3972 / 0.2E1 - t686 / 0.2E1) / 0.24E2
        t13626 = t925 - dy * t7094 / 0.24E2
        t13631 = t161 * t4133 * t183
        t13636 = t895 * t9583 * t183
        t13639 = dy * t7107
        t13642 = cc * t6440
        t13643 = j + 3
        t13644 = u(i,t13643,k,n)
        t13646 = (t13644 - t1407) * t183
        t13654 = rx(i,t13643,k,0,0)
        t13655 = rx(i,t13643,k,1,1)
        t13657 = rx(i,t13643,k,2,2)
        t13659 = rx(i,t13643,k,1,2)
        t13661 = rx(i,t13643,k,2,1)
        t13663 = rx(i,t13643,k,1,0)
        t13665 = rx(i,t13643,k,0,2)
        t13667 = rx(i,t13643,k,0,1)
        t13670 = rx(i,t13643,k,2,0)
        t13676 = 0.1E1 / (t13654 * t13655 * t13657 - t13654 * t13659 * t
     #13661 + t13663 * t13661 * t13665 - t13663 * t13667 * t13657 + t136
     #70 * t13667 * t13659 - t13670 * t13655 * t13665)
        t13677 = t13663 ** 2
        t13678 = t13655 ** 2
        t13679 = t13659 ** 2
        t13681 = t13676 * (t13677 + t13678 + t13679)
        t13684 = t4 * (t13681 / 0.2E1 + t3978 / 0.2E1)
        t13687 = (t13684 * t13646 - t3982) * t183
        t13698 = (t4091 - t4104) * t236
        t13710 = t4115 / 0.2E1
        t13720 = t4 * (t4110 / 0.2E1 + t13710 - dz * ((t8558 - t4110) * 
     #t236 / 0.2E1 - (t4115 - t4124) * t236 / 0.2E1) / 0.8E1)
        t13732 = t4 * (t13710 + t4124 / 0.2E1 - dz * ((t4110 - t4115) * 
     #t236 / 0.2E1 - (t4124 - t8706) * t236 / 0.2E1) / 0.8E1)
        t13737 = t3552 / 0.2E1
        t13747 = t4 * (t2969 / 0.2E1 + t13737 - dx * ((t2960 - t2969) * 
     #t94 / 0.2E1 - (t3552 - t3898) * t94 / 0.2E1) / 0.8E1)
        t13759 = t4 * (t13737 + t3898 / 0.2E1 - dx * ((t2969 - t3552) * 
     #t94 / 0.2E1 - (t3898 - t7486) * t94 / 0.2E1) / 0.8E1)
        t13766 = (t4039 - t4076) * t236
        t13796 = (t719 - t722) * t236
        t13798 = ((t5192 - t719) * t236 - t13796) * t236
        t13803 = (t13796 - (t722 - t5390) * t236) * t236
        t13819 = (t3564 - t3918) * t94
        t13830 = t4001 + t4077 + t3936 + t3919 + t3973 + t4092 + t4040 -
     # t1327 * ((t3981 * ((t13646 - t1409) * t183 - t6677) * t183 - t668
     #2) * t183 + ((t13687 - t3984) * t183 - t6691) * t183) / 0.24E2 - t
     #1228 * (((t8552 - t4091) * t236 - t13698) * t236 / 0.2E1 + (t13698
     # - (t4104 - t8700) * t236) * t236 / 0.2E1) / 0.6E1 + (t13720 * t71
     #9 - t13732 * t722) * t236 + (t13747 * t311 - t13759 * t639) * t94 
     #- t1228 * (((t8539 - t4039) * t236 - t13766) * t236 / 0.2E1 + (t13
     #766 - (t4076 - t8687) * t236) * t236 / 0.2E1) / 0.6E1 - t1701 * ((
     #t3688 * ((t1858 / 0.2E1 - t3966 / 0.2E1) * t94 - (t1860 / 0.2E1 - 
     #t7554 / 0.2E1) * t94) * t94 - t6742) * t183 / 0.2E1 + t6747 / 0.2E
     #1) / 0.6E1 - t1228 * ((t4118 * t13798 - t4127 * t13803) * t236 + (
     #(t8564 - t4130) * t236 - (t4130 - t8712) * t236) * t236) / 0.24E2 
     #- t1701 * (((t3000 - t3564) * t94 - t13819) * t94 / 0.2E1 + (t1381
     #9 - (t3918 - t7506) * t94) * t94 / 0.2E1) / 0.6E1
        t13834 = (t3573 - t3935) * t94
        t13845 = t4 * t13676
        t13850 = u(i,t13643,t233,n)
        t13852 = (t13850 - t13644) * t236
        t13853 = u(i,t13643,t238,n)
        t13855 = (t13644 - t13853) * t236
        t13188 = t13845 * (t13663 * t13670 + t13655 * t13661 + t13659 * 
     #t13657)
        t13861 = (t13188 * (t13852 / 0.2E1 + t13855 / 0.2E1) - t3998) * 
     #t183
        t13882 = t3377 * t6351
        t13905 = u(t5,t13643,k,n)
        t13907 = (t13905 - t13644) * t94
        t13908 = u(t96,t13643,k,n)
        t13910 = (t13644 - t13908) * t94
        t13212 = t13845 * (t13654 * t13663 + t13667 * t13655 + t13665 * 
     #t13659)
        t13916 = (t13212 * (t13907 / 0.2E1 + t13910 / 0.2E1) - t3970) * 
     #t183
        t13933 = t4 * (t3978 / 0.2E1 + t6430 - dy * ((t13681 - t3978) * 
     #t183 / 0.2E1 - t6445 / 0.2E1) / 0.8E1)
        t13938 = (t13850 - t3989) * t183
        t13230 = ((t13646 / 0.2E1 - t218 / 0.2E1) * t183 - t1412) * t183
        t13952 = t708 * t13230
        t13956 = (t13853 - t3992) * t183
        t13975 = t3377 * t6245
        t14004 = (t13905 - t1346) * t183
        t14014 = t629 * t13230
        t14018 = (t13908 - t3910) * t183
        t13258 = ((t3130 / 0.2E1 - t4031 / 0.2E1) * t94 - (t3606 / 0.2E1
     # - t7619 / 0.2E1) * t94) * t94
        t13262 = ((t3171 / 0.2E1 - t4070 / 0.2E1) * t94 - (t3645 / 0.2E1
     # - t7658 / 0.2E1) * t94) * t94
        t14045 = -t1701 * (((t3032 - t3573) * t94 - t13834) * t94 / 0.2E
     #1 + (t13834 - (t3935 - t7523) * t94) * t94 / 0.2E1) / 0.6E1 + t735
     # + t650 - t1327 * (((t13861 - t4000) * t183 - t6499) * t183 / 0.2E
     #1 + t6503 / 0.2E1) / 0.6E1 - t1701 * ((t3759 * t13258 - t13882) * 
     #t236 / 0.2E1 + (t13882 - t3793 * t13262) * t236 / 0.2E1) / 0.6E1 -
     # t1327 * (((t13916 - t3972) * t183 - t6417) * t183 / 0.2E1 + t6421
     # / 0.2E1) / 0.6E1 + (t13933 * t1409 - t6442) * t183 + t4105 + t357
     #4 + t3565 - t1327 * ((t3806 * ((t13938 / 0.2E1 - t834 / 0.2E1) * t
     #183 - t6563) * t183 - t13952) * t236 / 0.2E1 + (t13952 - t3817 * (
     #(t13956 / 0.2E1 - t851 / 0.2E1) * t183 - t6578) * t183) * t236 / 0
     #.2E1) / 0.6E1 - t1228 * ((t2765 * t1575 - t13975) * t94 / 0.2E1 + 
     #(t13975 - t3651 * t9882) * t94 / 0.2E1) / 0.6E1 - t1228 * ((t3719 
     #* ((t8496 / 0.2E1 - t3994 / 0.2E1) * t236 - (t3991 / 0.2E1 - t8644
     # / 0.2E1) * t236) * t236 - t6601) * t183 / 0.2E1 + t6606 / 0.2E1) 
     #/ 0.6E1 - t1327 * ((t306 * ((t14004 / 0.2E1 - t200 / 0.2E1) * t183
     # - t1351) * t183 - t14014) * t94 / 0.2E1 + (t14014 - t3636 * ((t14
     #018 / 0.2E1 - t582 / 0.2E1) * t183 - t6828) * t183) * t94 / 0.2E1)
     # / 0.6E1 - t1701 * ((t3555 * t13553 - t3901 * t13557) * t94 + ((t3
     #558 - t3904) * t94 - (t3904 - t7492) * t94) * t94) / 0.24E2
        t14048 = dt * (t13830 + t14045) * t631
        t14051 = ut(i,t13643,k,n)
        t14053 = (t14051 - t2127) * t183
        t14057 = ((t14053 - t2610) * t183 - t7091) * t183
        t14064 = dy * (t2610 / 0.2E1 + t9318 - t1327 * (t14057 / 0.2E1 +
     # t7095 / 0.2E1) / 0.6E1) / 0.2E1
        t14065 = ut(i,t1328,t1229,n)
        t14067 = (t14065 - t6989) * t236
        t14071 = ut(i,t1328,t1277,n)
        t14073 = (t7010 - t14071) * t236
        t14099 = t3377 * t6485
        t14123 = (t2333 - t6946) * t94 / 0.2E1 + (t6946 - t10612) * t94 
     #/ 0.2E1
        t14127 = (t7938 * t14123 - t9546) * t236
        t14131 = (t9550 - t9559) * t236
        t14139 = (t2339 - t6952) * t94 / 0.2E1 + (t6952 - t10618) * t94 
     #/ 0.2E1
        t14143 = (t9557 - t8081 * t14139) * t236
        t14152 = ut(t5,t13643,k,n)
        t14154 = (t14152 - t2081) * t183
        t13435 = ((t14053 / 0.2E1 - t925 / 0.2E1) * t183 - t2613) * t183
        t14168 = t629 * t13435
        t14171 = ut(t96,t13643,k,n)
        t14173 = (t14171 - t7032) * t183
        t14196 = (t13212 * ((t14152 - t14051) * t94 / 0.2E1 + (t14051 - 
     #t14171) * t94 / 0.2E1) - t7258) * t183
        t14213 = (t14065 - t6946) * t183
        t14219 = (t7949 * (t14213 / 0.2E1 + t7120 / 0.2E1) - t9564) * t2
     #36
        t14223 = (t9568 - t9575) * t236
        t14227 = (t14071 - t6952) * t183
        t14233 = (t9573 - t8101 * (t14227 / 0.2E1 + t7136 / 0.2E1)) * t2
     #36
        t14249 = (t9376 - t9532) * t94
        t14279 = (t9388 - t9537) * t94
        t13488 = ((t9393 / 0.2E1 - t9542 / 0.2E1) * t94 - (t9395 / 0.2E1
     # - t13046 / 0.2E1) * t94) * t94
        t13494 = ((t9406 / 0.2E1 - t9553 / 0.2E1) * t94 - (t9408 / 0.2E1
     # - t13057 / 0.2E1) * t94) * t94
        t14303 = -t1228 * ((t3719 * ((t14067 / 0.2E1 - t7193 / 0.2E1) * 
     #t236 - (t7191 / 0.2E1 - t14073 / 0.2E1) * t236) * t236 - t6961) * 
     #t183 / 0.2E1 + t6966 / 0.2E1) / 0.6E1 - t1701 * ((t3759 * t13488 -
     # t14099) * t236 / 0.2E1 + (t14099 - t3793 * t13494) * t236 / 0.2E1
     #) / 0.6E1 - t1228 * (((t14127 - t9550) * t236 - t14131) * t236 / 0
     #.2E1 + (t14131 - (t9559 - t14143) * t236) * t236 / 0.2E1) / 0.6E1 
     #- t1327 * ((t306 * ((t14154 / 0.2E1 - t912 / 0.2E1) * t183 - t2264
     #) * t183 - t14168) * t94 / 0.2E1 + (t14168 - t3636 * ((t14173 / 0.
     #2E1 - t1089 / 0.2E1) * t183 - t7037) * t183) * t94 / 0.2E1) / 0.6E
     #1 - t1327 * (((t14196 - t7260) * t183 - t7262) * t183 / 0.2E1 + t7
     #266 / 0.2E1) / 0.6E1 + (t13933 * t2610 - t7283) * t183 + (t13747 *
     # t975 - t13759 * t1114) * t94 - t1228 * (((t14219 - t9568) * t236 
     #- t14223) * t236 / 0.2E1 + (t14223 - (t9575 - t14233) * t236) * t2
     #36 / 0.2E1) / 0.6E1 + (t13720 * t1141 - t13732 * t1144) * t236 - t
     #1701 * (((t9369 - t9376) * t94 - t14249) * t94 / 0.2E1 + (t14249 -
     # (t9532 - t13036) * t94) * t94 / 0.2E1) / 0.6E1 - t1701 * ((t3688 
     #* ((t2126 / 0.2E1 - t7254 / 0.2E1) * t94 - (t2129 / 0.2E1 - t10680
     # / 0.2E1) * t94) * t94 - t6920) * t183 / 0.2E1 + t6929 / 0.2E1) / 
     #0.6E1 + t9389 - t1701 * (((t9383 - t9388) * t94 - t14279) * t94 / 
     #0.2E1 + (t14279 - (t9537 - t13041) * t94) * t94 / 0.2E1) / 0.6E1 +
     # t1153 - t1701 * ((t3555 * t13452 - t3901 * t13456) * t94 + ((t935
     #9 - t9526) * t94 - (t9526 - t13030) * t94) * t94) / 0.24E2
        t14307 = (t1141 - t1144) * t236
        t14309 = ((t6948 - t1141) * t236 - t14307) * t236
        t14314 = (t14307 - (t1144 - t6954) * t236) * t236
        t14320 = (t8561 * t6948 - t9577) * t236
        t14325 = (t9578 - t8709 * t6954) * t236
        t14333 = ut(i,t13643,t233,n)
        t14335 = (t14333 - t6989) * t183
        t14345 = t708 * t13435
        t14348 = ut(i,t13643,t238,n)
        t14350 = (t14348 - t7010) * t183
        t14373 = (t13188 * ((t14333 - t14051) * t236 / 0.2E1 + (t14051 -
     # t14348) * t236 / 0.2E1) - t7197) * t183
        t14387 = t3377 * t6510
        t14404 = (t13684 * t14053 - t7104) * t183
        t14412 = t9560 + t9569 + t9576 + t9538 + t1125 + t9377 - t1228 *
     # ((t4118 * t14309 - t4127 * t14314) * t236 + ((t14320 - t9580) * t
     #236 - (t9580 - t14325) * t236) * t236) / 0.24E2 + t9540 + t9551 - 
     #t1327 * ((t3806 * ((t14335 / 0.2E1 - t1188 / 0.2E1) * t183 - t6994
     #) * t183 - t14345) * t236 / 0.2E1 + (t14345 - t3817 * ((t14350 / 0
     #.2E1 - t1201 / 0.2E1) * t183 - t7015) * t183) * t236 / 0.2E1) / 0.
     #6E1 + t9539 + t9533 - t1327 * (((t14373 - t7199) * t183 - t7201) *
     # t183 / 0.2E1 + t7205 / 0.2E1) / 0.6E1 - t1228 * ((t2765 * t2283 -
     # t14387) * t94 / 0.2E1 + (t14387 - t3651 * t10236) * t94 / 0.2E1) 
     #/ 0.6E1 - t1327 * ((t3981 * t14057 - t7096) * t183 + ((t14404 - t7
     #106) * t183 - t7108) * t183) / 0.24E2
        t14415 = t161 * (t14303 + t14412) * t631
        t14418 = dt * dy
        t14419 = t1429 ** 2
        t14420 = t1442 ** 2
        t14421 = t1440 ** 2
        t14423 = t1451 * (t14419 + t14420 + t14421)
        t14424 = t3937 ** 2
        t14425 = t3950 ** 2
        t14426 = t3948 ** 2
        t14428 = t3959 * (t14424 + t14425 + t14426)
        t14431 = t4 * (t14423 / 0.2E1 + t14428 / 0.2E1)
        t14432 = t14431 * t1860
        t14433 = t7525 ** 2
        t14434 = t7538 ** 2
        t14435 = t7536 ** 2
        t14437 = t7547 * (t14433 + t14434 + t14435)
        t14440 = t4 * (t14428 / 0.2E1 + t14437 / 0.2E1)
        t14441 = t14440 * t3966
        t14445 = t14004 / 0.2E1 + t1348 / 0.2E1
        t14447 = t1718 * t14445
        t14449 = t13646 / 0.2E1 + t1409 / 0.2E1
        t14451 = t3688 * t14449
        t14454 = (t14447 - t14451) * t94 / 0.2E1
        t14456 = t14018 / 0.2E1 + t3912 / 0.2E1
        t14458 = t7031 * t14456
        t14461 = (t14451 - t14458) * t94 / 0.2E1
        t13789 = t1452 * (t1429 * t1445 + t1442 * t1436 + t1440 * t1432)
        t14467 = t13789 * t1462
        t13793 = t3960 * (t3937 * t3953 + t3950 * t3944 + t3948 * t3940)
        t14473 = t13793 * t3996
        t14476 = (t14467 - t14473) * t94 / 0.2E1
        t13801 = t7548 * (t7525 * t7541 + t7532 * t7538 + t7536 * t7528)
        t14482 = t13801 * t7584
        t14485 = (t14473 - t14482) * t94 / 0.2E1
        t13808 = t8465 * (t8442 * t8458 + t8455 * t8449 + t8453 * t8445)
        t14493 = t13808 * t8473
        t14495 = t13793 * t3968
        t14498 = (t14493 - t14495) * t236 / 0.2E1
        t13814 = t8613 * (t8590 * t8606 + t8603 * t8597 + t8601 * t8593)
        t14504 = t13814 * t8621
        t14507 = (t14495 - t14504) * t236 / 0.2E1
        t14509 = t13938 / 0.2E1 + t4083 / 0.2E1
        t14511 = t7907 * t14509
        t14513 = t3719 * t14449
        t14516 = (t14511 - t14513) * t236 / 0.2E1
        t14518 = t13956 / 0.2E1 + t4098 / 0.2E1
        t14520 = t8045 * t14518
        t14523 = (t14513 - t14520) * t236 / 0.2E1
        t14524 = t8458 ** 2
        t14525 = t8449 ** 2
        t14526 = t8445 ** 2
        t14528 = t8464 * (t14524 + t14525 + t14526)
        t14529 = t3953 ** 2
        t14530 = t3944 ** 2
        t14531 = t3940 ** 2
        t14533 = t3959 * (t14529 + t14530 + t14531)
        t14536 = t4 * (t14528 / 0.2E1 + t14533 / 0.2E1)
        t14537 = t14536 * t3991
        t14538 = t8606 ** 2
        t14539 = t8597 ** 2
        t14540 = t8593 ** 2
        t14542 = t8612 * (t14538 + t14539 + t14540)
        t14545 = t4 * (t14533 / 0.2E1 + t14542 / 0.2E1)
        t14546 = t14545 * t3994
        t14549 = (t14432 - t14441) * t94 + t14454 + t14461 + t14476 + t1
     #4485 + t13916 / 0.2E1 + t3973 + t13687 + t13861 / 0.2E1 + t4001 + 
     #t14498 + t14507 + t14516 + t14523 + (t14537 - t14546) * t236
        t14550 = t14549 * t3958
        t14552 = (t14550 - t4132) * t183
        t14554 = t14552 / 0.2E1 + t4134 / 0.2E1
        t14555 = t14418 * t14554
        t14563 = t1327 * (t7091 - dy * (t14057 - t7095) / 0.12E2) / 0.12
     #E2
        t14568 = t3034 ** 2
        t14569 = t3047 ** 2
        t14570 = t3045 ** 2
        t14579 = u(t64,t13643,k,n)
        t14598 = rx(t5,t13643,k,0,0)
        t14599 = rx(t5,t13643,k,1,1)
        t14601 = rx(t5,t13643,k,2,2)
        t14603 = rx(t5,t13643,k,1,2)
        t14605 = rx(t5,t13643,k,2,1)
        t14607 = rx(t5,t13643,k,1,0)
        t14609 = rx(t5,t13643,k,0,2)
        t14611 = rx(t5,t13643,k,0,1)
        t14614 = rx(t5,t13643,k,2,0)
        t14620 = 0.1E1 / (t14598 * t14599 * t14601 - t14598 * t14603 * t
     #14605 + t14607 * t14605 * t14609 - t14607 * t14611 * t14601 + t146
     #14 * t14611 * t14603 - t14614 * t14599 * t14609)
        t14621 = t4 * t14620
        t14635 = t14607 ** 2
        t14636 = t14599 ** 2
        t14637 = t14603 ** 2
        t14650 = u(t5,t13643,t233,n)
        t14653 = u(t5,t13643,t238,n)
        t14670 = t13789 * t1862
        t14686 = (t14650 - t1329) * t183 / 0.2E1 + t1331 / 0.2E1
        t14690 = t1315 * t14445
        t14697 = (t14653 - t1364) * t183 / 0.2E1 + t1366 / 0.2E1
        t14703 = t5577 ** 2
        t14704 = t5568 ** 2
        t14705 = t5564 ** 2
        t14708 = t1445 ** 2
        t14709 = t1436 ** 2
        t14710 = t1432 ** 2
        t14712 = t1451 * (t14708 + t14709 + t14710)
        t14717 = t5757 ** 2
        t14718 = t5748 ** 2
        t14719 = t5744 ** 2
        t13953 = t5584 * (t5561 * t5577 + t5574 * t5568 + t5572 * t5564)
        t13959 = t5764 * (t5741 * t5757 + t5754 * t5748 + t5752 * t5744)
        t14728 = (t4 * (t3056 * (t14568 + t14569 + t14570) / 0.2E1 + t14
     #423 / 0.2E1) * t1858 - t14432) * t94 + (t2979 * ((t14579 - t1386) 
     #* t183 / 0.2E1 + t1388 / 0.2E1) - t14447) * t94 / 0.2E1 + t14454 +
     # (t3057 * (t3034 * t3050 + t3047 * t3041 + t3045 * t3037) * t3093 
     #- t14467) * t94 / 0.2E1 + t14476 + (t14621 * (t14598 * t14607 + t1
     #4611 * t14599 + t14609 * t14603) * ((t14579 - t13905) * t94 / 0.2E
     #1 + t13907 / 0.2E1) - t1864) * t183 / 0.2E1 + t3575 + (t4 * (t1462
     #0 * (t14635 + t14636 + t14637) / 0.2E1 + t1537 / 0.2E1) * t14004 -
     # t1541) * t183 + (t14621 * (t14607 * t14614 + t14599 * t14605 + t1
     #4603 * t14601) * ((t14650 - t13905) * t236 / 0.2E1 + (t13905 - t14
     #653) * t236 / 0.2E1) - t1464) * t183 / 0.2E1 + t3576 + (t13953 * t
     #5594 - t14670) * t236 / 0.2E1 + (t14670 - t13959 * t5774) * t236 /
     # 0.2E1 + (t5273 * t14686 - t14690) * t236 / 0.2E1 + (t14690 - t550
     #4 * t14697) * t236 / 0.2E1 + (t4 * (t5583 * (t14703 + t14704 + t14
     #705) / 0.2E1 + t14712 / 0.2E1) * t1458 - t4 * (t14712 / 0.2E1 + t5
     #763 * (t14717 + t14718 + t14719) / 0.2E1) * t1460) * t236
        t14729 = t14728 * t1450
        t14737 = t629 * t14554
        t14741 = t11255 ** 2
        t14742 = t11268 ** 2
        t14743 = t11266 ** 2
        t14752 = u(t6458,t13643,k,n)
        t14771 = rx(t96,t13643,k,0,0)
        t14772 = rx(t96,t13643,k,1,1)
        t14774 = rx(t96,t13643,k,2,2)
        t14776 = rx(t96,t13643,k,1,2)
        t14778 = rx(t96,t13643,k,2,1)
        t14780 = rx(t96,t13643,k,1,0)
        t14782 = rx(t96,t13643,k,0,2)
        t14784 = rx(t96,t13643,k,0,1)
        t14787 = rx(t96,t13643,k,2,0)
        t14793 = 0.1E1 / (t14771 * t14772 * t14774 - t14771 * t14776 * t
     #14778 + t14780 * t14778 * t14782 - t14780 * t14784 * t14774 + t147
     #87 * t14784 * t14776 - t14787 * t14772 * t14782)
        t14794 = t4 * t14793
        t14808 = t14780 ** 2
        t14809 = t14772 ** 2
        t14810 = t14776 ** 2
        t14823 = u(t96,t13643,t233,n)
        t14826 = u(t96,t13643,t238,n)
        t14843 = t13801 * t7556
        t14859 = (t14823 - t7577) * t183 / 0.2E1 + t7671 / 0.2E1
        t14863 = t7052 * t14456
        t14870 = (t14826 - t7580) * t183 / 0.2E1 + t7686 / 0.2E1
        t14876 = t12188 ** 2
        t14877 = t12179 ** 2
        t14878 = t12175 ** 2
        t14881 = t7541 ** 2
        t14882 = t7532 ** 2
        t14883 = t7528 ** 2
        t14885 = t7547 * (t14881 + t14882 + t14883)
        t14890 = t12336 ** 2
        t14891 = t12327 ** 2
        t14892 = t12323 ** 2
        t14087 = t12195 * (t12172 * t12188 + t12185 * t12179 + t12183 * 
     #t12175)
        t14092 = t12343 * (t12320 * t12336 + t12333 * t12327 + t12331 * 
     #t12323)
        t14901 = (t14441 - t4 * (t14437 / 0.2E1 + t11277 * (t14741 + t14
     #742 + t14743) / 0.2E1) * t7554) * t94 + t14461 + (t14458 - t10740 
     #* ((t14752 - t7498) * t183 / 0.2E1 + t7500 / 0.2E1)) * t94 / 0.2E1
     # + t14485 + (t14482 - t11278 * (t11255 * t11271 + t11268 * t11262 
     #+ t11266 * t11258) * t11314) * t94 / 0.2E1 + (t14794 * (t14771 * t
     #14780 + t14784 * t14772 + t14782 * t14776) * (t13910 / 0.2E1 + (t1
     #3908 - t14752) * t94 / 0.2E1) - t7558) * t183 / 0.2E1 + t7561 + (t
     #4 * (t14793 * (t14808 + t14809 + t14810) / 0.2E1 + t7566 / 0.2E1) 
     #* t14018 - t7570) * t183 + (t14794 * (t14780 * t14787 + t14772 * t
     #14778 + t14776 * t14774) * ((t14823 - t13908) * t236 / 0.2E1 + (t1
     #3908 - t14826) * t236 / 0.2E1) - t7586) * t183 / 0.2E1 + t7589 + (
     #t14087 * t12203 - t14843) * t236 / 0.2E1 + (t14843 - t14092 * t123
     #51) * t236 / 0.2E1 + (t11563 * t14859 - t14863) * t236 / 0.2E1 + (
     #t14863 - t11710 * t14870) * t236 / 0.2E1 + (t4 * (t12194 * (t14876
     # + t14877 + t14878) / 0.2E1 + t14885 / 0.2E1) * t7579 - t4 * (t148
     #85 / 0.2E1 + t12342 * (t14890 + t14891 + t14892) / 0.2E1) * t7582)
     # * t236
        t14902 = t14901 * t7546
        t14915 = t3377 * t8718
        t14938 = t5561 ** 2
        t14939 = t5574 ** 2
        t14940 = t5572 ** 2
        t14943 = t8442 ** 2
        t14944 = t8455 ** 2
        t14945 = t8453 ** 2
        t14947 = t8464 * (t14943 + t14944 + t14945)
        t14952 = t12172 ** 2
        t14953 = t12185 ** 2
        t14954 = t12183 ** 2
        t14966 = t7884 * t14509
        t14978 = t13808 * t8498
        t14987 = rx(i,t13643,t233,0,0)
        t14988 = rx(i,t13643,t233,1,1)
        t14990 = rx(i,t13643,t233,2,2)
        t14992 = rx(i,t13643,t233,1,2)
        t14994 = rx(i,t13643,t233,2,1)
        t14996 = rx(i,t13643,t233,1,0)
        t14998 = rx(i,t13643,t233,0,2)
        t15000 = rx(i,t13643,t233,0,1)
        t15003 = rx(i,t13643,t233,2,0)
        t15009 = 0.1E1 / (t14987 * t14988 * t14990 - t14987 * t14992 * t
     #14994 + t14996 * t14994 * t14998 - t14996 * t15000 * t14990 + t150
     #03 * t15000 * t14992 - t15003 * t14988 * t14998)
        t15010 = t4 * t15009
        t15026 = t14996 ** 2
        t15027 = t14988 ** 2
        t15028 = t14992 ** 2
        t15041 = u(i,t13643,t1229,n)
        t15051 = rx(i,t1328,t1229,0,0)
        t15052 = rx(i,t1328,t1229,1,1)
        t15054 = rx(i,t1328,t1229,2,2)
        t15056 = rx(i,t1328,t1229,1,2)
        t15058 = rx(i,t1328,t1229,2,1)
        t15060 = rx(i,t1328,t1229,1,0)
        t15062 = rx(i,t1328,t1229,0,2)
        t15064 = rx(i,t1328,t1229,0,1)
        t15067 = rx(i,t1328,t1229,2,0)
        t15073 = 0.1E1 / (t15051 * t15052 * t15054 - t15051 * t15056 * t
     #15058 + t15060 * t15058 * t15062 - t15060 * t15064 * t15054 + t150
     #67 * t15064 * t15056 - t15067 * t15052 * t15062)
        t15074 = t4 * t15073
        t15084 = (t5615 - t8494) * t94 / 0.2E1 + (t8494 - t12224) * t94 
     #/ 0.2E1
        t15103 = t15067 ** 2
        t15104 = t15058 ** 2
        t15105 = t15054 ** 2
        t14250 = t15074 * (t15060 * t15067 + t15052 * t15058 + t15056 * 
     #t15054)
        t15114 = (t4 * (t5583 * (t14938 + t14939 + t14940) / 0.2E1 + t14
     #947 / 0.2E1) * t5592 - t4 * (t14947 / 0.2E1 + t12194 * (t14952 + t
     #14953 + t14954) / 0.2E1) * t8471) * t94 + (t5244 * t14686 - t14966
     #) * t94 / 0.2E1 + (t14966 - t11547 * t14859) * t94 / 0.2E1 + (t139
     #53 * t5619 - t14978) * t94 / 0.2E1 + (t14978 - t14087 * t12228) * 
     #t94 / 0.2E1 + (t15010 * (t14987 * t14996 + t15000 * t14988 + t1499
     #8 * t14992) * ((t14650 - t13850) * t94 / 0.2E1 + (t13850 - t14823)
     # * t94 / 0.2E1) - t8475) * t183 / 0.2E1 + t8478 + (t4 * (t15009 * 
     #(t15026 + t15027 + t15028) / 0.2E1 + t8483 / 0.2E1) * t13938 - t84
     #87) * t183 + (t15010 * (t14996 * t15003 + t14988 * t14994 + t14992
     # * t14990) * ((t15041 - t13850) * t236 / 0.2E1 + t13852 / 0.2E1) -
     # t8500) * t183 / 0.2E1 + t8503 + (t15074 * (t15051 * t15067 + t150
     #64 * t15058 + t15062 * t15054) * t15084 - t14493) * t236 / 0.2E1 +
     # t14498 + (t14250 * ((t15041 - t8494) * t183 / 0.2E1 + t8546 / 0.2
     #E1) - t14511) * t236 / 0.2E1 + t14516 + (t4 * (t15073 * (t15103 + 
     #t15104 + t15105) / 0.2E1 + t14528 / 0.2E1) * t8496 - t14537) * t23
     #6
        t15115 = t15114 * t8463
        t15118 = t5741 ** 2
        t15119 = t5754 ** 2
        t15120 = t5752 ** 2
        t15123 = t8590 ** 2
        t15124 = t8603 ** 2
        t15125 = t8601 ** 2
        t15127 = t8612 * (t15123 + t15124 + t15125)
        t15132 = t12320 ** 2
        t15133 = t12333 ** 2
        t15134 = t12331 ** 2
        t15146 = t8023 * t14518
        t15158 = t13814 * t8646
        t15167 = rx(i,t13643,t238,0,0)
        t15168 = rx(i,t13643,t238,1,1)
        t15170 = rx(i,t13643,t238,2,2)
        t15172 = rx(i,t13643,t238,1,2)
        t15174 = rx(i,t13643,t238,2,1)
        t15176 = rx(i,t13643,t238,1,0)
        t15178 = rx(i,t13643,t238,0,2)
        t15180 = rx(i,t13643,t238,0,1)
        t15183 = rx(i,t13643,t238,2,0)
        t15189 = 0.1E1 / (t15167 * t15168 * t15170 - t15167 * t15172 * t
     #15174 + t15176 * t15174 * t15178 - t15176 * t15180 * t15170 + t151
     #83 * t15180 * t15172 - t15183 * t15168 * t15178)
        t15190 = t4 * t15189
        t15206 = t15176 ** 2
        t15207 = t15168 ** 2
        t15208 = t15172 ** 2
        t15221 = u(i,t13643,t1277,n)
        t15231 = rx(i,t1328,t1277,0,0)
        t15232 = rx(i,t1328,t1277,1,1)
        t15234 = rx(i,t1328,t1277,2,2)
        t15236 = rx(i,t1328,t1277,1,2)
        t15238 = rx(i,t1328,t1277,2,1)
        t15240 = rx(i,t1328,t1277,1,0)
        t15242 = rx(i,t1328,t1277,0,2)
        t15244 = rx(i,t1328,t1277,0,1)
        t15247 = rx(i,t1328,t1277,2,0)
        t15253 = 0.1E1 / (t15231 * t15232 * t15234 - t15231 * t15236 * t
     #15238 + t15240 * t15238 * t15242 - t15240 * t15244 * t15234 + t152
     #47 * t15244 * t15236 - t15247 * t15232 * t15242)
        t15254 = t4 * t15253
        t15264 = (t5795 - t8642) * t94 / 0.2E1 + (t8642 - t12372) * t94 
     #/ 0.2E1
        t15283 = t15247 ** 2
        t15284 = t15238 ** 2
        t15285 = t15234 ** 2
        t14391 = t15254 * (t15240 * t15247 + t15232 * t15238 + t15236 * 
     #t15234)
        t15294 = (t4 * (t5763 * (t15118 + t15119 + t15120) / 0.2E1 + t15
     #127 / 0.2E1) * t5772 - t4 * (t15127 / 0.2E1 + t12342 * (t15132 + t
     #15133 + t15134) / 0.2E1) * t8619) * t94 + (t5489 * t14697 - t15146
     #) * t94 / 0.2E1 + (t15146 - t11687 * t14870) * t94 / 0.2E1 + (t139
     #59 * t5799 - t15158) * t94 / 0.2E1 + (t15158 - t14092 * t12376) * 
     #t94 / 0.2E1 + (t15190 * (t15167 * t15176 + t15180 * t15168 + t1517
     #2 * t15178) * ((t14653 - t13853) * t94 / 0.2E1 + (t13853 - t14826)
     # * t94 / 0.2E1) - t8623) * t183 / 0.2E1 + t8626 + (t4 * (t15189 * 
     #(t15206 + t15207 + t15208) / 0.2E1 + t8631 / 0.2E1) * t13956 - t86
     #35) * t183 + (t15190 * (t15183 * t15176 + t15168 * t15174 + t15172
     # * t15170) * (t13855 / 0.2E1 + (t13853 - t15221) * t236 / 0.2E1) -
     # t8648) * t183 / 0.2E1 + t8651 + t14507 + (t14504 - t15254 * (t152
     #31 * t15247 + t15244 * t15238 + t15242 * t15234) * t15264) * t236 
     #/ 0.2E1 + t14523 + (t14520 - t14391 * ((t15221 - t8642) * t183 / 0
     #.2E1 + t8694 / 0.2E1)) * t236 / 0.2E1 + (t14546 - t4 * (t14542 / 0
     #.2E1 + t15253 * (t15283 + t15284 + t15285) / 0.2E1) * t8644) * t23
     #6
        t15295 = t15294 * t8611
        t15310 = (t5689 - t8566) * t94 / 0.2E1 + (t8566 - t12296) * t94 
     #/ 0.2E1
        t15314 = t3377 * t8400
        t15323 = (t5869 - t8714) * t94 / 0.2E1 + (t8714 - t12444) * t94 
     #/ 0.2E1
        t15336 = t708 * t14554
        t15353 = (t3555 * t5487 - t3901 * t8398) * t94 + (t306 * ((t1472
     #9 - t3703) * t183 / 0.2E1 + t3705 / 0.2E1) - t14737) * t94 / 0.2E1
     # + (t14737 - t3636 * ((t14902 - t7720) * t183 / 0.2E1 + t7722 / 0.
     #2E1)) * t94 / 0.2E1 + (t2765 * t5873 - t14915) * t94 / 0.2E1 + (t1
     #4915 - t3651 * t12448) * t94 / 0.2E1 + (t3688 * ((t14729 - t14550)
     # * t94 / 0.2E1 + (t14550 - t14902) * t94 / 0.2E1) - t8402) * t183 
     #/ 0.2E1 + t8407 + (t3981 * t14552 - t8417) * t183 + (t3719 * ((t15
     #115 - t14550) * t236 / 0.2E1 + (t14550 - t15295) * t236 / 0.2E1) -
     # t8720) * t183 / 0.2E1 + t8725 + (t3759 * t15310 - t15314) * t236 
     #/ 0.2E1 + (t15314 - t3793 * t15323) * t236 / 0.2E1 + (t3806 * ((t1
     #5115 - t8566) * t183 / 0.2E1 + t9050 / 0.2E1) - t15336) * t236 / 0
     #.2E1 + (t15336 - t3817 * ((t15295 - t8714) * t183 / 0.2E1 + t9063 
     #/ 0.2E1)) * t236 / 0.2E1 + (t4118 * t8568 - t4127 * t8716) * t236
        t15355 = t895 * t15353 * t631
        t15358 = t161 * dy
        t15368 = t14053 / 0.2E1 + t2610 / 0.2E1
        t15370 = t3688 * t15368
        t15384 = t13793 * t7195
        t15400 = (t2080 - t6989) * t94 / 0.2E1 + (t6989 - t10710) * t94 
     #/ 0.2E1
        t15404 = t13793 * t7256
        t15413 = (t2084 - t7010) * t94 / 0.2E1 + (t7010 - t10713) * t94 
     #/ 0.2E1
        t15424 = t3719 * t15368
        t15439 = (t14431 * t2129 - t14440 * t7254) * t94 + (t1718 * (t14
     #154 / 0.2E1 + t2261 / 0.2E1) - t15370) * t94 / 0.2E1 + (t15370 - t
     #7031 * (t14173 / 0.2E1 + t7034 / 0.2E1)) * t94 / 0.2E1 + (t13789 *
     # t2088 - t15384) * t94 / 0.2E1 + (t15384 - t13801 * t10717) * t94 
     #/ 0.2E1 + t14196 / 0.2E1 + t9539 + t14404 + t14373 / 0.2E1 + t9540
     # + (t13808 * t15400 - t15404) * t236 / 0.2E1 + (t15404 - t13814 * 
     #t15413) * t236 / 0.2E1 + (t7907 * (t14335 / 0.2E1 + t6991 / 0.2E1)
     # - t15424) * t236 / 0.2E1 + (t15424 - t8045 * (t14350 / 0.2E1 + t7
     #012 / 0.2E1)) * t236 / 0.2E1 + (t14536 * t7191 - t14545 * t7193) *
     # t236
        t15445 = t15358 * ((t15439 * t3958 - t9582) * t183 / 0.2E1 + t95
     #84 / 0.2E1)
        t15449 = t14418 * (t14552 - t4134)
        t15454 = dy * (t9318 + t9319 - t9320) / 0.2E1
        t15455 = t14418 * t4400
        t15457 = t1227 * t15455 / 0.2E1
        t15463 = t1327 * (t7093 - dy * (t7095 - t7100) / 0.12E2) / 0.12E
     #2
        t15466 = t15358 * (t9584 / 0.2E1 + t9645 / 0.2E1)
        t15468 = t2079 * t15466 / 0.4E1
        t15470 = t14418 * (t4134 - t4398)
        t15472 = t1227 * t15470 / 0.12E2
        t15473 = t923 + t1227 * t14048 - t14064 + t2079 * t14415 / 0.2E1
     # - t1227 * t14555 / 0.2E1 + t14563 + t2923 * t15355 / 0.6E1 - t207
     #9 * t15445 / 0.4E1 + t1227 * t15449 / 0.12E2 - t2 - t6846 - t15454
     # - t7310 - t15457 - t15463 - t9081 - t15468 - t15472
        t15477 = 0.8E1 * t693
        t15478 = 0.8E1 * t694
        t15479 = 0.8E1 * t695
        t15489 = sqrt(0.8E1 * t688 + 0.8E1 * t689 + 0.8E1 * t690 + t1547
     #7 + t15478 + t15479 - 0.2E1 * dy * ((t3974 + t3975 + t3976 - t688 
     #- t689 - t690) * t183 / 0.2E1 - (t693 + t694 + t695 - t702 - t703 
     #- t704) * t183 / 0.2E1))
        t15490 = 0.1E1 / t15489
        t15495 = t6441 * t9213 * t13626
        t15498 = t700 * t9216 * t13631 / 0.2E1
        t15501 = t700 * t9220 * t13636 / 0.6E1
        t15503 = t9213 * t13639 / 0.24E2
        t15516 = t9226 * t15455 / 0.2E1
        t15518 = t9228 * t15466 / 0.4E1
        t15520 = t9226 * t15470 / 0.12E2
        t15521 = t923 + t9226 * t14048 - t14064 + t9228 * t14415 / 0.2E1
     # - t9226 * t14555 / 0.2E1 + t14563 + t9233 * t15355 / 0.6E1 - t922
     #8 * t15445 / 0.4E1 + t9226 * t15449 / 0.12E2 - t2 - t9240 - t15454
     # - t9242 - t15516 - t15463 - t9246 - t15518 - t15520
        t15524 = 0.2E1 * t13642 * t15521 * t15490
        t15526 = (t6441 * t136 * t13626 + t700 * t159 * t13631 / 0.2E1 +
     # t700 * t893 * t13636 / 0.6E1 - t136 * t13639 / 0.24E2 + 0.2E1 * t
     #13642 * t15473 * t15490 - t15495 - t15498 - t15501 + t15503 - t155
     #24) * t133
        t15532 = t6441 * (t218 - dy * t6680 / 0.24E2)
        t15534 = dy * t6690 / 0.24E2
        t15539 = t632 * t716
        t15541 = t57 * t730
        t15542 = t15541 / 0.2E1
        t15546 = t673 * t739
        t15554 = t4 * (t15539 / 0.2E1 + t15542 - dy * ((t3959 * t3988 - 
     #t15539) * t183 / 0.2E1 - (t15541 - t15546) * t183 / 0.2E1) / 0.8E1
     #)
        t15559 = t1228 * (t14309 / 0.2E1 + t14314 / 0.2E1)
        t15566 = (t7191 - t7193) * t236
        t15577 = t1141 / 0.2E1
        t15578 = t1144 / 0.2E1
        t15579 = t15559 / 0.6E1
        t15582 = t1156 / 0.2E1
        t15583 = t1159 / 0.2E1
        t15587 = (t1156 - t1159) * t236
        t15589 = ((t6969 - t1156) * t236 - t15587) * t236
        t15593 = (t15587 - (t1159 - t6975) * t236) * t236
        t15596 = t1228 * (t15589 / 0.2E1 + t15593 / 0.2E1)
        t15597 = t15596 / 0.6E1
        t15604 = t1141 / 0.4E1 + t1144 / 0.4E1 - t15559 / 0.12E2 + t9762
     # + t9763 - t9767 - dy * ((t7191 / 0.2E1 + t7193 / 0.2E1 - t1228 * 
     #(((t14067 - t7191) * t236 - t15566) * t236 / 0.2E1 + (t15566 - (t7
     #193 - t14073) * t236) * t236 / 0.2E1) / 0.6E1 - t15577 - t15578 + 
     #t15579) * t183 / 0.2E1 - (t9789 + t9790 - t9791 - t15582 - t15583 
     #+ t15597) * t183 / 0.2E1) / 0.8E1
        t15609 = t4 * (t15539 / 0.2E1 + t15541 / 0.2E1)
        t15611 = t8568 / 0.4E1 + t8716 / 0.4E1 + t5278 / 0.4E1 + t5476 /
     # 0.4E1
        t15622 = t4823 * t9562
        t15634 = t3759 * t10001
        t15646 = (t7884 * t15400 - t9985) * t183
        t15650 = (t8486 * t6991 - t9996) * t183
        t15656 = (t7907 * (t14067 / 0.2E1 + t7191 / 0.2E1) - t10003) * t
     #183
        t15660 = (t5533 * t9395 - t8428 * t9542) * t94 + (t4684 * t9417 
     #- t15622) * t94 / 0.2E1 + (t15622 - t7508 * t13066) * t94 / 0.2E1 
     #+ (t3402 * t9876 - t15634) * t94 / 0.2E1 + (t15634 - t7087 * t1329
     #3) * t94 / 0.2E1 + t15646 / 0.2E1 + t9990 + t15650 + t15656 / 0.2E
     #1 + t10008 + t14127 / 0.2E1 + t9551 + t14219 / 0.2E1 + t9569 + t14
     #320
        t15661 = t15660 * t4023
        t15671 = t4962 * t9571
        t15683 = t3793 * t10054
        t15695 = (t8023 * t15413 - t10038) * t183
        t15699 = (t8634 * t7012 - t10049) * t183
        t15705 = (t8045 * (t7193 / 0.2E1 + t14073 / 0.2E1) - t10056) * t
     #183
        t15709 = (t5713 * t9408 - t8576 * t9553) * t94 + (t4734 * t9426 
     #- t15671) * t94 / 0.2E1 + (t15671 - t7685 * t13075) * t94 / 0.2E1 
     #+ (t3433 * t9948 - t15683) * t94 / 0.2E1 + (t15683 - t7131 * t1334
     #6) * t94 / 0.2E1 + t15695 / 0.2E1 + t10043 + t15699 + t15705 / 0.2
     #E1 + t10061 + t9560 + t14143 / 0.2E1 + t9576 + t14233 / 0.2E1 + t1
     #4325
        t15710 = t15709 * t4062
        t15714 = (t15661 - t9582) * t236 / 0.4E1 + (t9582 - t15710) * t2
     #36 / 0.4E1 + t10021 / 0.4E1 + t10074 / 0.4E1
        t15720 = dy * (t7199 / 0.2E1 - t1165 / 0.2E1)
        t15724 = t15554 * t9213 * t15604
        t15727 = t15609 * t9658 * t15611 / 0.2E1
        t15730 = t15609 * t9662 * t15714 / 0.6E1
        t15732 = t9213 * t15720 / 0.24E2
        t15734 = (t15554 * t136 * t15604 + t15609 * t9349 * t15611 / 0.2
     #E1 + t15609 * t9355 * t15714 / 0.6E1 - t136 * t15720 / 0.24E2 - t1
     #5724 - t15727 - t15730 + t15732) * t133
        t15741 = t1228 * (t13798 / 0.2E1 + t13803 / 0.2E1)
        t15748 = (t3991 - t3994) * t236
        t15759 = t719 / 0.2E1
        t15760 = t722 / 0.2E1
        t15761 = t15741 / 0.6E1
        t15764 = t742 / 0.2E1
        t15765 = t745 / 0.2E1
        t15769 = (t742 - t745) * t236
        t15771 = ((t5204 - t742) * t236 - t15769) * t236
        t15775 = (t15769 - (t745 - t5402) * t236) * t236
        t15778 = t1228 * (t15771 / 0.2E1 + t15775 / 0.2E1)
        t15779 = t15778 / 0.6E1
        t15787 = t15554 * (t719 / 0.4E1 + t722 / 0.4E1 - t15741 / 0.12E2
     # + t10105 + t10106 - t10110 - dy * ((t3991 / 0.2E1 + t3994 / 0.2E1
     # - t1228 * (((t8496 - t3991) * t236 - t15748) * t236 / 0.2E1 + (t1
     #5748 - (t3994 - t8644) * t236) * t236 / 0.2E1) / 0.6E1 - t15759 - 
     #t15760 + t15761) * t183 / 0.2E1 - (t10132 + t10133 - t10134 - t157
     #64 - t15765 + t15779) * t183 / 0.2E1) / 0.8E1)
        t15791 = dy * (t4000 / 0.2E1 - t751 / 0.2E1) / 0.24E2
        t15807 = t4 * (t9272 + t13436 / 0.2E1 - dy * ((t13431 - t9271) *
     # t183 / 0.2E1 - (t13436 - t4223 * t4228) * t183 / 0.2E1) / 0.8E1)
        t15818 = (t2147 - t7268) * t94
        t15835 = t13461 + t13462 - t13463 + t990 / 0.4E1 + t1127 / 0.4E1
     # - t13499 / 0.12E2 - dy * ((t13480 + t13481 - t13482 - t2062 - t68
     #47 + t6858) * t183 / 0.2E1 - (t13485 + t13486 - t13500 - t2147 / 0
     #.2E1 - t7268 / 0.2E1 + t1701 * (((t2144 - t2147) * t94 - t15818) *
     # t94 / 0.2E1 + (t15818 - (t7268 - t10695) * t94) * t94 / 0.2E1) / 
     #0.6E1) * t183 / 0.2E1) / 0.8E1
        t15840 = t4 * (t9271 / 0.2E1 + t13436 / 0.2E1)
        t15842 = t2910 / 0.4E1 + t7444 / 0.4E1 + t5500 / 0.4E1 + t8409 /
     # 0.4E1
        t15851 = t6406 / 0.4E1 + t9181 / 0.4E1 + (t9521 - t9643) * t94 /
     # 0.4E1 + (t9643 - t13147) * t94 / 0.4E1
        t15857 = dy * (t1124 / 0.2E1 - t7274 / 0.2E1)
        t15861 = t15807 * t9213 * t15835
        t15864 = t15840 * t9658 * t15842 / 0.2E1
        t15867 = t15840 * t9662 * t15851 / 0.6E1
        t15869 = t9213 * t15857 / 0.24E2
        t15871 = (t15807 * t136 * t15835 + t15840 * t9349 * t15842 / 0.2
     #E1 + t15840 * t9355 * t15851 / 0.6E1 - t136 * t15857 / 0.24E2 - t1
     #5861 - t15864 - t15867 + t15869) * t133
        t15884 = (t1880 - t4230) * t94
        t15902 = t15807 * (t13562 + t13563 - t13567 + t354 / 0.4E1 + t68
     #0 / 0.4E1 - t13606 / 0.12E2 - dy * ((t13584 + t13585 - t13586 - t1
     #3589 - t13590 + t13591) * t183 / 0.2E1 - (t13592 + t13593 - t13607
     # - t1880 / 0.2E1 - t4230 / 0.2E1 + t1701 * (((t1878 - t1880) * t94
     # - t15884) * t94 / 0.2E1 + (t15884 - (t4230 - t7818) * t94) * t94 
     #/ 0.2E1) / 0.6E1) * t183 / 0.2E1) / 0.8E1)
        t15906 = dy * (t649 / 0.2E1 - t4236 / 0.2E1) / 0.24E2
        t15913 = t928 - dy * t7099 / 0.24E2
        t15918 = t161 * t4397 * t183
        t15923 = t895 * t9644 * t183
        t15926 = dy * t7112
        t15929 = cc * t6452
        t15930 = j - 3
        t15931 = u(t5,t15930,k,n)
        t15933 = (t1352 - t15931) * t183
        t15941 = u(i,t15930,k,n)
        t15943 = (t1413 - t15941) * t183
        t15122 = (t1418 - (t221 / 0.2E1 - t15943 / 0.2E1) * t183) * t183
        t15950 = t669 * t15122
        t15953 = u(t96,t15930,k,n)
        t15955 = (t4174 - t15953) * t183
        t15976 = rx(i,t15930,k,0,0)
        t15977 = rx(i,t15930,k,1,1)
        t15979 = rx(i,t15930,k,2,2)
        t15981 = rx(i,t15930,k,1,2)
        t15983 = rx(i,t15930,k,2,1)
        t15985 = rx(i,t15930,k,1,0)
        t15987 = rx(i,t15930,k,0,2)
        t15989 = rx(i,t15930,k,0,1)
        t15992 = rx(i,t15930,k,2,0)
        t15998 = 0.1E1 / (t15976 * t15977 * t15979 - t15976 * t15981 * t
     #15983 + t15985 * t15983 * t15987 - t15985 * t15989 * t15979 + t159
     #92 * t15989 * t15981 - t15992 * t15977 * t15987)
        t15999 = t15985 ** 2
        t16000 = t15977 ** 2
        t16001 = t15981 ** 2
        t16003 = t15998 * (t15999 + t16000 + t16001)
        t16006 = t4 * (t4242 / 0.2E1 + t16003 / 0.2E1)
        t16009 = (t4246 - t16006 * t15943) * t183
        t16038 = t4 * (t6443 + t4242 / 0.2E1 - dy * (t6435 / 0.2E1 - (t4
     #242 - t16003) * t183 / 0.2E1) / 0.8E1)
        t16042 = t4 * t15998
        t16048 = (t15931 - t15941) * t94
        t16050 = (t15941 - t15953) * t94
        t15164 = t16042 * (t15976 * t15985 + t15989 * t15977 + t15987 * 
     #t15981)
        t16056 = (t4234 - t15164 * (t16048 / 0.2E1 + t16050 / 0.2E1)) * 
     #t183
        t16068 = (t3722 - t4182) * t94
        t16080 = t4379 / 0.2E1
        t16090 = t4 * (t4374 / 0.2E1 + t16080 - dz * ((t8863 - t4374) * 
     #t236 / 0.2E1 - (t4379 - t4388) * t236 / 0.2E1) / 0.8E1)
        t16102 = t4 * (t16080 + t4388 / 0.2E1 - dz * ((t4374 - t4379) * 
     #t236 / 0.2E1 - (t4388 - t9011) * t236 / 0.2E1) / 0.8E1)
        t16111 = t3487 * t6252
        t16126 = (t4355 - t4368) * t236
        t16140 = (t3731 - t4199) * t94
        t16187 = u(i,t15930,t233,n)
        t16189 = (t16187 - t15941) * t236
        t16190 = u(i,t15930,t238,n)
        t16192 = (t15941 - t16190) * t236
        t15211 = t16042 * (t15985 * t15992 + t15977 * t15983 + t15981 * 
     #t15979)
        t16198 = (t4262 - t15211 * (t16189 / 0.2E1 + t16192 / 0.2E1)) * 
     #t183
        t16219 = t3487 * t6357
        t15405 = ((t3438 / 0.2E1 - t4295 / 0.2E1) * t94 - (t3764 / 0.2E1
     # - t7883 / 0.2E1) * t94) * t94
        t15409 = ((t3479 / 0.2E1 - t4334 / 0.2E1) * t94 - (t3803 / 0.2E1
     # - t7922 / 0.2E1) * t94) * t94
        t16238 = -t1327 * ((t348 * (t1357 - (t203 / 0.2E1 - t15933 / 0.2
     #E1) * t183) * t183 - t15950) * t94 / 0.2E1 + (t15950 - t3883 * (t6
     #831 - (t585 / 0.2E1 - t15955 / 0.2E1) * t183) * t183) * t94 / 0.2E
     #1) / 0.6E1 - t1327 * ((t6687 - t4245 * (t6684 - (t1415 - t15943) *
     # t183) * t183) * t183 + (t6693 - (t4248 - t16009) * t183) * t183) 
     #/ 0.24E2 - t1228 * ((t4382 * t15771 - t4391 * t15775) * t236 + ((t
     #8869 - t4394) * t236 - (t4394 - t9017) * t236) * t236) / 0.24E2 + 
     #(t6454 - t16038 * t1415) * t183 - t1327 * (t6425 / 0.2E1 + (t6423 
     #- (t4236 - t16056) * t183) * t183 / 0.2E1) / 0.6E1 - t1701 * (((t3
     #308 - t3722) * t94 - t16068) * t94 / 0.2E1 + (t16068 - (t4182 - t7
     #770) * t94) * t94 / 0.2E1) / 0.6E1 + (t16090 * t742 - t16102 * t74
     #5) * t236 - t1228 * ((t3101 * t1580 - t16111) * t94 / 0.2E1 + (t16
     #111 - t3899 * t9887) * t94 / 0.2E1) / 0.6E1 - t1228 * (((t8857 - t
     #4355) * t236 - t16126) * t236 / 0.2E1 + (t16126 - (t4368 - t9005) 
     #* t236) * t236 / 0.2E1) / 0.6E1 - t1701 * (((t3340 - t3731) * t94 
     #- t16140) * t94 / 0.2E1 + (t16140 - (t4199 - t7787) * t94) * t94 /
     # 0.2E1) / 0.6E1 - t1701 * (t6758 / 0.2E1 + (t6756 - t3941 * ((t187
     #8 / 0.2E1 - t4230 / 0.2E1) * t94 - (t1880 / 0.2E1 - t7818 / 0.2E1)
     # * t94) * t94) * t183 / 0.2E1) / 0.6E1 - t1228 * (t6618 / 0.2E1 + 
     #(t6616 - t3962 * ((t8801 / 0.2E1 - t4258 / 0.2E1) * t236 - (t4255 
     #/ 0.2E1 - t8949 / 0.2E1) * t236) * t236) * t183 / 0.2E1) / 0.6E1 -
     # t1327 * (t6507 / 0.2E1 + (t6505 - (t4264 - t16198) * t183) * t183
     # / 0.2E1) / 0.6E1 - t1701 * ((t4006 * t15405 - t16219) * t236 / 0.
     #2E1 + (t16219 - t4043 * t15409) * t236 / 0.2E1) / 0.6E1 + t4304
        t16253 = (t4253 - t16187) * t183
        t16263 = t731 * t15122
        t16267 = (t4256 - t16190) * t183
        t16282 = t3710 / 0.2E1
        t16292 = t4 * (t3277 / 0.2E1 + t16282 - dx * ((t3268 - t3277) * 
     #t94 / 0.2E1 - (t3710 - t4162) * t94 / 0.2E1) / 0.8E1)
        t16304 = t4 * (t16282 + t4162 / 0.2E1 - dx * ((t3277 - t3710) * 
     #t94 / 0.2E1 - (t4162 - t7750) * t94 / 0.2E1) / 0.8E1)
        t16311 = (t4303 - t4340) * t236
        t16322 = -t1701 * ((t3713 * t13599 - t4165 * t13603) * t94 + ((t
     #3716 - t4168) * t94 - (t4168 - t7756) * t94) * t94) / 0.24E2 + t41
     #83 + t4200 + t4369 + t4237 + t4265 + t4341 + t752 + t687 + t3732 +
     # t3723 - t1327 * ((t4056 * (t6566 - (t836 / 0.2E1 - t16253 / 0.2E1
     #) * t183) * t183 - t16263) * t236 / 0.2E1 + (t16263 - t4067 * (t65
     #81 - (t853 / 0.2E1 - t16267 / 0.2E1) * t183) * t183) * t236 / 0.2E
     #1) / 0.6E1 + (t16292 * t354 - t16304 * t680) * t94 - t1228 * (((t8
     #844 - t4303) * t236 - t16311) * t236 / 0.2E1 + (t16311 - (t4340 - 
     #t8992) * t236) * t236 / 0.2E1) / 0.6E1 + t4356
        t16325 = dt * (t16238 + t16322) * t672
        t16328 = ut(i,t15930,k,n)
        t16330 = (t2145 - t16328) * t183
        t16334 = (t7098 - (t2615 - t16330) * t183) * t183
        t16341 = dy * (t9319 + t2615 / 0.2E1 - t1327 * (t7100 / 0.2E1 + 
     #t16334 / 0.2E1) / 0.6E1) / 0.2E1
        t16346 = ut(i,t1335,t1229,n)
        t16348 = (t16346 - t6995) * t236
        t16352 = ut(i,t1335,t1277,n)
        t16354 = (t7016 - t16352) * t236
        t16369 = (t6967 - t16346) * t183
        t16375 = (t8249 * (t7122 / 0.2E1 + t16369 / 0.2E1) - t9625) * t2
     #36
        t16379 = (t9629 - t9636) * t236
        t16383 = (t6973 - t16352) * t183
        t16389 = (t9634 - t8394 * (t7138 / 0.2E1 + t16383 / 0.2E1)) * t2
     #36
        t16411 = ut(t5,t15930,k,n)
        t16413 = (t2100 - t16411) * t183
        t15555 = (t2618 - (t928 / 0.2E1 - t16330 / 0.2E1) * t183) * t183
        t16427 = t669 * t15555
        t16430 = ut(t96,t15930,k,n)
        t16432 = (t7038 - t16430) * t183
        t16451 = (t7109 - t16006 * t16330) * t183
        t16459 = (t16292 * t990 - t16304 * t1127) * t94 - t1228 * (t6984
     # / 0.2E1 + (t6982 - t3962 * ((t16348 / 0.2E1 - t7209 / 0.2E1) * t2
     #36 - (t7207 / 0.2E1 - t16354 / 0.2E1) * t236) * t236) * t183 / 0.2
     #E1) / 0.6E1 - t1228 * (((t16375 - t9629) * t236 - t16379) * t236 /
     # 0.2E1 + (t16379 - (t9636 - t16389) * t236) * t236 / 0.2E1) / 0.6E
     #1 + t1134 + t9599 + t1166 + t9601 + t9612 + t9600 + t9461 + t9473 
     #+ t9594 - t1701 * ((t3713 * t13492 - t4165 * t13496) * t94 + ((t94
     #43 - t9587) * t94 - (t9587 - t13091) * t94) * t94) / 0.24E2 - t132
     #7 * ((t348 * (t2269 - (t915 / 0.2E1 - t16413 / 0.2E1) * t183) * t1
     #83 - t16427) * t94 / 0.2E1 + (t16427 - t3883 * (t7043 - (t1092 / 0
     #.2E1 - t16432 / 0.2E1) * t183) * t183) * t94 / 0.2E1) / 0.6E1 - t1
     #327 * ((t7101 - t4245 * t16334) * t183 + (t7113 - (t7111 - t16451)
     # * t183) * t183) / 0.24E2
        t16464 = ut(i,t15930,t233,n)
        t16467 = ut(i,t15930,t238,n)
        t16475 = (t7213 - t15211 * ((t16464 - t16328) * t236 / 0.2E1 + (
     #t16328 - t16467) * t236 / 0.2E1)) * t183
        t16493 = (t7272 - t15164 * ((t16411 - t16328) * t94 / 0.2E1 + (t
     #16328 - t16430) * t94 / 0.2E1)) * t183
        t16517 = t3487 * t6497
        t16539 = (t9460 - t9593) * t94
        t16556 = (t8866 * t6969 - t9638) * t236
        t16561 = (t9639 - t9014 * t6975) * t236
        t16574 = t3487 * t6526
        t16607 = (t2365 - t6967) * t94 / 0.2E1 + (t6967 - t10633) * t94 
     #/ 0.2E1
        t16611 = (t8235 * t16607 - t9607) * t236
        t16615 = (t9611 - t9620) * t236
        t16623 = (t2371 - t6973) * t94 / 0.2E1 + (t6973 - t10639) * t94 
     #/ 0.2E1
        t16627 = (t9618 - t8378 * t16623) * t236
        t16637 = (t6995 - t16464) * t183
        t16647 = t731 * t15555
        t16651 = (t7016 - t16467) * t183
        t16668 = (t9472 - t9598) * t94
        t15744 = ((t9477 / 0.2E1 - t9603 / 0.2E1) * t94 - (t9479 / 0.2E1
     # - t13107 / 0.2E1) * t94) * t94
        t15749 = ((t9490 / 0.2E1 - t9614 / 0.2E1) * t94 - (t9492 / 0.2E1
     # - t13118 / 0.2E1) * t94) * t94
        t16679 = (t16090 * t1156 - t16102 * t1159) * t236 - t1327 * (t72
     #19 / 0.2E1 + (t7217 - (t7215 - t16475) * t183) * t183 / 0.2E1) / 0
     #.6E1 - t1327 * (t7278 / 0.2E1 + (t7276 - (t7274 - t16493) * t183) 
     #* t183 / 0.2E1) / 0.6E1 + (t7284 - t16038 * t2615) * t183 - t1701 
     #* ((t4006 * t15744 - t16517) * t236 / 0.2E1 + (t16517 - t4043 * t1
     #5749) * t236 / 0.2E1) / 0.6E1 - t1701 * (((t9453 - t9460) * t94 - 
     #t16539) * t94 / 0.2E1 + (t16539 - (t9593 - t13097) * t94) * t94 / 
     #0.2E1) / 0.6E1 - t1228 * ((t4382 * t15589 - t4391 * t15593) * t236
     # + ((t16556 - t9641) * t236 - (t9641 - t16561) * t236) * t236) / 0
     #.24E2 - t1228 * ((t3101 * t2288 - t16574) * t94 / 0.2E1 + (t16574 
     #- t3899 * t10245) * t94 / 0.2E1) / 0.6E1 - t1701 * (t6941 / 0.2E1 
     #+ (t6939 - t3941 * ((t2144 / 0.2E1 - t7268 / 0.2E1) * t94 - (t2147
     # / 0.2E1 - t10695 / 0.2E1) * t94) * t94) * t183 / 0.2E1) / 0.6E1 -
     # t1228 * (((t16611 - t9611) * t236 - t16615) * t236 / 0.2E1 + (t16
     #615 - (t9620 - t16627) * t236) * t236 / 0.2E1) / 0.6E1 - t1327 * (
     #(t4056 * (t7000 - (t1190 / 0.2E1 - t16637 / 0.2E1) * t183) * t183 
     #- t16647) * t236 / 0.2E1 + (t16647 - t4067 * (t7021 - (t1203 / 0.2
     #E1 - t16651 / 0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 + t96
     #21 + t9630 + t9637 - t1701 * (((t9467 - t9472) * t94 - t16668) * t
     #94 / 0.2E1 + (t16668 - (t9598 - t13102) * t94) * t94 / 0.2E1) / 0.
     #6E1
        t16682 = t161 * (t16459 + t16679) * t672
        t16685 = t1473 ** 2
        t16686 = t1486 ** 2
        t16687 = t1484 ** 2
        t16689 = t1495 * (t16685 + t16686 + t16687)
        t16690 = t4201 ** 2
        t16691 = t4214 ** 2
        t16692 = t4212 ** 2
        t16694 = t4223 * (t16690 + t16691 + t16692)
        t16697 = t4 * (t16689 / 0.2E1 + t16694 / 0.2E1)
        t16698 = t16697 * t1880
        t16699 = t7789 ** 2
        t16700 = t7802 ** 2
        t16701 = t7800 ** 2
        t16703 = t7811 * (t16699 + t16700 + t16701)
        t16706 = t4 * (t16694 / 0.2E1 + t16703 / 0.2E1)
        t16707 = t16706 * t4230
        t16711 = t1354 / 0.2E1 + t15933 / 0.2E1
        t16713 = t1731 * t16711
        t16715 = t1415 / 0.2E1 + t15943 / 0.2E1
        t16717 = t3941 * t16715
        t16720 = (t16713 - t16717) * t94 / 0.2E1
        t16722 = t4176 / 0.2E1 + t15955 / 0.2E1
        t16724 = t7261 * t16722
        t16727 = (t16717 - t16724) * t94 / 0.2E1
        t15895 = t1496 * (t1473 * t1489 + t1486 * t1480 + t1484 * t1476)
        t16733 = t15895 * t1506
        t15899 = t4224 * (t4201 * t4217 + t4214 * t4208 + t4212 * t4204)
        t16739 = t15899 * t4260
        t16742 = (t16733 - t16739) * t94 / 0.2E1
        t15907 = t7812 * (t7789 * t7805 + t7796 * t7802 + t7800 * t7792)
        t16748 = t15907 * t7848
        t16751 = (t16739 - t16748) * t94 / 0.2E1
        t15914 = t8770 * (t8747 * t8763 + t8760 * t8754 + t8758 * t8750)
        t16759 = t15914 * t8778
        t16761 = t15899 * t4232
        t16764 = (t16759 - t16761) * t236 / 0.2E1
        t15921 = t8918 * (t8895 * t8911 + t8908 * t8902 + t8906 * t8898)
        t16770 = t15921 * t8926
        t16773 = (t16761 - t16770) * t236 / 0.2E1
        t16775 = t4347 / 0.2E1 + t16253 / 0.2E1
        t16777 = t8198 * t16775
        t16779 = t3962 * t16715
        t16782 = (t16777 - t16779) * t236 / 0.2E1
        t16784 = t4362 / 0.2E1 + t16267 / 0.2E1
        t16786 = t8340 * t16784
        t16789 = (t16779 - t16786) * t236 / 0.2E1
        t16790 = t8763 ** 2
        t16791 = t8754 ** 2
        t16792 = t8750 ** 2
        t16794 = t8769 * (t16790 + t16791 + t16792)
        t16795 = t4217 ** 2
        t16796 = t4208 ** 2
        t16797 = t4204 ** 2
        t16799 = t4223 * (t16795 + t16796 + t16797)
        t16802 = t4 * (t16794 / 0.2E1 + t16799 / 0.2E1)
        t16803 = t16802 * t4255
        t16804 = t8911 ** 2
        t16805 = t8902 ** 2
        t16806 = t8898 ** 2
        t16808 = t8917 * (t16804 + t16805 + t16806)
        t16811 = t4 * (t16799 / 0.2E1 + t16808 / 0.2E1)
        t16812 = t16811 * t4258
        t16815 = (t16698 - t16707) * t94 + t16720 + t16727 + t16742 + t1
     #6751 + t4237 + t16056 / 0.2E1 + t16009 + t4265 + t16198 / 0.2E1 + 
     #t16764 + t16773 + t16782 + t16789 + (t16803 - t16812) * t236
        t16816 = t16815 * t4222
        t16818 = (t4396 - t16816) * t183
        t16820 = t4398 / 0.2E1 + t16818 / 0.2E1
        t16821 = t14418 * t16820
        t16829 = t1327 * (t7098 - dy * (t7100 - t16334) / 0.12E2) / 0.12
     #E2
        t16834 = t3342 ** 2
        t16835 = t3355 ** 2
        t16836 = t3353 ** 2
        t16845 = u(t64,t15930,k,n)
        t16864 = rx(t5,t15930,k,0,0)
        t16865 = rx(t5,t15930,k,1,1)
        t16867 = rx(t5,t15930,k,2,2)
        t16869 = rx(t5,t15930,k,1,2)
        t16871 = rx(t5,t15930,k,2,1)
        t16873 = rx(t5,t15930,k,1,0)
        t16875 = rx(t5,t15930,k,0,2)
        t16877 = rx(t5,t15930,k,0,1)
        t16880 = rx(t5,t15930,k,2,0)
        t16886 = 0.1E1 / (t16864 * t16865 * t16867 - t16864 * t16869 * t
     #16871 + t16873 * t16871 * t16875 - t16873 * t16877 * t16867 + t168
     #80 * t16877 * t16869 - t16880 * t16865 * t16875)
        t16887 = t4 * t16886
        t16901 = t16873 ** 2
        t16902 = t16865 ** 2
        t16903 = t16869 ** 2
        t16916 = u(t5,t15930,t233,n)
        t16919 = u(t5,t15930,t238,n)
        t16936 = t15895 * t1882
        t16952 = t1338 / 0.2E1 + (t1336 - t16916) * t183 / 0.2E1
        t16956 = t1347 * t16711
        t16963 = t1372 / 0.2E1 + (t1370 - t16919) * t183 / 0.2E1
        t16969 = t5946 ** 2
        t16970 = t5937 ** 2
        t16971 = t5933 ** 2
        t16974 = t1489 ** 2
        t16975 = t1480 ** 2
        t16976 = t1476 ** 2
        t16978 = t1495 * (t16974 + t16975 + t16976)
        t16983 = t6126 ** 2
        t16984 = t6117 ** 2
        t16985 = t6113 ** 2
        t16072 = t5953 * (t5930 * t5946 + t5943 * t5937 + t5941 * t5933)
        t16077 = t6133 * (t6110 * t6126 + t6123 * t6117 + t6121 * t6113)
        t16994 = (t4 * (t3364 * (t16834 + t16835 + t16836) / 0.2E1 + t16
     #689 / 0.2E1) * t1878 - t16698) * t94 + (t3278 * (t1394 / 0.2E1 + (
     #t1392 - t16845) * t183 / 0.2E1) - t16713) * t94 / 0.2E1 + t16720 +
     # (t3365 * (t3342 * t3358 + t3355 * t3349 + t3353 * t3345) * t3401 
     #- t16733) * t94 / 0.2E1 + t16742 + t3733 + (t1884 - t16887 * (t168
     #64 * t16873 + t16877 * t16865 + t16875 * t16869) * ((t16845 - t159
     #31) * t94 / 0.2E1 + t16048 / 0.2E1)) * t183 / 0.2E1 + (t1554 - t4 
     #* (t1550 / 0.2E1 + t16886 * (t16901 + t16902 + t16903) / 0.2E1) * 
     #t15933) * t183 + t3734 + (t1508 - t16887 * (t16873 * t16880 + t168
     #71 * t16865 + t16869 * t16867) * ((t16916 - t15931) * t236 / 0.2E1
     # + (t15931 - t16919) * t236 / 0.2E1)) * t183 / 0.2E1 + (t16072 * t
     #5963 - t16936) * t236 / 0.2E1 + (t16936 - t16077 * t6143) * t236 /
     # 0.2E1 + (t5676 * t16952 - t16956) * t236 / 0.2E1 + (t16956 - t584
     #6 * t16963) * t236 / 0.2E1 + (t4 * (t5952 * (t16969 + t16970 + t16
     #971) / 0.2E1 + t16978 / 0.2E1) * t1502 - t4 * (t16978 / 0.2E1 + t6
     #132 * (t16983 + t16984 + t16985) / 0.2E1) * t1504) * t236
        t16995 = t16994 * t1494
        t17003 = t669 * t16820
        t17007 = t11519 ** 2
        t17008 = t11532 ** 2
        t17009 = t11530 ** 2
        t17018 = u(t6458,t15930,k,n)
        t17037 = rx(t96,t15930,k,0,0)
        t17038 = rx(t96,t15930,k,1,1)
        t17040 = rx(t96,t15930,k,2,2)
        t17042 = rx(t96,t15930,k,1,2)
        t17044 = rx(t96,t15930,k,2,1)
        t17046 = rx(t96,t15930,k,1,0)
        t17048 = rx(t96,t15930,k,0,2)
        t17050 = rx(t96,t15930,k,0,1)
        t17053 = rx(t96,t15930,k,2,0)
        t17059 = 0.1E1 / (t17037 * t17038 * t17040 - t17037 * t17042 * t
     #17044 + t17046 * t17044 * t17048 - t17046 * t17050 * t17040 + t170
     #53 * t17050 * t17042 - t17053 * t17038 * t17048)
        t17060 = t4 * t17059
        t17074 = t17046 ** 2
        t17075 = t17038 ** 2
        t17076 = t17042 ** 2
        t17089 = u(t96,t15930,t233,n)
        t17092 = u(t96,t15930,t238,n)
        t17109 = t15907 * t7820
        t17125 = t7935 / 0.2E1 + (t7841 - t17089) * t183 / 0.2E1
        t17129 = t7282 * t16722
        t17136 = t7950 / 0.2E1 + (t7844 - t17092) * t183 / 0.2E1
        t17142 = t12493 ** 2
        t17143 = t12484 ** 2
        t17144 = t12480 ** 2
        t17147 = t7805 ** 2
        t17148 = t7796 ** 2
        t17149 = t7792 ** 2
        t17151 = t7811 * (t17147 + t17148 + t17149)
        t17156 = t12641 ** 2
        t17157 = t12632 ** 2
        t17158 = t12628 ** 2
        t16203 = t12500 * (t12477 * t12493 + t12490 * t12484 + t12488 * 
     #t12480)
        t16208 = t12648 * (t12625 * t12641 + t12638 * t12632 + t12636 * 
     #t12628)
        t17167 = (t16707 - t4 * (t16703 / 0.2E1 + t11541 * (t17007 + t17
     #008 + t17009) / 0.2E1) * t7818) * t94 + t16727 + (t16724 - t10949 
     #* (t7764 / 0.2E1 + (t7762 - t17018) * t183 / 0.2E1)) * t94 / 0.2E1
     # + t16751 + (t16748 - t11542 * (t11519 * t11535 + t11532 * t11526 
     #+ t11530 * t11522) * t11578) * t94 / 0.2E1 + t7825 + (t7822 - t170
     #60 * (t17037 * t17046 + t17050 * t17038 + t17048 * t17042) * (t160
     #50 / 0.2E1 + (t15953 - t17018) * t94 / 0.2E1)) * t183 / 0.2E1 + (t
     #7834 - t4 * (t7830 / 0.2E1 + t17059 * (t17074 + t17075 + t17076) /
     # 0.2E1) * t15955) * t183 + t7853 + (t7850 - t17060 * (t17046 * t17
     #053 + t17038 * t17044 + t17042 * t17040) * ((t17089 - t15953) * t2
     #36 / 0.2E1 + (t15953 - t17092) * t236 / 0.2E1)) * t183 / 0.2E1 + (
     #t16203 * t12508 - t17109) * t236 / 0.2E1 + (t17109 - t16208 * t126
     #56) * t236 / 0.2E1 + (t11852 * t17125 - t17129) * t236 / 0.2E1 + (
     #t17129 - t11987 * t17136) * t236 / 0.2E1 + (t4 * (t12499 * (t17142
     # + t17143 + t17144) / 0.2E1 + t17151 / 0.2E1) * t7843 - t4 * (t171
     #51 / 0.2E1 + t12647 * (t17156 + t17157 + t17158) / 0.2E1) * t7846)
     # * t236
        t17168 = t17167 * t7810
        t17181 = t3487 * t9023
        t17204 = t5930 ** 2
        t17205 = t5943 ** 2
        t17206 = t5941 ** 2
        t17209 = t8747 ** 2
        t17210 = t8760 ** 2
        t17211 = t8758 ** 2
        t17213 = t8769 * (t17209 + t17210 + t17211)
        t17218 = t12477 ** 2
        t17219 = t12490 ** 2
        t17220 = t12488 ** 2
        t17232 = t8168 * t16775
        t17244 = t15914 * t8803
        t17253 = rx(i,t15930,t233,0,0)
        t17254 = rx(i,t15930,t233,1,1)
        t17256 = rx(i,t15930,t233,2,2)
        t17258 = rx(i,t15930,t233,1,2)
        t17260 = rx(i,t15930,t233,2,1)
        t17262 = rx(i,t15930,t233,1,0)
        t17264 = rx(i,t15930,t233,0,2)
        t17266 = rx(i,t15930,t233,0,1)
        t17269 = rx(i,t15930,t233,2,0)
        t17275 = 0.1E1 / (t17253 * t17254 * t17256 - t17253 * t17258 * t
     #17260 + t17262 * t17260 * t17264 - t17262 * t17266 * t17256 + t172
     #69 * t17266 * t17258 - t17269 * t17254 * t17264)
        t17276 = t4 * t17275
        t17292 = t17262 ** 2
        t17293 = t17254 ** 2
        t17294 = t17258 ** 2
        t17307 = u(i,t15930,t1229,n)
        t17317 = rx(i,t1335,t1229,0,0)
        t17318 = rx(i,t1335,t1229,1,1)
        t17320 = rx(i,t1335,t1229,2,2)
        t17322 = rx(i,t1335,t1229,1,2)
        t17324 = rx(i,t1335,t1229,2,1)
        t17326 = rx(i,t1335,t1229,1,0)
        t17328 = rx(i,t1335,t1229,0,2)
        t17330 = rx(i,t1335,t1229,0,1)
        t17333 = rx(i,t1335,t1229,2,0)
        t17339 = 0.1E1 / (t17317 * t17318 * t17320 - t17317 * t17322 * t
     #17324 + t17326 * t17324 * t17328 - t17326 * t17330 * t17320 + t173
     #33 * t17330 * t17322 - t17333 * t17318 * t17328)
        t17340 = t4 * t17339
        t17350 = (t5984 - t8799) * t94 / 0.2E1 + (t8799 - t12529) * t94 
     #/ 0.2E1
        t17369 = t17333 ** 2
        t17370 = t17324 ** 2
        t17371 = t17320 ** 2
        t16366 = t17340 * (t17326 * t17333 + t17318 * t17324 + t17322 * 
     #t17320)
        t17380 = (t4 * (t5952 * (t17204 + t17205 + t17206) / 0.2E1 + t17
     #213 / 0.2E1) * t5961 - t4 * (t17213 / 0.2E1 + t12499 * (t17218 + t
     #17219 + t17220) / 0.2E1) * t8776) * t94 + (t5661 * t16952 - t17232
     #) * t94 / 0.2E1 + (t17232 - t11835 * t17125) * t94 / 0.2E1 + (t160
     #72 * t5988 - t17244) * t94 / 0.2E1 + (t17244 - t16203 * t12533) * 
     #t94 / 0.2E1 + t8783 + (t8780 - t17276 * (t17253 * t17262 + t17266 
     #* t17254 + t17264 * t17258) * ((t16916 - t16187) * t94 / 0.2E1 + (
     #t16187 - t17089) * t94 / 0.2E1)) * t183 / 0.2E1 + (t8792 - t4 * (t
     #8788 / 0.2E1 + t17275 * (t17292 + t17293 + t17294) / 0.2E1) * t162
     #53) * t183 + t8808 + (t8805 - t17276 * (t17262 * t17269 + t17254 *
     # t17260 + t17258 * t17256) * ((t17307 - t16187) * t236 / 0.2E1 + t
     #16189 / 0.2E1)) * t183 / 0.2E1 + (t17340 * (t17317 * t17333 + t173
     #30 * t17324 + t17328 * t17320) * t17350 - t16759) * t236 / 0.2E1 +
     # t16764 + (t16366 * (t8851 / 0.2E1 + (t8799 - t17307) * t183 / 0.2
     #E1) - t16777) * t236 / 0.2E1 + t16782 + (t4 * (t17339 * (t17369 + 
     #t17370 + t17371) / 0.2E1 + t16794 / 0.2E1) * t8801 - t16803) * t23
     #6
        t17381 = t17380 * t8768
        t17384 = t6110 ** 2
        t17385 = t6123 ** 2
        t17386 = t6121 ** 2
        t17389 = t8895 ** 2
        t17390 = t8908 ** 2
        t17391 = t8906 ** 2
        t17393 = t8917 * (t17389 + t17390 + t17391)
        t17398 = t12625 ** 2
        t17399 = t12638 ** 2
        t17400 = t12636 ** 2
        t17412 = t8318 * t16784
        t17424 = t15921 * t8951
        t17433 = rx(i,t15930,t238,0,0)
        t17434 = rx(i,t15930,t238,1,1)
        t17436 = rx(i,t15930,t238,2,2)
        t17438 = rx(i,t15930,t238,1,2)
        t17440 = rx(i,t15930,t238,2,1)
        t17442 = rx(i,t15930,t238,1,0)
        t17444 = rx(i,t15930,t238,0,2)
        t17446 = rx(i,t15930,t238,0,1)
        t17449 = rx(i,t15930,t238,2,0)
        t17455 = 0.1E1 / (t17433 * t17434 * t17436 - t17433 * t17438 * t
     #17440 + t17442 * t17440 * t17444 - t17442 * t17446 * t17436 + t174
     #49 * t17446 * t17438 - t17449 * t17434 * t17444)
        t17456 = t4 * t17455
        t17472 = t17442 ** 2
        t17473 = t17434 ** 2
        t17474 = t17438 ** 2
        t17487 = u(i,t15930,t1277,n)
        t17497 = rx(i,t1335,t1277,0,0)
        t17498 = rx(i,t1335,t1277,1,1)
        t17500 = rx(i,t1335,t1277,2,2)
        t17502 = rx(i,t1335,t1277,1,2)
        t17504 = rx(i,t1335,t1277,2,1)
        t17506 = rx(i,t1335,t1277,1,0)
        t17508 = rx(i,t1335,t1277,0,2)
        t17510 = rx(i,t1335,t1277,0,1)
        t17513 = rx(i,t1335,t1277,2,0)
        t17519 = 0.1E1 / (t17497 * t17498 * t17500 - t17497 * t17502 * t
     #17504 + t17506 * t17504 * t17508 - t17506 * t17510 * t17500 + t175
     #13 * t17510 * t17502 - t17513 * t17498 * t17508)
        t17520 = t4 * t17519
        t17530 = (t6164 - t8947) * t94 / 0.2E1 + (t8947 - t12677) * t94 
     #/ 0.2E1
        t17549 = t17513 ** 2
        t17550 = t17504 ** 2
        t17551 = t17500 ** 2
        t16510 = t17520 * (t17506 * t17513 + t17498 * t17504 + t17502 * 
     #t17500)
        t17560 = (t4 * (t6132 * (t17384 + t17385 + t17386) / 0.2E1 + t17
     #393 / 0.2E1) * t6141 - t4 * (t17393 / 0.2E1 + t12647 * (t17398 + t
     #17399 + t17400) / 0.2E1) * t8924) * t94 + (t5830 * t16963 - t17412
     #) * t94 / 0.2E1 + (t17412 - t11973 * t17136) * t94 / 0.2E1 + (t160
     #77 * t6168 - t17424) * t94 / 0.2E1 + (t17424 - t16208 * t12681) * 
     #t94 / 0.2E1 + t8931 + (t8928 - t17456 * (t17433 * t17442 + t17446 
     #* t17434 + t17438 * t17444) * ((t16919 - t16190) * t94 / 0.2E1 + (
     #t16190 - t17092) * t94 / 0.2E1)) * t183 / 0.2E1 + (t8940 - t4 * (t
     #8936 / 0.2E1 + t17455 * (t17472 + t17473 + t17474) / 0.2E1) * t162
     #67) * t183 + t8956 + (t8953 - t17456 * (t17442 * t17449 + t17434 *
     # t17440 + t17438 * t17436) * (t16192 / 0.2E1 + (t16190 - t17487) *
     # t236 / 0.2E1)) * t183 / 0.2E1 + t16773 + (t16770 - t17520 * (t174
     #97 * t17513 + t17510 * t17504 + t17508 * t17500) * t17530) * t236 
     #/ 0.2E1 + t16789 + (t16786 - t16510 * (t8999 / 0.2E1 + (t8947 - t1
     #7487) * t183 / 0.2E1)) * t236 / 0.2E1 + (t16812 - t4 * (t16808 / 0
     #.2E1 + t17519 * (t17549 + t17550 + t17551) / 0.2E1) * t8949) * t23
     #6
        t17561 = t17560 * t8916
        t17576 = (t6058 - t8871) * t94 / 0.2E1 + (t8871 - t12601) * t94 
     #/ 0.2E1
        t17580 = t3487 * t8411
        t17589 = (t6238 - t9019) * t94 / 0.2E1 + (t9019 - t12749) * t94 
     #/ 0.2E1
        t17602 = t731 * t16820
        t17619 = (t3713 * t5500 - t4165 * t8409) * t94 + (t348 * (t3863 
     #/ 0.2E1 + (t3861 - t16995) * t183 / 0.2E1) - t17003) * t94 / 0.2E1
     # + (t17003 - t3883 * (t7986 / 0.2E1 + (t7984 - t17168) * t183 / 0.
     #2E1)) * t94 / 0.2E1 + (t3101 * t6242 - t17181) * t94 / 0.2E1 + (t1
     #7181 - t3899 * t12753) * t94 / 0.2E1 + t8416 + (t8413 - t3941 * ((
     #t16995 - t16816) * t94 / 0.2E1 + (t16816 - t17168) * t94 / 0.2E1))
     # * t183 / 0.2E1 + (t8418 - t4245 * t16818) * t183 + t9028 + (t9025
     # - t3962 * ((t17381 - t16816) * t236 / 0.2E1 + (t16816 - t17561) *
     # t236 / 0.2E1)) * t183 / 0.2E1 + (t4006 * t17576 - t17580) * t236 
     #/ 0.2E1 + (t17580 - t4043 * t17589) * t236 / 0.2E1 + (t4056 * (t90
     #52 / 0.2E1 + (t8871 - t17381) * t183 / 0.2E1) - t17602) * t236 / 0
     #.2E1 + (t17602 - t4067 * (t9065 / 0.2E1 + (t9019 - t17561) * t183 
     #/ 0.2E1)) * t236 / 0.2E1 + (t4382 * t8873 - t4391 * t9021) * t236
        t17621 = t895 * t17619 * t672
        t17633 = t2615 / 0.2E1 + t16330 / 0.2E1
        t17635 = t3941 * t17633
        t17649 = t15899 * t7211
        t17665 = (t2099 - t6995) * t94 / 0.2E1 + (t6995 - t10728) * t94 
     #/ 0.2E1
        t17669 = t15899 * t7270
        t17678 = (t2103 - t7016) * t94 / 0.2E1 + (t7016 - t10731) * t94 
     #/ 0.2E1
        t17689 = t3962 * t17633
        t17704 = (t16697 * t2147 - t16706 * t7268) * t94 + (t1731 * (t22
     #66 / 0.2E1 + t16413 / 0.2E1) - t17635) * t94 / 0.2E1 + (t17635 - t
     #7261 * (t7040 / 0.2E1 + t16432 / 0.2E1)) * t94 / 0.2E1 + (t15895 *
     # t2107 - t17649) * t94 / 0.2E1 + (t17649 - t15907 * t10735) * t94 
     #/ 0.2E1 + t9600 + t16493 / 0.2E1 + t16451 + t9601 + t16475 / 0.2E1
     # + (t15914 * t17665 - t17669) * t236 / 0.2E1 + (t17669 - t15921 * 
     #t17678) * t236 / 0.2E1 + (t8198 * (t6997 / 0.2E1 + t16637 / 0.2E1)
     # - t17689) * t236 / 0.2E1 + (t17689 - t8340 * (t7018 / 0.2E1 + t16
     #651 / 0.2E1)) * t236 / 0.2E1 + (t16802 * t7207 - t16811 * t7209) *
     # t236
        t17710 = t15358 * (t9645 / 0.2E1 + (t9643 - t17704 * t4222) * t1
     #83 / 0.2E1)
        t17714 = t14418 * (t4398 - t16818)
        t17717 = t2 + t6846 - t15454 + t7310 - t15457 + t15463 + t9081 -
     # t15468 + t15472 - t926 - t1227 * t16325 - t16341 - t2079 * t16682
     # / 0.2E1 - t1227 * t16821 / 0.2E1 - t16829 - t2923 * t17621 / 0.6E
     #1 - t2079 * t17710 / 0.4E1 - t1227 * t17714 / 0.12E2
        t17730 = sqrt(t15477 + t15478 + t15479 + 0.8E1 * t702 + 0.8E1 * 
     #t703 + 0.8E1 * t704 - 0.2E1 * dy * ((t688 + t689 + t690 - t693 - t
     #694 - t695) * t183 / 0.2E1 - (t702 + t703 + t704 - t4238 - t4239 -
     # t4240) * t183 / 0.2E1))
        t17731 = 0.1E1 / t17730
        t17736 = t6453 * t9213 * t15913
        t17739 = t709 * t9216 * t15918 / 0.2E1
        t17742 = t709 * t9220 * t15923 / 0.6E1
        t17744 = t9213 * t15926 / 0.24E2
        t17756 = t2 + t9240 - t15454 + t9242 - t15516 + t15463 + t9246 -
     # t15518 + t15520 - t926 - t9226 * t16325 - t16341 - t9228 * t16682
     # / 0.2E1 - t9226 * t16821 / 0.2E1 - t16829 - t9233 * t17621 / 0.6E
     #1 - t9228 * t17710 / 0.4E1 - t9226 * t17714 / 0.12E2
        t17759 = 0.2E1 * t15929 * t17756 * t17731
        t17761 = (t6453 * t136 * t15913 + t709 * t159 * t15918 / 0.2E1 +
     # t709 * t893 * t15923 / 0.6E1 - t136 * t15926 / 0.24E2 + 0.2E1 * t
     #15929 * t17717 * t17731 - t17736 - t17739 - t17742 + t17744 - t177
     #59) * t133
        t17767 = t6453 * (t221 - dy * t6685 / 0.24E2)
        t17769 = dy * t6692 / 0.24E2
        t17785 = t4 * (t15542 + t15546 / 0.2E1 - dy * ((t15539 - t15541)
     # * t183 / 0.2E1 - (t15546 - t4223 * t4252) * t183 / 0.2E1) / 0.8E1
     #)
        t17796 = (t7207 - t7209) * t236
        t17813 = t9762 + t9763 - t9767 + t1156 / 0.4E1 + t1159 / 0.4E1 -
     # t15596 / 0.12E2 - dy * ((t15577 + t15578 - t15579 - t9789 - t9790
     # + t9791) * t183 / 0.2E1 - (t15582 + t15583 - t15597 - t7207 / 0.2
     #E1 - t7209 / 0.2E1 + t1228 * (((t16348 - t7207) * t236 - t17796) *
     # t236 / 0.2E1 + (t17796 - (t7209 - t16354) * t236) * t236 / 0.2E1)
     # / 0.6E1) * t183 / 0.2E1) / 0.8E1
        t17818 = t4 * (t15541 / 0.2E1 + t15546 / 0.2E1)
        t17820 = t5278 / 0.4E1 + t5476 / 0.4E1 + t8873 / 0.4E1 + t9021 /
     # 0.4E1
        t17831 = t4832 * t9623
        t17843 = t4006 * t10010
        t17855 = (t9992 - t8168 * t17665) * t183
        t17859 = (t9997 - t8791 * t6997) * t183
        t17865 = (t10012 - t8198 * (t16348 / 0.2E1 + t7207 / 0.2E1)) * t
     #183
        t17869 = (t5902 * t9479 - t8733 * t9603) * t94 + (t4694 * t9501 
     #- t17831) * t94 / 0.2E1 + (t17831 - t7520 * t13127) * t94 / 0.2E1 
     #+ (t3511 * t9885 - t17843) * t94 / 0.2E1 + (t17843 - t7317 * t1330
     #2) * t94 / 0.2E1 + t9995 + t17855 / 0.2E1 + t17859 + t10015 + t178
     #65 / 0.2E1 + t16611 / 0.2E1 + t9612 + t16375 / 0.2E1 + t9630 + t16
     #556
        t17870 = t17869 * t4287
        t17880 = t4971 * t9632
        t17892 = t4043 * t10063
        t17904 = (t10045 - t8318 * t17678) * t183
        t17908 = (t10050 - t8939 * t7018) * t183
        t17914 = (t10065 - t8340 * (t7209 / 0.2E1 + t16354 / 0.2E1)) * t
     #183
        t17918 = (t6082 * t9492 - t8881 * t9614) * t94 + (t4743 * t9510 
     #- t17880) * t94 / 0.2E1 + (t17880 - t7702 * t13136) * t94 / 0.2E1 
     #+ (t3543 * t9957 - t17892) * t94 / 0.2E1 + (t17892 - t7364 * t1335
     #5) * t94 / 0.2E1 + t10048 + t17904 / 0.2E1 + t17908 + t10068 + t17
     #914 / 0.2E1 + t9621 + t16627 / 0.2E1 + t9637 + t16389 / 0.2E1 + t1
     #6561
        t17919 = t17918 * t4326
        t17923 = t10021 / 0.4E1 + t10074 / 0.4E1 + (t17870 - t9643) * t2
     #36 / 0.4E1 + (t9643 - t17919) * t236 / 0.4E1
        t17929 = dy * (t1152 / 0.2E1 - t7215 / 0.2E1)
        t17933 = t17785 * t9213 * t17813
        t17936 = t17818 * t9658 * t17820 / 0.2E1
        t17939 = t17818 * t9662 * t17923 / 0.6E1
        t17941 = t9213 * t17929 / 0.24E2
        t17943 = (t17785 * t136 * t17813 + t17818 * t9349 * t17820 / 0.2
     #E1 + t17818 * t9355 * t17923 / 0.6E1 - t136 * t17929 / 0.24E2 - t1
     #7933 - t17936 - t17939 + t17941) * t133
        t17956 = (t4255 - t4258) * t236
        t17974 = t17785 * (t10105 + t10106 - t10110 + t742 / 0.4E1 + t74
     #5 / 0.4E1 - t15778 / 0.12E2 - dy * ((t15759 + t15760 - t15761 - t1
     #0132 - t10133 + t10134) * t183 / 0.2E1 - (t15764 + t15765 - t15779
     # - t4255 / 0.2E1 - t4258 / 0.2E1 + t1228 * (((t8801 - t4255) * t23
     #6 - t17956) * t236 / 0.2E1 + (t17956 - (t4258 - t8949) * t236) * t
     #236 / 0.2E1) / 0.6E1) * t183 / 0.2E1) / 0.8E1)
        t17978 = dy * (t734 / 0.2E1 - t4264 / 0.2E1) / 0.24E2
        t17983 = t13543 * t161 / 0.6E1 + (t13615 + t13533 + t13536 - t13
     #619 + t13539 - t13541 - t13543 * t9212) * t161 / 0.2E1 + t15526 * 
     #t161 / 0.6E1 + (t15532 + t15495 + t15498 - t15534 + t15501 - t1550
     #3 + t15524 - t15526 * t9212) * t161 / 0.2E1 + t15734 * t161 / 0.6E
     #1 + (t15787 + t15724 + t15727 - t15791 + t15730 - t15732 - t15734 
     #* t9212) * t161 / 0.2E1 - t15871 * t161 / 0.6E1 - (t15902 + t15861
     # + t15864 - t15906 + t15867 - t15869 - t15871 * t9212) * t161 / 0.
     #2E1 - t17761 * t161 / 0.6E1 - (t17767 + t17736 + t17739 - t17769 +
     # t17742 - t17744 + t17759 - t17761 * t9212) * t161 / 0.2E1 - t1794
     #3 * t161 / 0.6E1 - (t17974 + t17933 + t17936 - t17978 + t17939 - t
     #17941 - t17943 * t9212) * t161 / 0.2E1
        t17986 = t775 * t780
        t17991 = t814 * t819
        t17999 = t4 * (t17986 / 0.2E1 + t9743 - dz * ((t5234 * t5239 - t
     #17986) * t236 / 0.2E1 - (t9742 - t17991) * t236 / 0.2E1) / 0.8E1)
        t18005 = (t1033 - t1168) * t94
        t18007 = ((t1031 - t1033) * t94 - t18005) * t94
        t18011 = (t18005 - (t1168 - t7225) * t94) * t94
        t18014 = t1701 * (t18007 / 0.2E1 + t18011 / 0.2E1)
        t18021 = (t2212 - t7059) * t94
        t18032 = t1033 / 0.2E1
        t18033 = t1168 / 0.2E1
        t18034 = t18014 / 0.6E1
        t18037 = t1046 / 0.2E1
        t18038 = t1179 / 0.2E1
        t18042 = (t1046 - t1179) * t94
        t18044 = ((t1044 - t1046) * t94 - t18042) * t94
        t18048 = (t18042 - (t1179 - t7239) * t94) * t94
        t18051 = t1701 * (t18044 / 0.2E1 + t18048 / 0.2E1)
        t18052 = t18051 / 0.6E1
        t18059 = t1033 / 0.4E1 + t1168 / 0.4E1 - t18014 / 0.12E2 + t1346
     #1 + t13462 - t13463 - dz * ((t2212 / 0.2E1 + t7059 / 0.2E1 - t1701
     # * (((t2209 - t2212) * t94 - t18021) * t94 / 0.2E1 + (t18021 - (t7
     #059 - t10956) * t94) * t94 / 0.2E1) / 0.6E1 - t18032 - t18033 + t1
     #8034) * t236 / 0.2E1 - (t2062 + t6847 - t6858 - t18037 - t18038 + 
     #t18052) * t236 / 0.2E1) / 0.8E1
        t18064 = t4 * (t17986 / 0.2E1 + t9742 / 0.2E1)
        t18066 = t6251 / 0.4E1 + t9030 / 0.4E1 + t2910 / 0.4E1 + t7444 /
     # 0.4E1
        t18075 = (t9894 - t10019) * t94 / 0.4E1 + (t10019 - t13311) * t9
     #4 / 0.4E1 + t6406 / 0.4E1 + t9181 / 0.4E1
        t18081 = dz * (t7065 / 0.2E1 - t1185 / 0.2E1)
        t18085 = t17999 * t9213 * t18059
        t18088 = t18064 * t9658 * t18066 / 0.2E1
        t18091 = t18064 * t9662 * t18075 / 0.6E1
        t18093 = t9213 * t18081 / 0.24E2
        t18095 = (t17999 * t136 * t18059 + t18064 * t9349 * t18066 / 0.2
     #E1 + t18064 * t9355 * t18075 / 0.6E1 - t136 * t18081 / 0.24E2 - t1
     #8085 - t18088 - t18091 + t18093) * t133
        t18103 = (t458 - t782) * t94
        t18105 = ((t456 - t458) * t94 - t18103) * t94
        t18109 = (t18103 - (t782 - t6625) * t94) * t94
        t18112 = t1701 * (t18105 / 0.2E1 + t18109 / 0.2E1)
        t18119 = (t1264 - t5241) * t94
        t18130 = t458 / 0.2E1
        t18131 = t782 / 0.2E1
        t18132 = t18112 / 0.6E1
        t18135 = t499 / 0.2E1
        t18136 = t821 / 0.2E1
        t18140 = (t499 - t821) * t94
        t18142 = ((t497 - t499) * t94 - t18140) * t94
        t18146 = (t18140 - (t821 - t6646) * t94) * t94
        t18149 = t1701 * (t18142 / 0.2E1 + t18146 / 0.2E1)
        t18150 = t18149 / 0.6E1
        t18158 = t17999 * (t458 / 0.4E1 + t782 / 0.4E1 - t18112 / 0.12E2
     # + t13562 + t13563 - t13567 - dz * ((t1264 / 0.2E1 + t5241 / 0.2E1
     # - t1701 * (((t1261 - t1264) * t94 - t18119) * t94 / 0.2E1 + (t181
     #19 - (t5241 - t8154) * t94) * t94 / 0.2E1) / 0.6E1 - t18130 - t181
     #31 + t18132) * t236 / 0.2E1 - (t13589 + t13590 - t13591 - t18135 -
     # t18136 + t18150) * t236 / 0.2E1) / 0.8E1)
        t18162 = dz * (t5247 / 0.2E1 - t827 / 0.2E1) / 0.24E2
        t18167 = t775 * t832
        t18172 = t814 * t849
        t18180 = t4 * (t18167 / 0.2E1 + t15542 - dz * ((t5234 * t5252 - 
     #t18167) * t236 / 0.2E1 - (t15541 - t18172) * t236 / 0.2E1) / 0.8E1
     #)
        t18186 = (t1188 - t1190) * t183
        t18188 = ((t6991 - t1188) * t183 - t18186) * t183
        t18192 = (t18186 - (t1190 - t6997) * t183) * t183
        t18195 = t1327 * (t18188 / 0.2E1 + t18192 / 0.2E1)
        t18202 = (t7120 - t7122) * t183
        t18213 = t1188 / 0.2E1
        t18214 = t1190 / 0.2E1
        t18215 = t18195 / 0.6E1
        t18218 = t1201 / 0.2E1
        t18219 = t1203 / 0.2E1
        t18223 = (t1201 - t1203) * t183
        t18225 = ((t7012 - t1201) * t183 - t18223) * t183
        t18229 = (t18223 - (t1203 - t7018) * t183) * t183
        t18232 = t1327 * (t18225 / 0.2E1 + t18229 / 0.2E1)
        t18233 = t18232 / 0.6E1
        t18240 = t1188 / 0.4E1 + t1190 / 0.4E1 - t18195 / 0.12E2 + t9291
     # + t9292 - t9296 - dz * ((t7120 / 0.2E1 + t7122 / 0.2E1 - t1327 * 
     #(((t14213 - t7120) * t183 - t18202) * t183 / 0.2E1 + (t18202 - (t7
     #122 - t16369) * t183) * t183 / 0.2E1) / 0.6E1 - t18213 - t18214 + 
     #t18215) * t236 / 0.2E1 - (t9318 + t9319 - t9320 - t18218 - t18219 
     #+ t18233) * t236 / 0.2E1) / 0.8E1
        t18245 = t4 * (t18167 / 0.2E1 + t15541 / 0.2E1)
        t18247 = t9050 / 0.4E1 + t9052 / 0.4E1 + t4134 / 0.4E1 + t4398 /
     # 0.4E1
        t18256 = (t15661 - t10019) * t183 / 0.4E1 + (t10019 - t17870) * 
     #t183 / 0.4E1 + t9584 / 0.4E1 + t9645 / 0.4E1
        t18262 = dz * (t7128 / 0.2E1 - t1209 / 0.2E1)
        t18266 = t18180 * t9213 * t18240
        t18269 = t18245 * t9658 * t18247 / 0.2E1
        t18272 = t18245 * t9662 * t18256 / 0.6E1
        t18274 = t9213 * t18262 / 0.24E2
        t18276 = (t18180 * t136 * t18240 + t18245 * t9349 * t18247 / 0.2
     #E1 + t18245 * t9355 * t18256 / 0.6E1 - t136 * t18262 / 0.24E2 - t1
     #8266 - t18269 - t18272 + t18274) * t133
        t18284 = (t834 - t836) * t183
        t18286 = ((t4083 - t834) * t183 - t18284) * t183
        t18290 = (t18284 - (t836 - t4347) * t183) * t183
        t18293 = t1327 * (t18286 / 0.2E1 + t18290 / 0.2E1)
        t18300 = (t5254 - t5256) * t183
        t18311 = t834 / 0.2E1
        t18312 = t836 / 0.2E1
        t18313 = t18293 / 0.6E1
        t18316 = t851 / 0.2E1
        t18317 = t853 / 0.2E1
        t18321 = (t851 - t853) * t183
        t18323 = ((t4098 - t851) * t183 - t18321) * t183
        t18327 = (t18321 - (t853 - t4362) * t183) * t183
        t18330 = t1327 * (t18323 / 0.2E1 + t18327 / 0.2E1)
        t18331 = t18330 / 0.6E1
        t18339 = t18180 * (t834 / 0.4E1 + t836 / 0.4E1 - t18293 / 0.12E2
     # + t9678 + t9679 - t9683 - dz * ((t5254 / 0.2E1 + t5256 / 0.2E1 - 
     #t1327 * (((t8546 - t5254) * t183 - t18300) * t183 / 0.2E1 + (t1830
     #0 - (t5256 - t8851) * t183) * t183 / 0.2E1) / 0.6E1 - t18311 - t18
     #312 + t18313) * t236 / 0.2E1 - (t9705 + t9706 - t9707 - t18316 - t
     #18317 + t18331) * t236 / 0.2E1) / 0.8E1)
        t18343 = dz * (t5262 / 0.2E1 - t859 / 0.2E1) / 0.24E2
        t18350 = t961 - dz * t6866 / 0.24E2
        t18355 = t161 * t5277 * t236
        t18360 = t895 * t10020 * t236
        t18363 = dz * t6879
        t18366 = cc * t6809
        t18388 = t4670 * t6230
        t18400 = k + 3
        t18401 = u(i,j,t18400,n)
        t18403 = (t18401 - t1262) * t236
        t18411 = rx(i,j,t18400,0,0)
        t18412 = rx(i,j,t18400,1,1)
        t18414 = rx(i,j,t18400,2,2)
        t18416 = rx(i,j,t18400,1,2)
        t18418 = rx(i,j,t18400,2,1)
        t18420 = rx(i,j,t18400,1,0)
        t18422 = rx(i,j,t18400,0,2)
        t18424 = rx(i,j,t18400,0,1)
        t18427 = rx(i,j,t18400,2,0)
        t18433 = 0.1E1 / (t18411 * t18412 * t18414 - t18411 * t18416 * t
     #18418 + t18420 * t18418 * t18422 - t18420 * t18424 * t18414 + t184
     #27 * t18424 * t18416 - t18427 * t18412 * t18422)
        t18434 = t18427 ** 2
        t18435 = t18418 ** 2
        t18436 = t18414 ** 2
        t18438 = t18433 * (t18434 + t18435 + t18436)
        t18441 = t4 * (t18438 / 0.2E1 + t5268 / 0.2E1)
        t18444 = (t18441 * t18403 - t5272) * t236
        t18452 = u(t5,j,t18400,n)
        t18454 = (t18452 - t1259) * t236
        t17457 = ((t18403 / 0.2E1 - t269 / 0.2E1) * t236 - t1685) * t236
        t18468 = t771 * t17457
        t18471 = u(t96,j,t18400,n)
        t18473 = (t18471 - t5135) * t236
        t18500 = u(i,t180,t18400,n)
        t18502 = (t18500 - t5190) * t236
        t18512 = t822 * t17457
        t18515 = u(i,t185,t18400,n)
        t18517 = (t18515 - t5202) * t236
        t18531 = t4 * t18433
        t18537 = (t18500 - t18401) * t183
        t18539 = (t18401 - t18515) * t183
        t17466 = t18531 * (t18420 * t18427 + t18412 * t18418 + t18416 * 
     #t18414)
        t18545 = (t17466 * (t18537 / 0.2E1 + t18539 / 0.2E1) - t5260) * 
     #t236
        t18562 = t4 * (t5268 / 0.2E1 + t6799 - dz * ((t18438 - t5268) * 
     #t236 / 0.2E1 - t6814 / 0.2E1) / 0.8E1)
        t18584 = t4670 * t6258
        t18596 = t5144 + t5164 + t5201 - t1701 * ((t4872 * ((t1261 / 0.2
     #E1 - t5241 / 0.2E1) * t94 - (t1264 / 0.2E1 - t8154 / 0.2E1) * t94)
     # * t94 - t6632) * t236 / 0.2E1 + t6643 / 0.2E1) / 0.6E1 - t1327 * 
     #((t4153 * t1493 - t18388) * t94 / 0.2E1 + (t18388 - t4804 * t9779)
     # * t94 / 0.2E1) / 0.6E1 + t5248 - t1228 * ((t5271 * ((t18403 - t16
     #82) * t236 - t6539) * t236 - t6544) * t236 + ((t18444 - t5274) * t
     #236 - t6553) * t236) / 0.24E2 - t1228 * ((t452 * ((t18454 / 0.2E1 
     #- t252 / 0.2E1) * t236 - t1584) * t236 - t18468) * t94 / 0.2E1 + (
     #t18468 - t4813 * ((t18473 / 0.2E1 - t599 / 0.2E1) * t236 - t6662) 
     #* t236) * t94 / 0.2E1) / 0.6E1 - t1327 * ((t5177 * t18286 - t5186 
     #* t18290) * t183 + ((t8489 - t5189) * t183 - (t5189 - t8794) * t18
     #3) * t183) / 0.24E2 - t1228 * ((t3806 * ((t18502 / 0.2E1 - t719 / 
     #0.2E1) * t236 - t6594) * t236 - t18512) * t183 / 0.2E1 + (t18512 -
     # t4056 * ((t18517 / 0.2E1 - t742 / 0.2E1) * t236 - t6609) * t236) 
     #* t183 / 0.2E1) / 0.6E1 + t5155 - t1228 * (((t18545 - t5262) * t23
     #6 - t6786) * t236 / 0.2E1 + t6790 / 0.2E1) / 0.6E1 + (t18562 * t16
     #82 - t6811) * t236 - t1701 * ((t4893 * t18105 - t5111 * t18109) * 
     #t94 + ((t4896 - t5114) * t94 - (t5114 - t8027) * t94) * t94) / 0.2
     #4E2 - t1701 * ((t4823 * t13258 - t18584) * t183 / 0.2E1 + (t18584 
     #- t4832 * t15405) * t183 / 0.2E1) / 0.6E1
        t18616 = (t5154 - t5163) * t183
        t18628 = t5174 / 0.2E1
        t18638 = t4 * (t5169 / 0.2E1 + t18628 - dy * ((t8483 - t5169) * 
     #t183 / 0.2E1 - (t5174 - t5183) * t183 / 0.2E1) / 0.8E1)
        t18650 = t4 * (t18628 + t5183 / 0.2E1 - dy * ((t5169 - t5174) * 
     #t183 / 0.2E1 - (t5183 - t8788) * t183 / 0.2E1) / 0.8E1)
        t18657 = (t5200 - t5210) * t183
        t18671 = (t4904 - t5129) * t94
        t18685 = (t4911 - t5143) * t94
        t18697 = t4890 / 0.2E1
        t18707 = t4 * (t4447 / 0.2E1 + t18697 - dx * ((t4438 - t4447) * 
     #t94 / 0.2E1 - (t4890 - t5108) * t94 / 0.2E1) / 0.8E1)
        t18719 = t4 * (t18697 + t5108 / 0.2E1 - dx * ((t4447 - t4890) * 
     #t94 / 0.2E1 - (t5108 - t8021) * t94 / 0.2E1) / 0.8E1)
        t18728 = (t18452 - t18401) * t94
        t18730 = (t18401 - t18471) * t94
        t17688 = t18531 * (t18411 * t18427 + t18424 * t18418 + t18422 * 
     #t18414)
        t18736 = (t17688 * (t18728 / 0.2E1 + t18730 / 0.2E1) - t5245) * 
     #t236
        t18745 = t845 + t4905 + t4912 + t791 + t5263 - t1327 * ((t4882 *
     # ((t8546 / 0.2E1 - t5256 / 0.2E1) * t183 - (t5254 / 0.2E1 - t8851 
     #/ 0.2E1) * t183) * t183 - t6570) * t236 / 0.2E1 + t6575 / 0.2E1) /
     # 0.6E1 - t1327 * (((t8477 - t5154) * t183 - t18616) * t183 / 0.2E1
     # + (t18616 - (t5163 - t8782) * t183) * t183 / 0.2E1) / 0.6E1 + (t1
     #8638 * t834 - t18650 * t836) * t183 - t1327 * (((t8502 - t5200) * 
     #t183 - t18657) * t183 / 0.2E1 + (t18657 - (t5210 - t8807) * t183) 
     #* t183 / 0.2E1) / 0.6E1 - t1701 * (((t4483 - t4904) * t94 - t18671
     #) * t94 / 0.2E1 + (t18671 - (t5129 - t8042) * t94) * t94 / 0.2E1) 
     #/ 0.6E1 - t1701 * (((t4508 - t4911) * t94 - t18685) * t94 / 0.2E1 
     #+ (t18685 - (t5143 - t8056) * t94) * t94 / 0.2E1) / 0.6E1 + (t1870
     #7 * t458 - t18719 * t782) * t94 + t5211 - t1228 * (((t18736 - t524
     #7) * t236 - t6721) * t236 / 0.2E1 + t6725 / 0.2E1) / 0.6E1 + t5130
        t18748 = dt * (t18596 + t18745) * t774
        t18751 = ut(i,j,t18400,n)
        t18753 = (t18751 - t2210) * t236
        t18757 = ((t18753 - t2571) * t236 - t6863) * t236
        t18764 = dz * (t2571 / 0.2E1 + t9789 - t1228 * (t18757 / 0.2E1 +
     # t6867 / 0.2E1) / 0.6E1) / 0.2E1
        t18784 = (t10007 - t10014) * t183
        t18800 = t4670 * t6724
        t18812 = ut(i,t180,t18400,n)
        t18814 = (t18812 - t6946) * t236
        t17808 = ((t18753 / 0.2E1 - t961 / 0.2E1) * t236 - t2574) * t236
        t18828 = t822 * t17808
        t18831 = ut(i,t185,t18400,n)
        t18833 = (t18831 - t6967) * t236
        t18873 = t10015 + t9990 + t9840 + t9858 + t9983 + t9976 - t1701 
     #* ((t4872 * ((t2209 / 0.2E1 - t7059 / 0.2E1) * t94 - (t2212 / 0.2E
     #1 - t10956 / 0.2E1) * t94) * t94 - t7232) * t236 / 0.2E1 + t7237 /
     # 0.2E1) / 0.6E1 - t1327 * (((t15656 - t10007) * t183 - t18784) * t
     #183 / 0.2E1 + (t18784 - (t10014 - t17865) * t183) * t183 / 0.2E1) 
     #/ 0.6E1 + t1177 + t1199 - t1701 * ((t4823 * t13488 - t18800) * t18
     #3 / 0.2E1 + (t18800 - t4832 * t15744) * t183 / 0.2E1) / 0.6E1 - t1
     #228 * ((t3806 * ((t18814 / 0.2E1 - t1141 / 0.2E1) * t236 - t6951) 
     #* t236 - t18828) * t183 / 0.2E1 + (t18828 - t4056 * ((t18833 / 0.2
     #E1 - t1156 / 0.2E1) * t236 - t6972) * t236) * t183 / 0.2E1) / 0.6E
     #1 + t10017 - t1327 * ((t5177 * t18188 - t5186 * t18192) * t183 + (
     #(t15650 - t9999) * t183 - (t9999 - t17859) * t183) * t183) / 0.24E
     #2 - t1701 * ((t4893 * t18007 - t5111 * t18011) * t94 + ((t9828 - t
     #9971) * t94 - (t9971 - t13263) * t94) * t94) / 0.24E2
        t18885 = (t9839 - t9975) * t94
        t18901 = t4670 * t6538
        t18913 = ut(t5,j,t18400,n)
        t18916 = ut(t96,j,t18400,n)
        t18924 = (t17688 * ((t18913 - t18751) * t94 / 0.2E1 + (t18751 - 
     #t18916) * t94 / 0.2E1) - t7063) * t236
        t18958 = (t17466 * ((t18812 - t18751) * t183 / 0.2E1 + (t18751 -
     # t18831) * t183 / 0.2E1) - t7126) * t236
        t18973 = (t9857 - t9982) * t94
        t18985 = (t18913 - t2207) * t236
        t18995 = t771 * t17808
        t18999 = (t18916 - t7057) * t236
        t19018 = (t18441 * t18753 - t6876) * t236
        t19029 = (t9989 - t9994) * t183
        t19040 = (t18638 * t1188 - t18650 * t1190) * t183 + (t18707 * t1
     #033 - t18719 * t1168) * t94 - t1701 * (((t9834 - t9839) * t94 - t1
     #8885) * t94 / 0.2E1 + (t18885 - (t9975 - t13267) * t94) * t94 / 0.
     #2E1) / 0.6E1 - t1327 * ((t4153 * t2240 - t18901) * t94 / 0.2E1 + (
     #t18901 - t4804 * t10341) * t94 / 0.2E1) / 0.6E1 + t10016 - t1228 *
     # (((t18924 - t7065) * t236 - t7067) * t236 / 0.2E1 + t7071 / 0.2E1
     #) / 0.6E1 - t1327 * ((t4882 * ((t14213 / 0.2E1 - t7122 / 0.2E1) * 
     #t183 - (t7120 / 0.2E1 - t16369 / 0.2E1) * t183) * t183 - t7004) * 
     #t236 / 0.2E1 + t7009 / 0.2E1) / 0.6E1 - t1228 * (((t18958 - t7128)
     # * t236 - t7130) * t236 / 0.2E1 + t7134 / 0.2E1) / 0.6E1 + (t18562
     # * t2571 - t7153) * t236 - t1701 * (((t9850 - t9857) * t94 - t1897
     #3) * t94 / 0.2E1 + (t18973 - (t9982 - t13274) * t94) * t94 / 0.2E1
     #) / 0.6E1 + t10008 + t9995 - t1228 * ((t452 * ((t18985 / 0.2E1 - t
     #948 / 0.2E1) * t236 - t2353) * t236 - t18995) * t94 / 0.2E1 + (t18
     #995 - t4813 * ((t18999 / 0.2E1 - t1102 / 0.2E1) * t236 - t7174) * 
     #t236) * t94 / 0.2E1) / 0.6E1 - t1228 * ((t5271 * t18757 - t6868) *
     # t236 + ((t19018 - t6878) * t236 - t6880) * t236) / 0.24E2 - t1327
     # * (((t15646 - t9989) * t183 - t19029) * t183 / 0.2E1 + (t19029 - 
     #(t9994 - t17855) * t183) * t183 / 0.2E1) / 0.6E1
        t19043 = t161 * (t18873 + t19040) * t774
        t19046 = dt * dz
        t19047 = t1230 ** 2
        t19048 = t1243 ** 2
        t19049 = t1241 ** 2
        t19051 = t1252 * (t19047 + t19048 + t19049)
        t19052 = t5212 ** 2
        t19053 = t5225 ** 2
        t19054 = t5223 ** 2
        t19056 = t5234 * (t19052 + t19053 + t19054)
        t19059 = t4 * (t19051 / 0.2E1 + t19056 / 0.2E1)
        t19060 = t19059 * t1264
        t19061 = t8125 ** 2
        t19062 = t8138 ** 2
        t19063 = t8136 ** 2
        t19065 = t8147 * (t19061 + t19062 + t19063)
        t19068 = t4 * (t19056 / 0.2E1 + t19065 / 0.2E1)
        t19069 = t19068 * t5241
        t18102 = t1253 * (t1230 * t1239 + t1243 * t1231 + t1241 * t1235)
        t19077 = t18102 * t1744
        t18108 = t5235 * (t5212 * t5221 + t5225 * t5213 + t5223 * t5217)
        t19083 = t18108 * t5258
        t19086 = (t19077 - t19083) * t94 / 0.2E1
        t18116 = t8148 * (t8125 * t8134 + t8138 * t8126 + t8136 * t8130)
        t19092 = t18116 * t8171
        t19095 = (t19083 - t19092) * t94 / 0.2E1
        t19097 = t18454 / 0.2E1 + t1581 / 0.2E1
        t19099 = t1155 * t19097
        t19101 = t18403 / 0.2E1 + t1682 / 0.2E1
        t19103 = t4872 * t19101
        t19106 = (t19099 - t19103) * t94 / 0.2E1
        t19108 = t18473 / 0.2E1 + t5137 / 0.2E1
        t19110 = t7596 * t19108
        t19113 = (t19103 - t19110) * t94 / 0.2E1
        t18133 = t8527 * (t8504 * t8513 + t8517 * t8505 + t8515 * t8509)
        t19119 = t18133 * t8535
        t19121 = t18108 * t5243
        t19124 = (t19119 - t19121) * t183 / 0.2E1
        t18143 = t8832 * (t8809 * t8818 + t8822 * t8810 + t8820 * t8814)
        t19130 = t18143 * t8840
        t19133 = (t19121 - t19130) * t183 / 0.2E1
        t19134 = t8513 ** 2
        t19135 = t8505 ** 2
        t19136 = t8509 ** 2
        t19138 = t8526 * (t19134 + t19135 + t19136)
        t19139 = t5221 ** 2
        t19140 = t5213 ** 2
        t19141 = t5217 ** 2
        t19143 = t5234 * (t19139 + t19140 + t19141)
        t19146 = t4 * (t19138 / 0.2E1 + t19143 / 0.2E1)
        t19147 = t19146 * t5254
        t19148 = t8818 ** 2
        t19149 = t8810 ** 2
        t19150 = t8814 ** 2
        t19152 = t8831 * (t19148 + t19149 + t19150)
        t19155 = t4 * (t19143 / 0.2E1 + t19152 / 0.2E1)
        t19156 = t19155 * t5256
        t19160 = t18502 / 0.2E1 + t5192 / 0.2E1
        t19162 = t7949 * t19160
        t19164 = t4882 * t19101
        t19167 = (t19162 - t19164) * t183 / 0.2E1
        t19169 = t18517 / 0.2E1 + t5204 / 0.2E1
        t19171 = t8249 * t19169
        t19174 = (t19164 - t19171) * t183 / 0.2E1
        t19177 = (t19060 - t19069) * t94 + t19086 + t19095 + t19106 + t1
     #9113 + t19124 + t19133 + (t19147 - t19156) * t183 + t19167 + t1917
     #4 + t18736 / 0.2E1 + t5248 + t18545 / 0.2E1 + t5263 + t18444
        t19178 = t19177 * t5233
        t19180 = (t19178 - t5276) * t236
        t19182 = t19180 / 0.2E1 + t5278 / 0.2E1
        t19183 = t19046 * t19182
        t19191 = t1228 * (t6863 - dz * (t18757 - t6867) / 0.12E2) / 0.12
     #E2
        t19199 = t4670 * t9054
        t19208 = t4577 ** 2
        t19209 = t4590 ** 2
        t19210 = t4588 ** 2
        t19228 = u(t64,j,t18400,n)
        t19245 = t18102 * t1266
        t19258 = t5634 ** 2
        t19259 = t5626 ** 2
        t19260 = t5630 ** 2
        t19263 = t1239 ** 2
        t19264 = t1231 ** 2
        t19265 = t1235 ** 2
        t19267 = t1252 * (t19263 + t19264 + t19265)
        t19272 = t6003 ** 2
        t19273 = t5995 ** 2
        t19274 = t5999 ** 2
        t19283 = u(t5,t180,t18400,n)
        t19287 = (t19283 - t1564) * t236 / 0.2E1 + t1566 / 0.2E1
        t19291 = t1653 * t19097
        t19295 = u(t5,t185,t18400,n)
        t19299 = (t19295 - t1596) * t236 / 0.2E1 + t1598 / 0.2E1
        t19305 = rx(t5,j,t18400,0,0)
        t19306 = rx(t5,j,t18400,1,1)
        t19308 = rx(t5,j,t18400,2,2)
        t19310 = rx(t5,j,t18400,1,2)
        t19312 = rx(t5,j,t18400,2,1)
        t19314 = rx(t5,j,t18400,1,0)
        t19316 = rx(t5,j,t18400,0,2)
        t19318 = rx(t5,j,t18400,0,1)
        t19321 = rx(t5,j,t18400,2,0)
        t19327 = 0.1E1 / (t19305 * t19306 * t19308 - t19305 * t19310 * t
     #19312 + t19314 * t19312 * t19316 - t19314 * t19318 * t19308 + t193
     #21 * t19318 * t19310 - t19321 * t19306 * t19316)
        t19328 = t4 * t19327
        t19357 = t19321 ** 2
        t19358 = t19312 ** 2
        t19359 = t19308 ** 2
        t18249 = t5648 * (t5625 * t5634 + t5638 * t5626 + t5636 * t5630)
        t18254 = t6017 * (t5994 * t6003 + t6007 * t5995 + t6005 * t5999)
        t19368 = (t4 * (t4599 * (t19208 + t19209 + t19210) / 0.2E1 + t19
     #051 / 0.2E1) * t1261 - t19060) * t94 + (t4600 * (t4577 * t4586 + t
     #4590 * t4578 + t4588 * t4582) * t4623 - t19077) * t94 / 0.2E1 + t1
     #9086 + (t4373 * ((t19228 - t1258) * t236 / 0.2E1 + t1663 / 0.2E1) 
     #- t19099) * t94 / 0.2E1 + t19106 + (t18249 * t5658 - t19245) * t18
     #3 / 0.2E1 + (t19245 - t18254 * t6027) * t183 / 0.2E1 + (t4 * (t564
     #7 * (t19258 + t19259 + t19260) / 0.2E1 + t19267 / 0.2E1) * t1740 -
     # t4 * (t19267 / 0.2E1 + t6016 * (t19272 + t19273 + t19274) / 0.2E1
     #) * t1742) * t183 + (t5297 * t19287 - t19291) * t183 / 0.2E1 + (t1
     #9291 - t5691 * t19299) * t183 / 0.2E1 + (t19328 * (t19305 * t19321
     # + t19318 * t19312 + t19316 * t19308) * ((t19228 - t18452) * t94 /
     # 0.2E1 + t18728 / 0.2E1) - t1268) * t236 / 0.2E1 + t4974 + (t19328
     # * (t19314 * t19321 + t19306 * t19312 + t19310 * t19308) * ((t1928
     #3 - t18452) * t183 / 0.2E1 + (t18452 - t19295) * t183 / 0.2E1) - t
     #1746) * t236 / 0.2E1 + t4975 + (t4 * (t19327 * (t19357 + t19358 + 
     #t19359) / 0.2E1 + t1795 / 0.2E1) * t18454 - t1799) * t236
        t19369 = t19368 * t1251
        t19377 = t771 * t19182
        t19381 = t11855 ** 2
        t19382 = t11868 ** 2
        t19383 = t11866 ** 2
        t19401 = u(t6458,j,t18400,n)
        t19418 = t18116 * t8156
        t19431 = t12243 ** 2
        t19432 = t12235 ** 2
        t19433 = t12239 ** 2
        t19436 = t8134 ** 2
        t19437 = t8126 ** 2
        t19438 = t8130 ** 2
        t19440 = t8147 * (t19436 + t19437 + t19438)
        t19445 = t12548 ** 2
        t19446 = t12540 ** 2
        t19447 = t12544 ** 2
        t19456 = u(t96,t180,t18400,n)
        t19460 = (t19456 - t8103) * t236 / 0.2E1 + t8105 / 0.2E1
        t19464 = t7609 * t19108
        t19468 = u(t96,t185,t18400,n)
        t19472 = (t19468 - t8115) * t236 / 0.2E1 + t8117 / 0.2E1
        t19478 = rx(t96,j,t18400,0,0)
        t19479 = rx(t96,j,t18400,1,1)
        t19481 = rx(t96,j,t18400,2,2)
        t19483 = rx(t96,j,t18400,1,2)
        t19485 = rx(t96,j,t18400,2,1)
        t19487 = rx(t96,j,t18400,1,0)
        t19489 = rx(t96,j,t18400,0,2)
        t19491 = rx(t96,j,t18400,0,1)
        t19494 = rx(t96,j,t18400,2,0)
        t19500 = 0.1E1 / (t19478 * t19479 * t19481 - t19478 * t19483 * t
     #19485 + t19487 * t19485 * t19489 - t19487 * t19491 * t19481 + t194
     #94 * t19491 * t19483 - t19494 * t19479 * t19489)
        t19501 = t4 * t19500
        t19530 = t19494 ** 2
        t19531 = t19485 ** 2
        t19532 = t19481 ** 2
        t18398 = t12257 * (t12234 * t12243 + t12247 * t12235 + t12245 * 
     #t12239)
        t18406 = t12562 * (t12539 * t12548 + t12552 * t12540 + t12544 * 
     #t12550)
        t19541 = (t19069 - t4 * (t19065 / 0.2E1 + t11877 * (t19381 + t19
     #382 + t19383) / 0.2E1) * t8154) * t94 + t19095 + (t19092 - t11878 
     #* (t11855 * t11864 + t11868 * t11856 + t11866 * t11860) * t11901) 
     #* t94 / 0.2E1 + t19113 + (t19110 - t11219 * ((t19401 - t8048) * t2
     #36 / 0.2E1 + t8050 / 0.2E1)) * t94 / 0.2E1 + (t18398 * t12265 - t1
     #9418) * t183 / 0.2E1 + (t19418 - t18406 * t12570) * t183 / 0.2E1 +
     # (t4 * (t12256 * (t19431 + t19432 + t19433) / 0.2E1 + t19440 / 0.2
     #E1) * t8167 - t4 * (t19440 / 0.2E1 + t12561 * (t19445 + t19446 + t
     #19447) / 0.2E1) * t8169) * t183 + (t11575 * t19460 - t19464) * t18
     #3 / 0.2E1 + (t19464 - t11870 * t19472) * t183 / 0.2E1 + (t19501 * 
     #(t19478 * t19494 + t19491 * t19485 + t19489 * t19481) * (t18730 / 
     #0.2E1 + (t18471 - t19401) * t94 / 0.2E1) - t8158) * t236 / 0.2E1 +
     # t8161 + (t19501 * (t19494 * t19487 + t19479 * t19485 + t19483 * t
     #19481) * ((t19456 - t18471) * t183 / 0.2E1 + (t18471 - t19468) * t
     #183 / 0.2E1) - t8173) * t236 / 0.2E1 + t8176 + (t4 * (t19500 * (t1
     #9530 + t19531 + t19532) / 0.2E1 + t8181 / 0.2E1) * t18473 - t8185)
     # * t236
        t19542 = t19541 * t8146
        t19555 = t4670 * t9032
        t19568 = t5625 ** 2
        t19569 = t5638 ** 2
        t19570 = t5636 ** 2
        t19573 = t8504 ** 2
        t19574 = t8517 ** 2
        t19575 = t8515 ** 2
        t19577 = t8526 * (t19573 + t19574 + t19575)
        t19582 = t12234 ** 2
        t19583 = t12247 ** 2
        t19584 = t12245 ** 2
        t19596 = t18133 * t8548
        t19608 = t7938 * t19160
        t19626 = t15060 ** 2
        t19627 = t15052 ** 2
        t19628 = t15056 ** 2
        t19637 = u(i,t1328,t18400,n)
        t19647 = rx(i,t180,t18400,0,0)
        t19648 = rx(i,t180,t18400,1,1)
        t19650 = rx(i,t180,t18400,2,2)
        t19652 = rx(i,t180,t18400,1,2)
        t19654 = rx(i,t180,t18400,2,1)
        t19656 = rx(i,t180,t18400,1,0)
        t19658 = rx(i,t180,t18400,0,2)
        t19660 = rx(i,t180,t18400,0,1)
        t19663 = rx(i,t180,t18400,2,0)
        t19669 = 0.1E1 / (t19647 * t19648 * t19650 - t19647 * t19652 * t
     #19654 + t19656 * t19654 * t19658 - t19656 * t19660 * t19650 + t196
     #63 * t19660 * t19652 - t19663 * t19648 * t19658)
        t19670 = t4 * t19669
        t19699 = t19663 ** 2
        t19700 = t19654 ** 2
        t19701 = t19650 ** 2
        t19710 = (t4 * (t5647 * (t19568 + t19569 + t19570) / 0.2E1 + t19
     #577 / 0.2E1) * t5656 - t4 * (t19577 / 0.2E1 + t12256 * (t19582 + t
     #19583 + t19584) / 0.2E1) * t8533) * t94 + (t18249 * t5671 - t19596
     #) * t94 / 0.2E1 + (t19596 - t18398 * t12278) * t94 / 0.2E1 + (t528
     #9 * t19287 - t19608) * t94 / 0.2E1 + (t19608 - t11568 * t19460) * 
     #t94 / 0.2E1 + (t15074 * (t15051 * t15060 + t15064 * t15052 + t1506
     #2 * t15056) * t15084 - t19119) * t183 / 0.2E1 + t19124 + (t4 * (t1
     #5073 * (t19626 + t19627 + t19628) / 0.2E1 + t19138 / 0.2E1) * t854
     #6 - t19147) * t183 + (t14250 * ((t19637 - t8494) * t236 / 0.2E1 + 
     #t8496 / 0.2E1) - t19162) * t183 / 0.2E1 + t19167 + (t19670 * (t196
     #47 * t19663 + t19660 * t19654 + t19658 * t19650) * ((t19283 - t185
     #00) * t94 / 0.2E1 + (t18500 - t19456) * t94 / 0.2E1) - t8537) * t2
     #36 / 0.2E1 + t8540 + (t19670 * (t19656 * t19663 + t19648 * t19654 
     #+ t19652 * t19650) * ((t19637 - t18500) * t183 / 0.2E1 + t18537 / 
     #0.2E1) - t8550) * t236 / 0.2E1 + t8553 + (t4 * (t19669 * (t19699 +
     # t19700 + t19701) / 0.2E1 + t8558 / 0.2E1) * t18502 - t8562) * t23
     #6
        t19711 = t19710 * t8525
        t19719 = t822 * t19182
        t19723 = t5994 ** 2
        t19724 = t6007 ** 2
        t19725 = t6005 ** 2
        t19728 = t8809 ** 2
        t19729 = t8822 ** 2
        t19730 = t8820 ** 2
        t19732 = t8831 * (t19728 + t19729 + t19730)
        t19737 = t12539 ** 2
        t19738 = t12552 ** 2
        t19739 = t12550 ** 2
        t19751 = t18143 * t8853
        t19763 = t8235 * t19169
        t19781 = t17326 ** 2
        t19782 = t17318 ** 2
        t19783 = t17322 ** 2
        t19792 = u(i,t1335,t18400,n)
        t19802 = rx(i,t185,t18400,0,0)
        t19803 = rx(i,t185,t18400,1,1)
        t19805 = rx(i,t185,t18400,2,2)
        t19807 = rx(i,t185,t18400,1,2)
        t19809 = rx(i,t185,t18400,2,1)
        t19811 = rx(i,t185,t18400,1,0)
        t19813 = rx(i,t185,t18400,0,2)
        t19815 = rx(i,t185,t18400,0,1)
        t19818 = rx(i,t185,t18400,2,0)
        t19824 = 0.1E1 / (t19802 * t19803 * t19805 - t19802 * t19807 * t
     #19809 + t19811 * t19809 * t19813 - t19811 * t19815 * t19805 + t198
     #18 * t19815 * t19807 - t19818 * t19803 * t19813)
        t19825 = t4 * t19824
        t19854 = t19818 ** 2
        t19855 = t19809 ** 2
        t19856 = t19805 ** 2
        t19865 = (t4 * (t6016 * (t19723 + t19724 + t19725) / 0.2E1 + t19
     #732 / 0.2E1) * t6025 - t4 * (t19732 / 0.2E1 + t12561 * (t19737 + t
     #19738 + t19739) / 0.2E1) * t8838) * t94 + (t18254 * t6040 - t19751
     #) * t94 / 0.2E1 + (t19751 - t18406 * t12583) * t94 / 0.2E1 + (t568
     #4 * t19299 - t19763) * t94 / 0.2E1 + (t19763 - t11861 * t19472) * 
     #t94 / 0.2E1 + t19133 + (t19130 - t17340 * (t17317 * t17326 + t1733
     #0 * t17318 + t17328 * t17322) * t17350) * t183 / 0.2E1 + (t19156 -
     # t4 * (t19152 / 0.2E1 + t17339 * (t19781 + t19782 + t19783) / 0.2E
     #1) * t8851) * t183 + t19174 + (t19171 - t16366 * ((t19792 - t8799)
     # * t236 / 0.2E1 + t8801 / 0.2E1)) * t183 / 0.2E1 + (t19825 * (t198
     #02 * t19818 + t19815 * t19809 + t19813 * t19805) * ((t19295 - t185
     #15) * t94 / 0.2E1 + (t18515 - t19468) * t94 / 0.2E1) - t8842) * t2
     #36 / 0.2E1 + t8845 + (t19825 * (t19811 * t19818 + t19803 * t19809 
     #+ t19807 * t19805) * (t18539 / 0.2E1 + (t18515 - t19792) * t183 / 
     #0.2E1) - t8855) * t236 / 0.2E1 + t8858 + (t4 * (t19824 * (t19854 +
     # t19855 + t19856) / 0.2E1 + t8863 / 0.2E1) * t18517 - t8867) * t23
     #6
        t19866 = t19865 * t8830
        t19901 = (t4893 * t6251 - t5111 * t9030) * t94 + (t4153 * t6277 
     #- t19199) * t94 / 0.2E1 + (t19199 - t4804 * t12784) * t94 / 0.2E1 
     #+ (t452 * ((t19369 - t4977) * t236 / 0.2E1 + t4979 / 0.2E1) - t193
     #77) * t94 / 0.2E1 + (t19377 - t4813 * ((t19542 - t8189) * t236 / 0
     #.2E1 + t8191 / 0.2E1)) * t94 / 0.2E1 + (t4823 * t15310 - t19555) *
     # t183 / 0.2E1 + (t19555 - t4832 * t17576) * t183 / 0.2E1 + (t5177 
     #* t9050 - t5186 * t9052) * t183 + (t3806 * ((t19711 - t8566) * t23
     #6 / 0.2E1 + t8568 / 0.2E1) - t19719) * t183 / 0.2E1 + (t19719 - t4
     #056 * ((t19866 - t8871) * t236 / 0.2E1 + t8873 / 0.2E1)) * t183 / 
     #0.2E1 + (t4872 * ((t19369 - t19178) * t94 / 0.2E1 + (t19178 - t195
     #42) * t94 / 0.2E1) - t9034) * t236 / 0.2E1 + t9039 + (t4882 * ((t1
     #9711 - t19178) * t183 / 0.2E1 + (t19178 - t19866) * t183 / 0.2E1) 
     #- t9056) * t236 / 0.2E1 + t9061 + (t5271 * t19180 - t9073) * t236
        t19903 = t895 * t19901 * t774
        t19906 = t161 * dz
        t19914 = t18108 * t7124
        t19928 = t18753 / 0.2E1 + t2571 / 0.2E1
        t19930 = t4872 * t19928
        t19944 = t18108 * t7061
        t19962 = t4882 * t19928
        t19975 = (t19059 * t2212 - t19068 * t7059) * t94 + (t18102 * t23
     #92 - t19914) * t94 / 0.2E1 + (t19914 - t18116 * t10792) * t94 / 0.
     #2E1 + (t1155 * (t18985 / 0.2E1 + t2350 / 0.2E1) - t19930) * t94 / 
     #0.2E1 + (t19930 - t7596 * (t18999 / 0.2E1 + t7171 / 0.2E1)) * t94 
     #/ 0.2E1 + (t18133 * t14123 - t19944) * t183 / 0.2E1 + (t19944 - t1
     #8143 * t16607) * t183 / 0.2E1 + (t19146 * t7120 - t19155 * t7122) 
     #* t183 + (t7949 * (t18814 / 0.2E1 + t6948 / 0.2E1) - t19962) * t18
     #3 / 0.2E1 + (t19962 - t8249 * (t18833 / 0.2E1 + t6969 / 0.2E1)) * 
     #t183 / 0.2E1 + t18924 / 0.2E1 + t10016 + t18958 / 0.2E1 + t10017 +
     # t19018
        t19981 = t19906 * ((t19975 * t5233 - t10019) * t236 / 0.2E1 + t1
     #0021 / 0.2E1)
        t19985 = t19046 * (t19180 - t5278)
        t19990 = dz * (t9789 + t9790 - t9791) / 0.2E1
        t19991 = t19046 * t5478
        t19993 = t1227 * t19991 / 0.2E1
        t19999 = t1228 * (t6865 - dz * (t6867 - t6872) / 0.12E2) / 0.12E
     #2
        t20002 = t19906 * (t10021 / 0.2E1 + t10074 / 0.2E1)
        t20004 = t2079 * t20002 / 0.4E1
        t20006 = t19046 * (t5278 - t5476)
        t20008 = t1227 * t20006 / 0.12E2
        t20009 = t959 + t1227 * t18748 - t18764 + t2079 * t19043 / 0.2E1
     # - t1227 * t19183 / 0.2E1 + t19191 + t2923 * t19903 / 0.6E1 - t207
     #9 * t19981 / 0.4E1 + t1227 * t19985 / 0.12E2 - t2 - t6846 - t19990
     # - t7310 - t19993 - t19999 - t9081 - t20004 - t20008
        t20013 = 0.8E1 * t866
        t20014 = 0.8E1 * t867
        t20015 = 0.8E1 * t868
        t20025 = sqrt(0.8E1 * t861 + 0.8E1 * t862 + 0.8E1 * t863 + t2001
     #3 + t20014 + t20015 - 0.2E1 * dz * ((t5264 + t5265 + t5266 - t861 
     #- t862 - t863) * t236 / 0.2E1 - (t866 + t867 + t868 - t875 - t876 
     #- t877) * t236 / 0.2E1))
        t20026 = 0.1E1 / t20025
        t20031 = t6810 * t9213 * t18350
        t20034 = t873 * t9216 * t18355 / 0.2E1
        t20037 = t873 * t9220 * t18360 / 0.6E1
        t20039 = t9213 * t18363 / 0.24E2
        t20052 = t9226 * t19991 / 0.2E1
        t20054 = t9228 * t20002 / 0.4E1
        t20056 = t9226 * t20006 / 0.12E2
        t20057 = t959 + t9226 * t18748 - t18764 + t9228 * t19043 / 0.2E1
     # - t9226 * t19183 / 0.2E1 + t19191 + t9233 * t19903 / 0.6E1 - t922
     #8 * t19981 / 0.4E1 + t9226 * t19985 / 0.12E2 - t2 - t9240 - t19990
     # - t9242 - t20052 - t19999 - t9246 - t20054 - t20056
        t20060 = 0.2E1 * t18366 * t20057 * t20026
        t20062 = (t6810 * t136 * t18350 + t873 * t159 * t18355 / 0.2E1 +
     # t873 * t893 * t18360 / 0.6E1 - t136 * t18363 / 0.24E2 + 0.2E1 * t
     #18366 * t20009 * t20026 - t20031 - t20034 - t20037 + t20039 - t200
     #60) * t133
        t20068 = t6810 * (t269 - dz * t6542 / 0.24E2)
        t20070 = dz * t6552 / 0.24E2
        t20086 = t4 * (t9743 + t17991 / 0.2E1 - dz * ((t17986 - t9742) *
     # t236 / 0.2E1 - (t17991 - t5432 * t5437) * t236 / 0.2E1) / 0.8E1)
        t20097 = (t2231 - t7074) * t94
        t20114 = t13461 + t13462 - t13463 + t1046 / 0.4E1 + t1179 / 0.4E
     #1 - t18051 / 0.12E2 - dz * ((t18032 + t18033 - t18034 - t2062 - t6
     #847 + t6858) * t236 / 0.2E1 - (t18037 + t18038 - t18052 - t2231 / 
     #0.2E1 - t7074 / 0.2E1 + t1701 * (((t2228 - t2231) * t94 - t20097) 
     #* t94 / 0.2E1 + (t20097 - (t7074 - t10971) * t94) * t94 / 0.2E1) /
     # 0.6E1) * t236 / 0.2E1) / 0.8E1
        t20119 = t4 * (t9742 / 0.2E1 + t17991 / 0.2E1)
        t20121 = t2910 / 0.4E1 + t7444 / 0.4E1 + t6264 / 0.4E1 + t9041 /
     # 0.4E1
        t20130 = t6406 / 0.4E1 + t9181 / 0.4E1 + (t9966 - t10072) * t94 
     #/ 0.4E1 + (t10072 - t13364) * t94 / 0.4E1
        t20136 = dz * (t1176 / 0.2E1 - t7080 / 0.2E1)
        t20140 = t20086 * t9213 * t20114
        t20143 = t20119 * t9658 * t20121 / 0.2E1
        t20146 = t20119 * t9662 * t20130 / 0.6E1
        t20148 = t9213 * t20136 / 0.24E2
        t20150 = (t20086 * t136 * t20114 + t20119 * t9349 * t20121 / 0.2
     #E1 + t20119 * t9355 * t20130 / 0.6E1 - t136 * t20136 / 0.24E2 - t2
     #0140 - t20143 - t20146 + t20148) * t133
        t20163 = (t1312 - t5439) * t94
        t20181 = t20086 * (t13562 + t13563 - t13567 + t499 / 0.4E1 + t82
     #1 / 0.4E1 - t18149 / 0.12E2 - dz * ((t18130 + t18131 - t18132 - t1
     #3589 - t13590 + t13591) * t236 / 0.2E1 - (t18135 + t18136 - t18150
     # - t1312 / 0.2E1 - t5439 / 0.2E1 + t1701 * (((t1309 - t1312) * t94
     # - t20163) * t94 / 0.2E1 + (t20163 - (t5439 - t8352) * t94) * t94 
     #/ 0.2E1) / 0.6E1) * t236 / 0.2E1) / 0.8E1)
        t20185 = dz * (t790 / 0.2E1 - t5445 / 0.2E1) / 0.24E2
        t20201 = t4 * (t15542 + t18172 / 0.2E1 - dz * ((t18167 - t15541)
     # * t236 / 0.2E1 - (t18172 - t5432 * t5450) * t236 / 0.2E1) / 0.8E1
     #)
        t20212 = (t7136 - t7138) * t183
        t20229 = t9291 + t9292 - t9296 + t1201 / 0.4E1 + t1203 / 0.4E1 -
     # t18232 / 0.12E2 - dz * ((t18213 + t18214 - t18215 - t9318 - t9319
     # + t9320) * t236 / 0.2E1 - (t18218 + t18219 - t18233 - t7136 / 0.2
     #E1 - t7138 / 0.2E1 + t1327 * (((t14227 - t7136) * t183 - t20212) *
     # t183 / 0.2E1 + (t20212 - (t7138 - t16383) * t183) * t183 / 0.2E1)
     # / 0.6E1) * t236 / 0.2E1) / 0.8E1
        t20234 = t4 * (t15541 / 0.2E1 + t18172 / 0.2E1)
        t20236 = t4134 / 0.4E1 + t4398 / 0.4E1 + t9063 / 0.4E1 + t9065 /
     # 0.4E1
        t20245 = t9584 / 0.4E1 + t9645 / 0.4E1 + (t15710 - t10072) * t18
     #3 / 0.4E1 + (t10072 - t17919) * t183 / 0.4E1
        t20251 = dz * (t1198 / 0.2E1 - t7144 / 0.2E1)
        t20255 = t20201 * t9213 * t20229
        t20258 = t20234 * t9658 * t20236 / 0.2E1
        t20261 = t20234 * t9662 * t20245 / 0.6E1
        t20263 = t9213 * t20251 / 0.24E2
        t20265 = (t20201 * t136 * t20229 + t20234 * t9349 * t20236 / 0.2
     #E1 + t20234 * t9355 * t20245 / 0.6E1 - t136 * t20251 / 0.24E2 - t2
     #0255 - t20258 - t20261 + t20263) * t133
        t20278 = (t5452 - t5454) * t183
        t20296 = t20201 * (t9678 + t9679 - t9683 + t851 / 0.4E1 + t853 /
     # 0.4E1 - t18330 / 0.12E2 - dz * ((t18311 + t18312 - t18313 - t9705
     # - t9706 + t9707) * t236 / 0.2E1 - (t18316 + t18317 - t18331 - t54
     #52 / 0.2E1 - t5454 / 0.2E1 + t1327 * (((t8694 - t5452) * t183 - t2
     #0278) * t183 / 0.2E1 + (t20278 - (t5454 - t8999) * t183) * t183 / 
     #0.2E1) / 0.6E1) * t236 / 0.2E1) / 0.8E1)
        t20300 = dz * (t844 / 0.2E1 - t5460 / 0.2E1) / 0.24E2
        t20307 = t964 - dz * t6871 / 0.24E2
        t20312 = t161 * t5475 * t236
        t20317 = t895 * t10073 * t236
        t20320 = dz * t6884
        t20323 = cc * t6821
        t20327 = (t5352 - t5361) * t183
        t20351 = k - 3
        t20352 = u(i,t180,t20351,n)
        t20354 = (t5388 - t20352) * t236
        t20362 = u(i,j,t20351,n)
        t20364 = (t1310 - t20362) * t236
        t19243 = (t1690 - (t272 / 0.2E1 - t20364 / 0.2E1) * t236) * t236
        t20371 = t837 * t19243
        t20374 = u(i,t185,t20351,n)
        t20376 = (t5400 - t20374) * t236
        t20391 = t4984 / 0.2E1
        t20401 = t4 * (t4685 / 0.2E1 + t20391 - dx * ((t4676 - t4685) * 
     #t94 / 0.2E1 - (t4984 - t5306) * t94 / 0.2E1) / 0.8E1)
        t20413 = t4 * (t20391 + t5306 / 0.2E1 - dx * ((t4685 - t4984) * 
     #t94 / 0.2E1 - (t5306 - t8219) * t94 / 0.2E1) / 0.8E1)
        t20420 = (t5398 - t5408) * t183
        t20431 = rx(i,j,t20351,0,0)
        t20432 = rx(i,j,t20351,1,1)
        t20434 = rx(i,j,t20351,2,2)
        t20436 = rx(i,j,t20351,1,2)
        t20438 = rx(i,j,t20351,2,1)
        t20440 = rx(i,j,t20351,1,0)
        t20442 = rx(i,j,t20351,0,2)
        t20444 = rx(i,j,t20351,0,1)
        t20447 = rx(i,j,t20351,2,0)
        t20453 = 0.1E1 / (t20431 * t20432 * t20434 - t20431 * t20436 * t
     #20438 + t20440 * t20438 * t20442 - t20440 * t20444 * t20434 + t204
     #47 * t20444 * t20436 - t20447 * t20432 * t20442)
        t20454 = t4 * t20453
        t20459 = u(t5,j,t20351,n)
        t20461 = (t20459 - t20362) * t94
        t20462 = u(t96,j,t20351,n)
        t20464 = (t20362 - t20462) * t94
        t19301 = t20454 * (t20431 * t20447 + t20444 * t20438 + t20442 * 
     #t20434)
        t20470 = (t5443 - t19301 * (t20461 / 0.2E1 + t20464 / 0.2E1)) * 
     #t236
        t20498 = (t4998 - t5327) * t94
        t20514 = t4726 * t6268
        t20529 = (t5005 - t5341) * t94
        t20545 = t4726 * t6236
        t20557 = -t1327 * (((t8625 - t5352) * t183 - t20327) * t183 / 0.
     #2E1 + (t20327 - (t5361 - t8930) * t183) * t183 / 0.2E1) / 0.6E1 - 
     #t1327 * ((t5375 * t18323 - t5384 * t18327) * t183 + ((t8637 - t538
     #7) * t183 - (t5387 - t8942) * t183) * t183) / 0.24E2 - t1228 * ((t
     #3817 * (t6597 - (t722 / 0.2E1 - t20354 / 0.2E1) * t236) * t236 - t
     #20371) * t183 / 0.2E1 + (t20371 - t4067 * (t6612 - (t745 / 0.2E1 -
     # t20376 / 0.2E1) * t236) * t236) * t183 / 0.2E1) / 0.6E1 + (t20401
     # * t499 - t20413 * t821) * t94 - t1327 * (((t8650 - t5398) * t183 
     #- t20420) * t183 / 0.2E1 + (t20420 - (t5408 - t8955) * t183) * t18
     #3 / 0.2E1) / 0.6E1 - t1228 * (t6729 / 0.2E1 + (t6727 - (t5445 - t2
     #0470) * t236) * t236 / 0.2E1) / 0.6E1 - t1327 * (t6587 / 0.2E1 + (
     #t6585 - t5059 * ((t8694 / 0.2E1 - t5454 / 0.2E1) * t183 - (t5452 /
     # 0.2E1 - t8999 / 0.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 + 
     #t5006 - t1701 * (((t4721 - t4998) * t94 - t20498) * t94 / 0.2E1 + 
     #(t20498 - (t5327 - t8240) * t94) * t94 / 0.2E1) / 0.6E1 - t1701 * 
     #((t4962 * t13262 - t20514) * t183 / 0.2E1 + (t20514 - t4971 * t154
     #09) * t183 / 0.2E1) / 0.6E1 - t1701 * (((t4746 - t5005) * t94 - t2
     #0529) * t94 / 0.2E1 + (t20529 - (t5341 - t8254) * t94) * t94 / 0.2
     #E1) / 0.6E1 - t1327 * ((t4468 * t1501 - t20545) * t94 / 0.2E1 + (t
     #20545 - t4931 * t9783) * t94 / 0.2E1) / 0.6E1 + t5353 + t5409 + t5
     #446
        t20565 = t20447 ** 2
        t20566 = t20438 ** 2
        t20567 = t20434 ** 2
        t20569 = t20453 * (t20565 + t20566 + t20567)
        t20572 = t4 * (t5466 / 0.2E1 + t20569 / 0.2E1)
        t20575 = (t5470 - t20572 * t20364) * t236
        t20584 = (t1307 - t20459) * t236
        t20594 = t809 * t19243
        t20598 = (t5333 - t20462) * t236
        t20617 = (t20352 - t20362) * t183
        t20619 = (t20362 - t20374) * t183
        t19496 = t20454 * (t20440 * t20447 + t20432 * t20438 + t20436 * 
     #t20434)
        t20625 = (t5458 - t19496 * (t20617 / 0.2E1 + t20619 / 0.2E1)) * 
     #t236
        t20635 = t5372 / 0.2E1
        t20645 = t4 * (t5367 / 0.2E1 + t20635 - dy * ((t8631 - t5367) * 
     #t183 / 0.2E1 - (t5372 - t5381) * t183 / 0.2E1) / 0.8E1)
        t20657 = t4 * (t20635 + t5381 / 0.2E1 - dy * ((t5367 - t5372) * 
     #t183 / 0.2E1 - (t5381 - t8936) * t183 / 0.2E1) / 0.8E1)
        t20669 = t4 * (t6812 + t5466 / 0.2E1 - dz * (t6804 / 0.2E1 - (t5
     #466 - t20569) * t236 / 0.2E1) / 0.8E1)
        t20702 = t5461 - t1228 * ((t6549 - t5469 * (t6546 - (t1687 - t20
     #364) * t236) * t236) * t236 + (t6555 - (t5472 - t20575) * t236) * 
     #t236) / 0.24E2 - t1228 * ((t492 * (t1589 - (t255 / 0.2E1 - t20584 
     #/ 0.2E1) * t236) * t236 - t20594) * t94 / 0.2E1 + (t20594 - t4953 
     #* (t6665 - (t602 / 0.2E1 - t20598 / 0.2E1) * t236) * t236) * t94 /
     # 0.2E1) / 0.6E1 + t860 + t828 - t1228 * (t6794 / 0.2E1 + (t6792 - 
     #(t5460 - t20625) * t236) * t236 / 0.2E1) / 0.6E1 + (t20645 * t851 
     #- t20657 * t853) * t183 + (t6823 - t20669 * t1687) * t236 + t5328 
     #+ t5342 + t5362 + t5399 + t4999 - t1701 * (t6655 / 0.2E1 + (t6653 
     #- t5046 * ((t1309 / 0.2E1 - t5439 / 0.2E1) * t94 - (t1312 / 0.2E1 
     #- t8352 / 0.2E1) * t94) * t94) * t236 / 0.2E1) / 0.6E1 - t1701 * (
     #(t4987 * t18142 - t5309 * t18146) * t94 + ((t4990 - t5312) * t94 -
     # (t5312 - t8225) * t94) * t94) / 0.24E2
        t20705 = dt * (t20557 + t20702) * t813
        t20708 = ut(i,j,t20351,n)
        t20710 = (t2229 - t20708) * t236
        t20714 = (t6870 - (t2576 - t20710) * t236) * t236
        t20721 = dz * (t9790 + t2576 / 0.2E1 - t1228 * (t6872 / 0.2E1 + 
     #t20714 / 0.2E1) / 0.6E1) / 0.2E1
        t20727 = t4726 * t6732
        t20756 = ut(i,t180,t20351,n)
        t20758 = (t6952 - t20756) * t236
        t19649 = (t2579 - (t964 / 0.2E1 - t20710 / 0.2E1) * t236) * t236
        t20772 = t837 * t19649
        t20775 = ut(i,t185,t20351,n)
        t20777 = (t6973 - t20775) * t236
        t20796 = (t6881 - t20572 * t20710) * t236
        t20807 = (t10060 - t10067) * t183
        t20834 = ut(t5,j,t20351,n)
        t20837 = ut(t96,j,t20351,n)
        t20845 = (t7078 - t19301 * ((t20834 - t20708) * t94 / 0.2E1 + (t
     #20708 - t20837) * t94 / 0.2E1)) * t236
        t20854 = -t1701 * ((t4962 * t13494 - t20727) * t183 / 0.2E1 + (t
     #20727 - t4971 * t15749) * t183 / 0.2E1) / 0.6E1 - t1327 * ((t5375 
     #* t18225 - t5384 * t18229) * t183 + ((t15699 - t10052) * t183 - (t
     #10052 - t17908) * t183) * t183) / 0.24E2 + (t20645 * t1201 - t2065
     #7 * t1203) * t183 - t1228 * ((t3817 * (t6957 - (t1144 / 0.2E1 - t2
     #0758 / 0.2E1) * t236) * t236 - t20772) * t183 / 0.2E1 + (t20772 - 
     #t4067 * (t6978 - (t1159 / 0.2E1 - t20777 / 0.2E1) * t236) * t236) 
     #* t183 / 0.2E1) / 0.6E1 - t1228 * ((t6873 - t5469 * t20714) * t236
     # + (t6885 - (t6883 - t20796) * t236) * t236) / 0.24E2 + t9912 - t1
     #327 * (((t15705 - t10060) * t183 - t20807) * t183 / 0.2E1 + (t2080
     #7 - (t10067 - t17914) * t183) * t183 / 0.2E1) / 0.6E1 + t9930 - t1
     #701 * (t7248 / 0.2E1 + (t7246 - t5046 * ((t2228 / 0.2E1 - t7074 / 
     #0.2E1) * t94 - (t2231 / 0.2E1 - t10971 / 0.2E1) * t94) * t94) * t2
     #36 / 0.2E1) / 0.6E1 - t1228 * (t7084 / 0.2E1 + (t7082 - (t7080 - t
     #20845) * t236) * t236 / 0.2E1) / 0.6E1 + t10029 + t10036 + t10043 
     #+ t10048 + t10068
        t20858 = (t9911 - t10028) * t94
        t20870 = (t2226 - t20834) * t236
        t20880 = t809 * t19649
        t20884 = (t7072 - t20837) * t236
        t20905 = (t10042 - t10047) * t183
        t20938 = (t7142 - t19496 * ((t20756 - t20708) * t183 / 0.2E1 + (
     #t20708 - t20775) * t183 / 0.2E1)) * t236
        t20955 = t4726 * t6559
        t20986 = (t9929 - t10035) * t94
        t20997 = t10061 - t1701 * (((t9906 - t9911) * t94 - t20858) * t9
     #4 / 0.2E1 + (t20858 - (t10028 - t13320) * t94) * t94 / 0.2E1) / 0.
     #6E1 - t1228 * ((t492 * (t2358 - (t951 / 0.2E1 - t20870 / 0.2E1) * 
     #t236) * t236 - t20880) * t94 / 0.2E1 + (t20880 - t4953 * (t7179 - 
     #(t1105 / 0.2E1 - t20884 / 0.2E1) * t236) * t236) * t94 / 0.2E1) / 
     #0.6E1 + (t20401 * t1046 - t20413 * t1179) * t94 - t1327 * (((t1569
     #5 - t10042) * t183 - t20905) * t183 / 0.2E1 + (t20905 - (t10047 - 
     #t17904) * t183) * t183 / 0.2E1) / 0.6E1 - t1701 * ((t4987 * t18044
     # - t5309 * t18048) * t94 + ((t9900 - t10024) * t94 - (t10024 - t13
     #316) * t94) * t94) / 0.24E2 - t1228 * (t7148 / 0.2E1 + (t7146 - (t
     #7144 - t20938) * t236) * t236 / 0.2E1) / 0.6E1 + (t7154 - t20669 *
     # t2576) * t236 + t1186 + t1210 + t10069 + t10070 - t1327 * ((t4468
     # * t2244 - t20955) * t94 / 0.2E1 + (t20955 - t4931 * t10345) * t94
     # / 0.2E1) / 0.6E1 - t1327 * (t7027 / 0.2E1 + (t7025 - t5059 * ((t1
     #4227 / 0.2E1 - t7138 / 0.2E1) * t183 - (t7136 / 0.2E1 - t16383 / 0
     #.2E1) * t183) * t183) * t236 / 0.2E1) / 0.6E1 - t1701 * (((t9922 -
     # t9929) * t94 - t20986) * t94 / 0.2E1 + (t20986 - (t10035 - t13327
     #) * t94) * t94 / 0.2E1) / 0.6E1
        t21000 = t161 * (t20854 + t20997) * t813
        t21003 = t1278 ** 2
        t21004 = t1291 ** 2
        t21005 = t1289 ** 2
        t21007 = t1300 * (t21003 + t21004 + t21005)
        t21008 = t5410 ** 2
        t21009 = t5423 ** 2
        t21010 = t5421 ** 2
        t21012 = t5432 * (t21008 + t21009 + t21010)
        t21015 = t4 * (t21007 / 0.2E1 + t21012 / 0.2E1)
        t21016 = t21015 * t1312
        t21017 = t8323 ** 2
        t21018 = t8336 ** 2
        t21019 = t8334 ** 2
        t21021 = t8345 * (t21017 + t21018 + t21019)
        t21024 = t4 * (t21012 / 0.2E1 + t21021 / 0.2E1)
        t21025 = t21024 * t5439
        t19941 = t1301 * (t1278 * t1287 + t1291 * t1279 + t1289 * t1283)
        t21033 = t19941 * t1764
        t19946 = t5433 * (t5410 * t5419 + t5423 * t5411 + t5421 * t5415)
        t21039 = t19946 * t5456
        t21042 = (t21033 - t21039) * t94 / 0.2E1
        t19952 = t8346 * (t8323 * t8332 + t8336 * t8324 + t8334 * t8328)
        t21048 = t19952 * t8369
        t21051 = (t21039 - t21048) * t94 / 0.2E1
        t21053 = t1586 / 0.2E1 + t20584 / 0.2E1
        t21055 = t1202 * t21053
        t21057 = t1687 / 0.2E1 + t20364 / 0.2E1
        t21059 = t5046 * t21057
        t21062 = (t21055 - t21059) * t94 / 0.2E1
        t21064 = t5335 / 0.2E1 + t20598 / 0.2E1
        t21066 = t7777 * t21064
        t21069 = (t21059 - t21066) * t94 / 0.2E1
        t19966 = t8675 * (t8652 * t8661 + t8665 * t8653 + t8663 * t8657)
        t21075 = t19966 * t8683
        t21077 = t19946 * t5441
        t21080 = (t21075 - t21077) * t183 / 0.2E1
        t19972 = t8980 * (t8957 * t8966 + t8970 * t8958 + t8968 * t8962)
        t21086 = t19972 * t8988
        t21089 = (t21077 - t21086) * t183 / 0.2E1
        t21090 = t8661 ** 2
        t21091 = t8653 ** 2
        t21092 = t8657 ** 2
        t21094 = t8674 * (t21090 + t21091 + t21092)
        t21095 = t5419 ** 2
        t21096 = t5411 ** 2
        t21097 = t5415 ** 2
        t21099 = t5432 * (t21095 + t21096 + t21097)
        t21102 = t4 * (t21094 / 0.2E1 + t21099 / 0.2E1)
        t21103 = t21102 * t5452
        t21104 = t8966 ** 2
        t21105 = t8958 ** 2
        t21106 = t8962 ** 2
        t21108 = t8979 * (t21104 + t21105 + t21106)
        t21111 = t4 * (t21099 / 0.2E1 + t21108 / 0.2E1)
        t21112 = t21111 * t5454
        t21116 = t5390 / 0.2E1 + t20354 / 0.2E1
        t21118 = t8101 * t21116
        t21120 = t5059 * t21057
        t21123 = (t21118 - t21120) * t183 / 0.2E1
        t21125 = t5402 / 0.2E1 + t20376 / 0.2E1
        t21127 = t8394 * t21125
        t21130 = (t21120 - t21127) * t183 / 0.2E1
        t21133 = (t21016 - t21025) * t94 + t21042 + t21051 + t21062 + t2
     #1069 + t21080 + t21089 + (t21103 - t21112) * t183 + t21123 + t2113
     #0 + t5446 + t20470 / 0.2E1 + t5461 + t20625 / 0.2E1 + t20575
        t21134 = t21133 * t5431
        t21136 = (t5474 - t21134) * t236
        t21138 = t5476 / 0.2E1 + t21136 / 0.2E1
        t21139 = t19046 * t21138
        t21147 = t1228 * (t6870 - dz * (t6872 - t20714) / 0.12E2) / 0.12
     #E2
        t21155 = t4726 * t9067
        t21164 = t4815 ** 2
        t21165 = t4828 ** 2
        t21166 = t4826 ** 2
        t21184 = u(t64,j,t20351,n)
        t21201 = t19941 * t1314
        t21214 = t5814 ** 2
        t21215 = t5806 ** 2
        t21216 = t5810 ** 2
        t21219 = t1287 ** 2
        t21220 = t1279 ** 2
        t21221 = t1283 ** 2
        t21223 = t1300 * (t21219 + t21220 + t21221)
        t21228 = t6183 ** 2
        t21229 = t6175 ** 2
        t21230 = t6179 ** 2
        t21239 = u(t5,t180,t20351,n)
        t21243 = t1572 / 0.2E1 + (t1570 - t21239) * t236 / 0.2E1
        t21247 = t1665 * t21053
        t21251 = u(t5,t185,t20351,n)
        t21255 = t1604 / 0.2E1 + (t1602 - t21251) * t236 / 0.2E1
        t21261 = rx(t5,j,t20351,0,0)
        t21262 = rx(t5,j,t20351,1,1)
        t21264 = rx(t5,j,t20351,2,2)
        t21266 = rx(t5,j,t20351,1,2)
        t21268 = rx(t5,j,t20351,2,1)
        t21270 = rx(t5,j,t20351,1,0)
        t21272 = rx(t5,j,t20351,0,2)
        t21274 = rx(t5,j,t20351,0,1)
        t21277 = rx(t5,j,t20351,2,0)
        t21283 = 0.1E1 / (t21261 * t21262 * t21264 - t21261 * t21266 * t
     #21268 + t21270 * t21268 * t21272 - t21270 * t21274 * t21264 + t212
     #77 * t21274 * t21266 - t21277 * t21262 * t21272)
        t21284 = t4 * t21283
        t21313 = t21277 ** 2
        t21314 = t21268 ** 2
        t21315 = t21264 ** 2
        t20081 = t5828 * (t5805 * t5814 + t5818 * t5806 + t5816 * t5810)
        t20087 = t6197 * (t6174 * t6183 + t6187 * t6175 + t6185 * t6179)
        t21324 = (t4 * (t4837 * (t21164 + t21165 + t21166) / 0.2E1 + t21
     #007 / 0.2E1) * t1309 - t21016) * t94 + (t4838 * (t4815 * t4824 + t
     #4828 * t4816 + t4826 * t4820) * t4861 - t21033) * t94 / 0.2E1 + t2
     #1042 + (t4618 * (t1668 / 0.2E1 + (t1306 - t21184) * t236 / 0.2E1) 
     #- t21055) * t94 / 0.2E1 + t21062 + (t20081 * t5838 - t21201) * t18
     #3 / 0.2E1 + (t21201 - t20087 * t6207) * t183 / 0.2E1 + (t4 * (t582
     #7 * (t21214 + t21215 + t21216) / 0.2E1 + t21223 / 0.2E1) * t1760 -
     # t4 * (t21223 / 0.2E1 + t6196 * (t21228 + t21229 + t21230) / 0.2E1
     #) * t1762) * t183 + (t5520 * t21243 - t21247) * t183 / 0.2E1 + (t2
     #1247 - t5861 * t21255) * t183 / 0.2E1 + t5068 + (t1316 - t21284 * 
     #(t21261 * t21277 + t21274 * t21268 + t21272 * t21264) * ((t21184 -
     # t20459) * t94 / 0.2E1 + t20461 / 0.2E1)) * t236 / 0.2E1 + t5069 +
     # (t1766 - t21284 * (t21270 * t21277 + t21262 * t21268 + t21266 * t
     #21264) * ((t21239 - t20459) * t183 / 0.2E1 + (t20459 - t21251) * t
     #183 / 0.2E1)) * t236 / 0.2E1 + (t1812 - t4 * (t1808 / 0.2E1 + t212
     #83 * (t21313 + t21314 + t21315) / 0.2E1) * t20584) * t236
        t21325 = t21324 * t1299
        t21333 = t809 * t21138
        t21337 = t12053 ** 2
        t21338 = t12066 ** 2
        t21339 = t12064 ** 2
        t21357 = u(t6458,j,t20351,n)
        t21374 = t19952 * t8354
        t21387 = t12391 ** 2
        t21388 = t12383 ** 2
        t21389 = t12387 ** 2
        t21392 = t8332 ** 2
        t21393 = t8324 ** 2
        t21394 = t8328 ** 2
        t21396 = t8345 * (t21392 + t21393 + t21394)
        t21401 = t12696 ** 2
        t21402 = t12688 ** 2
        t21403 = t12692 ** 2
        t21412 = u(t96,t180,t20351,n)
        t21416 = t8303 / 0.2E1 + (t8301 - t21412) * t236 / 0.2E1
        t21420 = t7797 * t21064
        t21424 = u(t96,t185,t20351,n)
        t21428 = t8315 / 0.2E1 + (t8313 - t21424) * t236 / 0.2E1
        t21434 = rx(t96,j,t20351,0,0)
        t21435 = rx(t96,j,t20351,1,1)
        t21437 = rx(t96,j,t20351,2,2)
        t21439 = rx(t96,j,t20351,1,2)
        t21441 = rx(t96,j,t20351,2,1)
        t21443 = rx(t96,j,t20351,1,0)
        t21445 = rx(t96,j,t20351,0,2)
        t21447 = rx(t96,j,t20351,0,1)
        t21450 = rx(t96,j,t20351,2,0)
        t21456 = 0.1E1 / (t21434 * t21435 * t21437 - t21434 * t21439 * t
     #21441 + t21443 * t21441 * t21445 - t21443 * t21447 * t21437 + t214
     #50 * t21447 * t21439 - t21450 * t21435 * t21445)
        t21457 = t4 * t21456
        t21486 = t21450 ** 2
        t21487 = t21441 ** 2
        t21488 = t21437 ** 2
        t20217 = t12405 * (t12391 * t12382 + t12395 * t12383 + t12393 * 
     #t12387)
        t20222 = t12710 * (t12687 * t12696 + t12700 * t12688 + t12698 * 
     #t12692)
        t21497 = (t21025 - t4 * (t21021 / 0.2E1 + t12075 * (t21337 + t21
     #338 + t21339) / 0.2E1) * t8352) * t94 + t21051 + (t21048 - t12076 
     #* (t12053 * t12062 + t12066 * t12054 + t12064 * t12058) * t12099) 
     #* t94 / 0.2E1 + t21069 + (t21066 - t11412 * (t8248 / 0.2E1 + (t824
     #6 - t21357) * t236 / 0.2E1)) * t94 / 0.2E1 + (t20217 * t12413 - t2
     #1374) * t183 / 0.2E1 + (t21374 - t20222 * t12718) * t183 / 0.2E1 +
     # (t4 * (t12404 * (t21387 + t21388 + t21389) / 0.2E1 + t21396 / 0.2
     #E1) * t8365 - t4 * (t21396 / 0.2E1 + t12709 * (t21401 + t21402 + t
     #21403) / 0.2E1) * t8367) * t183 + (t11722 * t21416 - t21420) * t18
     #3 / 0.2E1 + (t21420 - t11999 * t21428) * t183 / 0.2E1 + t8359 + (t
     #8356 - t21457 * (t21434 * t21450 + t21447 * t21441 + t21445 * t214
     #37) * (t20464 / 0.2E1 + (t20462 - t21357) * t94 / 0.2E1)) * t236 /
     # 0.2E1 + t8374 + (t8371 - t21457 * (t21443 * t21450 + t21435 * t21
     #441 + t21439 * t21437) * ((t21412 - t20462) * t183 / 0.2E1 + (t204
     #62 - t21424) * t183 / 0.2E1)) * t236 / 0.2E1 + (t8383 - t4 * (t837
     #9 / 0.2E1 + t21456 * (t21486 + t21487 + t21488) / 0.2E1) * t20598)
     # * t236
        t21498 = t21497 * t8344
        t21511 = t4726 * t9043
        t21524 = t5805 ** 2
        t21525 = t5818 ** 2
        t21526 = t5816 ** 2
        t21529 = t8652 ** 2
        t21530 = t8665 ** 2
        t21531 = t8663 ** 2
        t21533 = t8674 * (t21529 + t21530 + t21531)
        t21538 = t12382 ** 2
        t21539 = t12395 ** 2
        t21540 = t12393 ** 2
        t21552 = t19966 * t8696
        t21564 = t8081 * t21116
        t21582 = t15240 ** 2
        t21583 = t15232 ** 2
        t21584 = t15236 ** 2
        t21593 = u(i,t1328,t20351,n)
        t21603 = rx(i,t180,t20351,0,0)
        t21604 = rx(i,t180,t20351,1,1)
        t21606 = rx(i,t180,t20351,2,2)
        t21608 = rx(i,t180,t20351,1,2)
        t21610 = rx(i,t180,t20351,2,1)
        t21612 = rx(i,t180,t20351,1,0)
        t21614 = rx(i,t180,t20351,0,2)
        t21616 = rx(i,t180,t20351,0,1)
        t21619 = rx(i,t180,t20351,2,0)
        t21625 = 0.1E1 / (t21603 * t21604 * t21606 - t21603 * t21608 * t
     #21610 + t21612 * t21610 * t21614 - t21612 * t21616 * t21606 + t216
     #19 * t21616 * t21608 - t21619 * t21604 * t21614)
        t21626 = t4 * t21625
        t21655 = t21619 ** 2
        t21656 = t21610 ** 2
        t21657 = t21606 ** 2
        t21666 = (t4 * (t5827 * (t21524 + t21525 + t21526) / 0.2E1 + t21
     #533 / 0.2E1) * t5836 - t4 * (t21533 / 0.2E1 + t12404 * (t21538 + t
     #21539 + t21540) / 0.2E1) * t8681) * t94 + (t20081 * t5851 - t21552
     #) * t94 / 0.2E1 + (t21552 - t20217 * t12426) * t94 / 0.2E1 + (t550
     #9 * t21243 - t21564) * t94 / 0.2E1 + (t21564 - t11717 * t21416) * 
     #t94 / 0.2E1 + (t15254 * (t15231 * t15240 + t15244 * t15232 + t1524
     #2 * t15236) * t15264 - t21075) * t183 / 0.2E1 + t21080 + (t4 * (t1
     #5253 * (t21582 + t21583 + t21584) / 0.2E1 + t21094 / 0.2E1) * t869
     #4 - t21103) * t183 + (t14391 * (t8644 / 0.2E1 + (t8642 - t21593) *
     # t236 / 0.2E1) - t21118) * t183 / 0.2E1 + t21123 + t8688 + (t8685 
     #- t21626 * (t21603 * t21619 + t21616 * t21610 + t21614 * t21606) *
     # ((t21239 - t20352) * t94 / 0.2E1 + (t20352 - t21412) * t94 / 0.2E
     #1)) * t236 / 0.2E1 + t8701 + (t8698 - t21626 * (t21619 * t21612 + 
     #t21604 * t21610 + t21608 * t21606) * ((t21593 - t20352) * t183 / 0
     #.2E1 + t20617 / 0.2E1)) * t236 / 0.2E1 + (t8710 - t4 * (t8706 / 0.
     #2E1 + t21625 * (t21655 + t21656 + t21657) / 0.2E1) * t20354) * t23
     #6
        t21667 = t21666 * t8673
        t21675 = t837 * t21138
        t21679 = t6174 ** 2
        t21680 = t6187 ** 2
        t21681 = t6185 ** 2
        t21684 = t8957 ** 2
        t21685 = t8970 ** 2
        t21686 = t8968 ** 2
        t21688 = t8979 * (t21684 + t21685 + t21686)
        t21693 = t12687 ** 2
        t21694 = t12700 ** 2
        t21695 = t12698 ** 2
        t21707 = t19972 * t9001
        t21719 = t8378 * t21125
        t21737 = t17506 ** 2
        t21738 = t17498 ** 2
        t21739 = t17502 ** 2
        t21748 = u(i,t1335,t20351,n)
        t21758 = rx(i,t185,t20351,0,0)
        t21759 = rx(i,t185,t20351,1,1)
        t21761 = rx(i,t185,t20351,2,2)
        t21763 = rx(i,t185,t20351,1,2)
        t21765 = rx(i,t185,t20351,2,1)
        t21767 = rx(i,t185,t20351,1,0)
        t21769 = rx(i,t185,t20351,0,2)
        t21771 = rx(i,t185,t20351,0,1)
        t21774 = rx(i,t185,t20351,2,0)
        t21780 = 0.1E1 / (t21758 * t21759 * t21761 - t21758 * t21763 * t
     #21765 + t21767 * t21765 * t21769 - t21767 * t21771 * t21761 + t217
     #74 * t21771 * t21763 - t21774 * t21759 * t21769)
        t21781 = t4 * t21780
        t21810 = t21774 ** 2
        t21811 = t21765 ** 2
        t21812 = t21761 ** 2
        t21821 = (t4 * (t6196 * (t21679 + t21680 + t21681) / 0.2E1 + t21
     #688 / 0.2E1) * t6205 - t4 * (t21688 / 0.2E1 + t12709 * (t21693 + t
     #21694 + t21695) / 0.2E1) * t8986) * t94 + (t20087 * t6220 - t21707
     #) * t94 / 0.2E1 + (t21707 - t20222 * t12731) * t94 / 0.2E1 + (t585
     #3 * t21255 - t21719) * t94 / 0.2E1 + (t21719 - t11994 * t21428) * 
     #t94 / 0.2E1 + t21089 + (t21086 - t17520 * (t17497 * t17506 + t1751
     #0 * t17498 + t17508 * t17502) * t17530) * t183 / 0.2E1 + (t21112 -
     # t4 * (t21108 / 0.2E1 + t17519 * (t21737 + t21738 + t21739) / 0.2E
     #1) * t8999) * t183 + t21130 + (t21127 - t16510 * (t8949 / 0.2E1 + 
     #(t8947 - t21748) * t236 / 0.2E1)) * t183 / 0.2E1 + t8993 + (t8990 
     #- t21781 * (t21758 * t21774 + t21771 * t21765 + t21769 * t21761) *
     # ((t21251 - t20374) * t94 / 0.2E1 + (t20374 - t21424) * t94 / 0.2E
     #1)) * t236 / 0.2E1 + t9006 + (t9003 - t21781 * (t21774 * t21767 + 
     #t21759 * t21765 + t21763 * t21761) * (t20619 / 0.2E1 + (t20374 - t
     #21748) * t183 / 0.2E1)) * t236 / 0.2E1 + (t9015 - t4 * (t9011 / 0.
     #2E1 + t21780 * (t21810 + t21811 + t21812) / 0.2E1) * t20376) * t23
     #6
        t21822 = t21821 * t8978
        t21857 = (t4987 * t6264 - t5309 * t9041) * t94 + (t4468 * t6290 
     #- t21155) * t94 / 0.2E1 + (t21155 - t4931 * t12797) * t94 / 0.2E1 
     #+ (t492 * (t5073 / 0.2E1 + (t5071 - t21325) * t236 / 0.2E1) - t213
     #33) * t94 / 0.2E1 + (t21333 - t4953 * (t8389 / 0.2E1 + (t8387 - t2
     #1498) * t236 / 0.2E1)) * t94 / 0.2E1 + (t4962 * t15323 - t21511) *
     # t183 / 0.2E1 + (t21511 - t4971 * t17589) * t183 / 0.2E1 + (t5375 
     #* t9063 - t5384 * t9065) * t183 + (t3817 * (t8716 / 0.2E1 + (t8714
     # - t21667) * t236 / 0.2E1) - t21675) * t183 / 0.2E1 + (t21675 - t4
     #067 * (t9021 / 0.2E1 + (t9019 - t21822) * t236 / 0.2E1)) * t183 / 
     #0.2E1 + t9048 + (t9045 - t5046 * ((t21325 - t21134) * t94 / 0.2E1 
     #+ (t21134 - t21498) * t94 / 0.2E1)) * t236 / 0.2E1 + t9072 + (t906
     #9 - t5059 * ((t21667 - t21134) * t183 / 0.2E1 + (t21134 - t21822) 
     #* t183 / 0.2E1)) * t236 / 0.2E1 + (t9074 - t5469 * t21136) * t236
        t21859 = t895 * t21857 * t813
        t21869 = t19946 * t7140
        t21883 = t2576 / 0.2E1 + t20710 / 0.2E1
        t21885 = t5046 * t21883
        t21899 = t19946 * t7076
        t21917 = t5059 * t21883
        t21930 = (t21015 * t2231 - t21024 * t7074) * t94 + (t19941 * t24
     #08 - t21869) * t94 / 0.2E1 + (t21869 - t19952 * t10808) * t94 / 0.
     #2E1 + (t1202 * (t2355 / 0.2E1 + t20870 / 0.2E1) - t21885) * t94 / 
     #0.2E1 + (t21885 - t7777 * (t7176 / 0.2E1 + t20884 / 0.2E1)) * t94 
     #/ 0.2E1 + (t19966 * t14139 - t21899) * t183 / 0.2E1 + (t21899 - t1
     #9972 * t16623) * t183 / 0.2E1 + (t21102 * t7136 - t21111 * t7138) 
     #* t183 + (t8101 * (t6954 / 0.2E1 + t20758 / 0.2E1) - t21917) * t18
     #3 / 0.2E1 + (t21917 - t8394 * (t6975 / 0.2E1 + t20777 / 0.2E1)) * 
     #t183 / 0.2E1 + t10069 + t20845 / 0.2E1 + t10070 + t20938 / 0.2E1 +
     # t20796
        t21936 = t19906 * (t10074 / 0.2E1 + (t10072 - t21930 * t5431) * 
     #t236 / 0.2E1)
        t21940 = t19046 * (t5476 - t21136)
        t21943 = t2 + t6846 - t19990 + t7310 - t19993 + t19999 + t9081 -
     # t20004 + t20008 - t962 - t1227 * t20705 - t20721 - t2079 * t21000
     # / 0.2E1 - t1227 * t21139 / 0.2E1 - t21147 - t2923 * t21859 / 0.6E
     #1 - t2079 * t21936 / 0.4E1 - t1227 * t21940 / 0.12E2
        t21956 = sqrt(t20013 + t20014 + t20015 + 0.8E1 * t875 + 0.8E1 * 
     #t876 + 0.8E1 * t877 - 0.2E1 * dz * ((t861 + t862 + t863 - t866 - t
     #867 - t868) * t236 / 0.2E1 - (t875 + t876 + t877 - t5462 - t5463 -
     # t5464) * t236 / 0.2E1))
        t21957 = 0.1E1 / t21956
        t21962 = t6822 * t9213 * t20307
        t21965 = t882 * t9216 * t20312 / 0.2E1
        t21968 = t882 * t9220 * t20317 / 0.6E1
        t21970 = t9213 * t20320 / 0.24E2
        t21982 = t2 + t9240 - t19990 + t9242 - t20052 + t19999 + t9246 -
     # t20054 + t20056 - t962 - t9226 * t20705 - t20721 - t9228 * t21000
     # / 0.2E1 - t9226 * t21139 / 0.2E1 - t21147 - t9233 * t21859 / 0.6E
     #1 - t9228 * t21936 / 0.4E1 - t9226 * t21940 / 0.12E2
        t21985 = 0.2E1 * t20323 * t21982 * t21957
        t21987 = (t6822 * t136 * t20307 + t882 * t159 * t20312 / 0.2E1 +
     # t882 * t893 * t20317 / 0.6E1 - t136 * t20320 / 0.24E2 + 0.2E1 * t
     #20323 * t21943 * t21957 - t21962 - t21965 - t21968 + t21970 - t219
     #85) * t133
        t21993 = t6822 * (t272 - dz * t6547 / 0.24E2)
        t21995 = dz * t6554 / 0.24E2
        t22000 = t18095 * t161 / 0.6E1 + (t18158 + t18085 + t18088 - t18
     #162 + t18091 - t18093 - t18095 * t9212) * t161 / 0.2E1 + t18276 * 
     #t161 / 0.6E1 + (t18339 + t18266 + t18269 - t18343 + t18272 - t1827
     #4 - t18276 * t9212) * t161 / 0.2E1 + t20062 * t161 / 0.6E1 + (t200
     #68 + t20031 + t20034 - t20070 + t20037 - t20039 + t20060 - t20062 
     #* t9212) * t161 / 0.2E1 - t20150 * t161 / 0.6E1 - (t20181 + t20140
     # + t20143 - t20185 + t20146 - t20148 - t20150 * t9212) * t161 / 0.
     #2E1 - t20265 * t161 / 0.6E1 - (t20296 + t20255 + t20258 - t20300 +
     # t20261 - t20263 - t20265 * t9212) * t161 / 0.2E1 - t21987 * t161 
     #/ 0.6E1 - (t21993 + t21962 + t21965 - t21995 + t21968 - t21970 + t
     #21985 - t21987 * t9212) * t161 / 0.2E1
        t22034 = t9256 * dt / 0.2E1 + (t9262 + t9215 + t9219 - t9264 + t
     #9223 - t9225 + t9254) * dt - t9256 * t9213 + t9669 * dt / 0.2E1 + 
     #(t9731 + t9657 + t9661 - t9735 + t9665 - t9667) * dt - t9669 * t92
     #13 + t10096 * dt / 0.2E1 + (t10158 + t10086 + t10089 - t10162 + t1
     #0092 - t10094) * dt - t10096 * t9213 - t12965 * dt / 0.2E1 - (t129
     #71 + t12940 + t12943 - t12973 + t12946 - t12948 + t12963) * dt + t
     #12965 * t9213 - t13171 * dt / 0.2E1 - (t13202 + t13161 + t13164 - 
     #t13206 + t13167 - t13169) * dt + t13171 * t9213 - t13388 * dt / 0.
     #2E1 - (t13419 + t13378 + t13381 - t13423 + t13384 - t13386) * dt +
     # t13388 * t9213
        t22067 = t13543 * dt / 0.2E1 + (t13615 + t13533 + t13536 - t1361
     #9 + t13539 - t13541) * dt - t13543 * t9213 + t15526 * dt / 0.2E1 +
     # (t15532 + t15495 + t15498 - t15534 + t15501 - t15503 + t15524) * 
     #dt - t15526 * t9213 + t15734 * dt / 0.2E1 + (t15787 + t15724 + t15
     #727 - t15791 + t15730 - t15732) * dt - t15734 * t9213 - t15871 * d
     #t / 0.2E1 - (t15902 + t15861 + t15864 - t15906 + t15867 - t15869) 
     #* dt + t15871 * t9213 - t17761 * dt / 0.2E1 - (t17767 + t17736 + t
     #17739 - t17769 + t17742 - t17744 + t17759) * dt + t17761 * t9213 -
     # t17943 * dt / 0.2E1 - (t17974 + t17933 + t17936 - t17978 + t17939
     # - t17941) * dt + t17943 * t9213
        t22100 = t18095 * dt / 0.2E1 + (t18158 + t18085 + t18088 - t1816
     #2 + t18091 - t18093) * dt - t18095 * t9213 + t18276 * dt / 0.2E1 +
     # (t18339 + t18266 + t18269 - t18343 + t18272 - t18274) * dt - t182
     #76 * t9213 + t20062 * dt / 0.2E1 + (t20068 + t20031 + t20034 - t20
     #070 + t20037 - t20039 + t20060) * dt - t20062 * t9213 - t20150 * d
     #t / 0.2E1 - (t20181 + t20140 + t20143 - t20185 + t20146 - t20148) 
     #* dt + t20150 * t9213 - t20265 * dt / 0.2E1 - (t20296 + t20255 + t
     #20258 - t20300 + t20261 - t20263) * dt + t20265 * t9213 - t21987 *
     # dt / 0.2E1 - (t21993 + t21962 + t21965 - t21995 + t21968 - t21970
     # + t21985) * dt + t21987 * t9213

        unew(i,j,k) = t1 + dt * t2 + t13428 * t56 * t94 + t17983 * t5
     #6 * t183 + t22000 * t56 * t236

        utnew(i,j,k) = t2 + t22034 * t56 * t94 + t220
     #67 * t56 * t183 + t22100 * t56 * t236

c        blah = array(int(t1 + dt * t2 + t13428 * t56 * t94 + t17983 * t5
c     #6 * t183 + t22000 * t56 * t236),int(t2 + t22034 * t56 * t94 + t220
c     #67 * t56 * t183 + t22100 * t56 * t236))

        return
      end
