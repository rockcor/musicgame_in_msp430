for i =1:8
    picData=[];
    p=imread([num2str(i),'.jpg']);
    for j=1:240
        for k=1:320
            if p(j,k)<100
                picData=[picData,',{',num2str(k-1),',',num2str(j-1),'}'];
            end
        end
    end
    fid=fopen([num2str(i),'.txt'],'wt');
    fprintf(fid,'%s\n',picData);
    fclose(fid);
end