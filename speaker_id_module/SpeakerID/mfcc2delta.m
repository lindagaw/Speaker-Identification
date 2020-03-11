function delta_coeff = mfcc2delta(CepCoeff, d)

% CepCoeff: Cepstral Coefficient
% d : Lag size for delta feature computation 
% Output:-> delta_coeff: Output delta coefficient 
CepCoeff=CepCoeff';

[NoOfFrame, NoOfCoeff]=size(CepCoeff);

vf=(d:-1:-d);
vf=vf/sum(vf.^2);

ww=ones(d,1);

cx=[CepCoeff(ww,:); CepCoeff; CepCoeff(NoOfFrame*ww,:)];

vx=reshape(filter(vf,1,cx(:)),NoOfFrame+2*d,NoOfCoeff);
vx(1:2*d,:)=[];

delta_coeff=vx';

end