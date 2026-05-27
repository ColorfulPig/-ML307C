/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*********************************************************************
* Filename:     AUX_TABLE_AfcParameters_CalBody.h
* Created By:   Terry Yao on 12/06/82009
* Programmers:  Terry Yao
* Module:       Generic RF Module - Data Base
* RF Type:      SKY_Hel4
* Description:  Contains the "AUX_TABLE_AfcParameters" CalBody
*
* Notes:      
*********************************************************************/

/*
 * each line in the file contain the data of 5*Int16:
*/
//freqoffset/ dacoffset/0.9(in 900MHz band) = slope
// Multiplier = 1000               divisor = Multiplier * slope
// for SP8  freqoffset = 80  dacoffset = 100    for POR307 we change from 18500 to 17000
//|-Multiplier-------------|-Divisor----------------|-NominalValue-----------|-Min_DAC----------------|-Max_DAC----------------|
1000,					 850,				   15104,					4000, 			 28000,   //
