tm4c = serial('COM8');
set(tm4c)
set(tm4c, 'BaudRate', 115200);
fopen(tm4c);
out = fscanf(tm4c);
disp(out);
fclose(tm4c);
delete(tm4c);
clear tm4c