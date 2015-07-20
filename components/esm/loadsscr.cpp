#include "loadsscr.hpp"

#include "esmreader.hpp"
#include "esmwriter.hpp"
#include "defs.hpp"

namespace ESM
{
    unsigned int StartScript::sRecordId = REC_SSCR;

    void StartScript::load(ESMReader &esm, bool &isDeleted)
    {
        isDeleted = false;

        bool hasData = false;
        bool hasName = false;
        while (esm.hasMoreSubs())
        {
            esm.getSubName();
            switch (esm.retSubName().val)
            {
                case ESM::FourCC<'D','A','T','A'>::value:
                    mData = esm.getHString();
                    hasData = true;
                    break;
                case ESM::FourCC<'N','A','M','E'>::value:
                    mId = esm.getHString();
                    hasName = true;
                    break;
                case ESM::FourCC<'D','E','L','E'>::value:
                    esm.skipHSub();
                    isDeleted = true;
                    break;
                default:
                    esm.fail("Unknown subrecord");
                    break;
            }
        }

        if (!hasData)
            esm.fail("Missing DATA");
        if (!hasName)
            esm.fail("Missing NAME");
    }
    void StartScript::save(ESMWriter &esm, bool isDeleted) const
    {
        esm.writeHNString("DATA", mData);
        esm.writeHNString("NAME", mId);

        if (isDeleted)
        {
            esm.writeHNCString("DELE", "");
        }
    }

    void StartScript::blank()
    {
        mData.clear();
    }
}
