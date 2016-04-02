a = '/Users/wachs/Dropbox/SIFT/Videos/MundoVisto-de-Cima/ScreenCaptureProject108.avi';
reader = VideoReader(a);
numFrames = get(reader, 'NumberOfFrames');

dif=importdata('diferencas.txt');

mat = [];
inicio = 4000;
for k = 4000 : numFrames       
    if dif(k) > 100000 && k-inicio>50
        display(['pronto | carregando...', num2str(inicio) , ' até ', num2str(k)]);
        %z = implay( reader.read([inicio,k]));
        mat = [mat; inicio, k];
        inicio = k;
        %input('continua?')
        %close(z);
    end   
end
 
