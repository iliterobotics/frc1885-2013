#include "bhs_GDTransmitData.h"

bhs_GDTransmitData::bhs_GDTransmitData()
	: mtd_diagnosticData()
{
	memset(&mtd_diagnosticData, 0, sizeof(mtd_diagnosticData));
}
