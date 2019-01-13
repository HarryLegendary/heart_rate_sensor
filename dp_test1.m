delete(instrfind);
clear;
s = serial('COM4','BaudRate',38400,'TimeOut',20,'Terminator','LF');
fopen(s);
x = 0;
while(x<60000)
    x = x+1;
    %received = fgets(s);
    y(x) = fscanf(s, '%d');
    %disp(received);
    
    plot(y,'b-','linewidth',2)
    title('pulse graph');
    xlabel('Time in 25ms');
    ylabel('SCI input');
    if(x<30)
        startSpot = 0;
    else
        startSpot = x-30;
    end
    axis([ startSpot,startSpot+30,0,1010]);
    grid
    drawnow;
    %pause(0.001)
end
fclose(s);
