
%---------------------------------------------------------------------
% Modelo CP para colorir o mapa de Goiás + estados vizinhos
% Preparado por: Cid C. de Souza - IC-Unicamp
% Data: 20190424
%---------------------------------------------------------------------

int: nc = 3; %% número de cores possíveis

% Variáveis: uma  por estado,  podendo assumir  uma das 4 cores

var 1..nc: GO;   var 1..nc: TO;  var 1..nc: BA;   
var 1..nc: MG;   var 1..nc: MS;  var 1..nc: MT;   

% Restrições: estados vizinhos não podem ter a mesma cor
constraint GO != TO;
constraint GO != BA;
constraint GO != MG;
constraint GO != MS;
constraint GO != MT;
constraint TO != BA;
constraint BA != MG;
constraint MG != MS;
constraint MS != MT;
constraint MT != TO;

solve satisfy;

output ["GO=\(GO)\t TO=\(TO)\t BA=\(BA)\n",
        "MG=\(MG)\t MS=\(MS)\t MT=\(MT)\n"]




