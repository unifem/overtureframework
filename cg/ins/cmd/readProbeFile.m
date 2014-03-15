function  [headers,labels,t,q] = readProbeFile(fname)
% 
%  readProbeFile  reads data from a file that contains headers and column data
% 
%  Usage: 
%     [headers,labels,t,q] = readProbeFile(fname)
%
%  fname  (input) : name of the file containing the data (required)
%  headers (output) : matrix of header labels
%  labels  (output) : matrix of labels.  
%  t (output) : column vector of time. 
%  q (output) : matrix of solution values.
%
% 
% ---- Here is the format of the file: -----
%   numHead numCols
%   header-line 1
%   header-line 2
%   ...
%   header-line numHead
%      t    x  y  z  var1 var2 ...                  (labels for columns, a total of numCols labels)
%      t1  x1 y1 z1  v11   v21  ...                 (time, position, values)
%      t2  x2 y2 z2  v12   v22  ...
%      .   .  .  .   .     .
% 

%  open file for input
fin = fopen(fname,'r');
if fin < 0
   error(['Could not open ',fname,' for input']);
end

% read first line and extra the number-of-headers and number-of-columns
% determine the max length of any header comment so we can allocate storage

buffer = fgetl(fin);  
maxHeaderLen = length(buffer);  
head1 = sscanf(buffer,'%d'); 
nhead=head1(1);
ncols=head1(2);

fprintf('readProbeFile: nhead=%d ncols=%d\n',nhead,ncols);

for i=2:nhead
 buffer = fgetl(fin);  
 maxHeaderLen = max(maxHeaderLen,length(buffer));
end

% Now read the column headers and find the length of the longest column label
maxlen = 0;
buffer = fgetl(fin);          %  get next line as a string
for j=1:ncols
   [next,buffer] = strtok(buffer);       %  parse next column label
   maxlen = max(maxlen,length(next));   %  find the longest so far
   % fprintf(' label %d = [%s]\n',j,next);
end

% fprintf(1,' labels : maxlen=%d\n',maxlen);
labels = blanks(maxlen);

frewind(fin);    %  rewind in preparation for actual reading of labels and data

%  Now read and save headers...
headers = blanks(maxHeaderLen);
for i=1:nhead  
   buffer = fgetl(fin);  
   headers(i,1:length(buffer)) = buffer;
end

%  Read labels
buffer = fgetl(fin);          %  get next line as a string
for j=1:ncols
   [next,buffer] = strtok(buffer);     %  parse next column label
   labels(j,1:length(next)) = next;    %  append to the labels matrix
end

%  Read in the data.

data = fscanf(fin,'%f');  %  Load the numerical values into one long vector

nd = length(data);        %  total number of data points
nr = nd/ncols;            %  number of rows; 
if nr ~= round(nd/ncols)
   fprintf(1,'\ndata: nrow = %f\tncol = %d\n',nr,ncols);
   fprintf(1,'number of data points = %d does not equal nrow*ncol\n',nd);
   error('data is not rectangular')
end

data = reshape(data,ncols,nr)';   %  note transpose 
t = data(:,1);
q = data(:,2:ncols);

fclose(fin);
%  end of readProbeFile.m
