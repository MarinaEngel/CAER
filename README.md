# CAER
A short manual of CAER (Cloud Adjacency Effect Radius) program. CAER produce an upper estimate of the distance from a cloud in a cloud field where the atmosphere is considered cloudless, for reconstructing the reflection coefficients of the earth's surface. 
To produce the calculation it is required to form the file "input.txt".

The example of the file "input.txt".

3  – a wavelength number (1: 0.41 mkm, 2: 0.47 mkm, 3: 0.55 mkm, 4: 0.68 mkm, 5: 0.86 mkm)
0.43  – aerosol optical depth
27 – solar zenith angle (deg)
34 – satellite zenith angle (deg)
1.6 – mean cloud geometric thickness (km)
0.087 – cloud cover index
60 – cloud extinction (reciprocal km)
1 – reflectance

The first line contain a number of wavelength in list '0.41 mkm, 0.47 mkm, 0.55 mkm, 0.68 mkm, 0.86 mkm.' Then 3 corresponds to 0.55 mkm.
The second line contain a mean of aerosol optical depth, units are dimensionless. It is 0.43 in this example. The valid range for this value is 0-1.27, 0-1.14, 0-0.89, 0-0.73, and 0-0.54 for each adjusted wavelength.
The third line contain a mean of solar zenith angle, units are degrees. It is 27 in this example.  The valid range for this value is 0-60.
The fourth line contain a mean of satellite zenith angle, units are degrees. It is 34 in this example.  The valid range for this value is 0-60.
The fifth line contain a mean of cloud geometric thickness, units are km. It is 1.6 in this example. 
The valid range for this value is 0.5-6.
The sixths line contain a mean of cloud cover index, units are km. It is 0.087 in this example. The valid range for this value is 0.5 – 6, type is real.
The sevenths line contain a mean of cloud extinction, units are reciprocal km. It is 60 in this example. The valid range for this value is 10 – 60, type is real.
The eights line contain a mean of reflectance, units are dimensionless. The valid range for this value is 0 – 1, type is real.

The calculation results are written in a file 'res.txt'. The  calculation parameters have been written in first eight lines. The last line contains the distance from the cloud in the cloud field (units are km),  where the atmosphere is considered cloudless. This value can be used for reconstructing the reflection coefficients of the earth's surface.

The example of the file "res.txt".

Wavelength number 3 (0.55 mkm)
Aerosol optical depth: 0.43
Solar zenith angle: 27 deg
Satellite zenith angle: 34 deg
Cloud geometric thickness: 1.6 km
Cloud cover index: 0.087
Cloud extinction: 60 reciprocal km
Ground reflectance: 1
RCritical = 4.7555

CAER program have been compiled  in the OS Windows using compiler GCC. Any other compiler  C can be use to get executable code. To produce the calculation it is required to form the file "input.txt" in according to description being given above and to place in a folder with executable code.

Contacts: principal investigator Mikhail Tarasenkov, e-mail tmv@iao.ru.
Organization: V.E. Zuev Institute of Atmospheric Optics SB RAS (IAO SB RAS)
Postal Address: 1, Academician Zuev square
Tomsk 634055
Russia
