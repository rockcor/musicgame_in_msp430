clear all
Gsound=readmidi('C.mid');
Ginfo=midiInfo(Gsound);
%{
1.track number
2.channel number
3.note number (midi encoding of pitch)
4.velocity
5.start time (seconds)
6.end time (seconds)
7.message number of note_on
8.message number of note_off
%}
%1开始的输出对照表
table=[30264,28565,26961,25448,24020,22672,21400,20198,19064,17995,16985,16031,15132,14283,13481,12724,12010,11336,10700,10099,9532,8997,8492,8016,7566,7141,6740,6362,6005,5668,5350,5050,4766,4499,4246,4008,3783,3571,3370,3181,3003,2834,2675,2525,2383,2249,2123,2004,1891,1785,1685,1591,1501,1417,1337,1262,1192,1125,1062,1002,946,893,843,795,751,708,669,631,596,562,531,501,473,446,421,398,375,354,334,316,298,281,265,250,236,223,211,199,188,177,167,158,149,141,133,125,118,112,105,99,94,89,84,79,74,70,66,63,59,56,53,50,47,44,42,39,37,35,33,31,30,28,26,25,23,22,21];
musicData1=[];
musicData2=[];
musicFrame1=[];
musicFrame2=[];
sz=size(Ginfo);
for i=1:sz(1)
    if Ginfo(i,1)==1
        musicData1=[musicData1,num2str(table(Ginfo(i,3))),','];
        musicFrame1=[musicFrame1,num2str(round(Ginfo(i,5)*30)),','];
        %musicData1=[musicData1,num2str(65535),','];
        %musicFrame1=[musicFrame1,num2str(round(Ginfo(i,6)*30)),','];
    elseif Ginfo(i,1)==2
        musicData2=[musicData2,num2str(table(Ginfo(i,3))),','];
        musicFrame2=[musicFrame2,num2str(round(Ginfo(i,5)*30)),','];
        %musicData2=[musicData2,num2str(65535),','];
        %musicFrame2=[musicFrame2,num2str(round(Ginfo(i,6)*30)),','];
    end
end