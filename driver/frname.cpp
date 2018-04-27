// inlined

/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include "kx.h"

enum Products{
    Generic = -1,
    SoundBlaster = 0,
    EMU = 1
};

int kx_get_friendly_name(unsigned int device,unsigned int subsys,unsigned int chiprev,
                         char *tmp_str,
                         unsigned char &is_51,
                         unsigned char &has_surdac,
                         unsigned char &is_aps,
                         unsigned char &is_10k2,
                         unsigned char &is_10k3,
                         unsigned char &is_a2,
                         unsigned char &is_a2ex,
                         unsigned char &is_k8,
                         unsigned char &is_a4,
                         unsigned char &is_edsp,
                         char &have_ac97,
                         char &lack_ac97,
                         unsigned char &is_zsnb,
                         unsigned char &is_cardbus
                         ,CardBus &busName);

int kx_get_friendly_name(unsigned int device,unsigned int subsys,unsigned int chiprev,
                         char *tmp_str,
                         unsigned char &is_51,
                         unsigned char &has_surdac,
                         unsigned char &is_aps,
                         unsigned char &is_10k2,
                         unsigned char &is_10k3,
                         unsigned char &is_a2,
                         unsigned char &is_a2ex,
                         unsigned char &is_k8,
                         unsigned char &is_a4,
                         unsigned char &is_edsp,
                         char &have_ac97,
                         char &lack_ac97,
                         unsigned char &is_zsnb,
                         unsigned char &is_cardbus,
                         CardBus &busName)
{
    int ret=0;
    lack_ac97=0;
    have_ac97=0; // autodetect
    
    if(device==0x00041102) // 10k2
        is_10k2=1;
    
    if(device==0x00041102 && chiprev==4)  // a2
        is_a2=1;
    
    if(device==0x00081102) // a2value, ... with p17v
    {
        is_10k2=1;
        is_a2=1;
        is_k8=1;
        
        // this is assumed:
        // if((subsys&0xf0000000)==0x10000000) // any ca0108
        // is_k8=1;
        
        if((subsys&0xf0000000)==0x20000000) // any zsnb
            is_cardbus=1;
        
        if((subsys&0xf0000000)==0x40000000) // any e-dsp v2
            is_edsp=1;
        
        if((subsys&0xff000000)==0x42000000) // any e-dsp v2 cardbus
            is_cardbus=1;
    }
    
    if(is_edsp)
        lack_ac97=1;
    
    if(is_10k2)
        is_51=1; // assume 10k2 is always at least '5.1'
    
    busName=CardBus(0);
    
    busName=PCI;
    
    Products productName = SoundBlaster;
    
    switch(subsys)
    {
        case 0x40011102:
            productName = EMU;
            if(is_10k2==0) // [1]
            {
                strncpy(tmp_str,"EMUAPS",KX_MAX_STRING);
                is_aps=1;
                lack_ac97=1;
            }
            else
            {
                strncpy(tmp_str,"EM8810",KX_MAX_STRING); // [110] - 1010 v1
                is_edsp=1;
                lack_ac97=1;
            }
            break;
            
        case 0x00201102: // [2]
            strncpy(tmp_str,"live! CT4670",KX_MAX_STRING); // live! value
            break;
        case 0x00211102: // [3]
            strncpy(tmp_str,"live! CT4620",KX_MAX_STRING); // live! standard
            break;
        case 0x80221102: // [4]
            strncpy(tmp_str,"live! CT4780",KX_MAX_STRING); // value digital
            break;
        case 0x80231102: // [5]
            strncpy(tmp_str,"PCI 512 CT4790",KX_MAX_STRING); // 512
            break;
        case 0x80251102: // sb0040 ? too // [6]
            strncpy(tmp_str,"live! CT1140",KX_MAX_STRING); // MB
            break;
        case 0x80261102: // [7]
            strncpy(tmp_str,"live! CT4830",KX_MAX_STRING); // value digital
            break;
        case 0x80271102: // [8]
            strncpy(tmp_str,"live! CT483X",KX_MAX_STRING); // value digital
            break;
        case 0x80281102: // [9]
            strncpy(tmp_str,"live! CT4870",KX_MAX_STRING); // value
            break;
        case 0x80291102: // [10]
            strncpy(tmp_str,"live! CT4872",KX_MAX_STRING); // value
            break;
        case 0x802a1102: // [11]
            strncpy(tmp_str,"live! CT4890",KX_MAX_STRING); // k256
            break;
        case 0x802b1102: // [12]
            strncpy(tmp_str,"PCI 256 CT4891",KX_MAX_STRING); // 256
            break;
        case 0x002F1102: // M002/M003 // [13]
            strncpy(tmp_str,"live! CTM002",KX_MAX_STRING); // MB
            break;
        case 0x80311102: // [14]
            strncpy(tmp_str,"live! CT4831",KX_MAX_STRING); // value
            break;
        case 0x80321102: // [15]
            strncpy(tmp_str,"live! CT4871",KX_MAX_STRING); // value
            break;
        case 0x80331102: // [16]
            strncpy(tmp_str,"live! CT4893",KX_MAX_STRING); // k256
            break;
        case 0x80351102: // [17]
            strncpy(tmp_str,"live! CT0060",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x80401102: // [18]
            strncpy(tmp_str,"live! CT4760",KX_MAX_STRING); // live digital entertainment/platinum
            break;
            
        case 0x80501102: // [19]
            strncpy(tmp_str,"live! CT4750",KX_MAX_STRING); // was 4820?! k512
            break;
        case 0x80511102: // [20]
            strncpy(tmp_str,"live! CT4850",KX_MAX_STRING);
            break;
            
        case 0x80611102: // [21]
            strncpy(tmp_str,"live! 5.1 SB0060",KX_MAX_STRING);  // live 5.1
            is_51=1;
            has_surdac=1;
            break;
        case 0x80621102: // [22]
            strncpy(tmp_str,"live! 5.1 SB0100",KX_MAX_STRING);  // live 5.1
            is_51=1;
            has_surdac=1;
            break;
        case 0x80631102: // [23]
            productName = Generic;
            strncpy(tmp_str,"DXW MB",KX_MAX_STRING);
            break;
        case 0x80641102: // [24]
            strncpy(tmp_str,"live! 5.1 SB0102",KX_MAX_STRING);  // live 5.1
            is_51=1;
            has_surdac=1;
            break;
        case 0x80651102: // [25]
            strncpy(tmp_str,"live! 5.1 Digital SB022X",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x80661102: // [33]
            strncpy(tmp_str,"live! 5.1 SB0228",KX_MAX_STRING); // Russ's
            is_51=1;
            has_surdac=1;
            break;
        case 0x100a1102:
            if(!is_10k2)
            {
                if(chiprev==0xa)
                    strncpy(tmp_str,"live! 5.1 SB0229",KX_MAX_STRING); // [34] - LS
                else
                    strncpy(tmp_str,"live! 5.1 SB022b",KX_MAX_STRING); // [33]
                
                is_51=1;
                has_surdac=1;
            }
            else
            {
                strncpy(tmp_str,"Audigy 2 SB0246",KX_MAX_STRING); // Audigy2 gateway oem [88]
            }
            break;
        case 0x80671102: // [32]
            strncpy(tmp_str,"ive! 5.1 Digital SB0220",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x80681102: // [26]
            strncpy(tmp_str,"live! 5.1 CT0061",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x80691102: // [27]
            strncpy(tmp_str,"live! 5.1 SB0101",KX_MAX_STRING); // value
            is_51=1;
            has_surdac=1;
            break;
        case 0x806a1102: // [28]
            strncpy(tmp_str,"live! 5.1 SB0103",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x806b1102: // [29]
            strncpy(tmp_str,"live! 5.1 SB0105",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x806C1102: // [30]
            strncpy(tmp_str,"live! 5.1 Digital SB0221",KX_MAX_STRING);
            is_51=1;
            has_surdac=1;
            break;
        case 0x80711102: // [31]
            strncpy(tmp_str,"PCI 512 SB0150",KX_MAX_STRING); // PCI512
            is_51=1;
            has_surdac=1;
            break;
            
        case 0x00401102: // ?.. dead audigy [50]
            strncpy(tmp_str,"Audigy SB0090",KX_MAX_STRING); // audigy
            break;
        case 0x00411102:
            strncpy(tmp_str,"CT4820",KX_MAX_STRING); // [51]
            break;
        case 0x00421102:
            strncpy(tmp_str,"CT0070",KX_MAX_STRING); // [52]
            break;
        case 0x00431102:
            strncpy(tmp_str,"CT007X",KX_MAX_STRING); // ct0073? // [53]
            break;
        case 0x00511102: // [54]
            strncpy(tmp_str,"Audigy SB0090",KX_MAX_STRING); // audigy with golden connectors
            break;
        case 0x00521102: // [55]
            strncpy(tmp_str,"Audigy SB016X",KX_MAX_STRING); // audigy: plastic + gameport (no 1394)
            // unknown: sb0162 -- audigy w/midi jack + plastic jacks
            break;
        case 0x00531102: // [56]
            strncpy(tmp_str,"Audigy SB0092",KX_MAX_STRING); // audigy OEM / Player SB0092
            break;
        case 0x00541102: // [57]
            strncpy(tmp_str,"Audigy SB0161",KX_MAX_STRING); // audigy
            break;
        case 0x00551102: // [58]
            strncpy(tmp_str,"Audigy SB0192",KX_MAX_STRING); // audigy
            break;
        case 0x00561102: // [59]
            strncpy(tmp_str,"Audigy SB0191",KX_MAX_STRING); // audigy
            break;
        case 0x00571102: // [60]
            strncpy(tmp_str,"Audigy (OEM) SB0091",KX_MAX_STRING); // audigy
            break;
        case 0x00581102: // [61]
            strncpy(tmp_str,"Audigy (OEM) SB0095",KX_MAX_STRING); // audigy
            break;
        case 0x00591102: // [62]
            strncpy(tmp_str,"Audigy SB0230",KX_MAX_STRING); // audigy
            break;
        case 0x005A1102: // [63]
            strncpy(tmp_str,"Audigy SB0231",KX_MAX_STRING); // audigy
            break;
        case 0x005B1102: // [64]
            strncpy(tmp_str,"Audigy SB0232",KX_MAX_STRING); // audigy
            break;
        case 0x005C1102: // [65]
            strncpy(tmp_str,"Audigy SB0238",KX_MAX_STRING); // audigy
            break;
            
        case 0x10021102: // [80]
            strncpy(tmp_str,"Audigy 2 SB0240",KX_MAX_STRING); // Audigy2
            break;
        case 0x10031102: // [81]
            strncpy(tmp_str,"Audigy 2 SB0243",KX_MAX_STRING); // Audigy2
            break;
        case 0x10041102: // [82]
            strncpy(tmp_str,"Audigy 2 SB0242",KX_MAX_STRING); // Audigy2
            break;
        case 0x10051102: // [83]
            strncpy(tmp_str,"Audigy 2 Platinum EX SB0280",KX_MAX_STRING); // Audigy2 Platinum Ex; sb244, sb250, sb290
            is_a2ex=1;
            lack_ac97=1;
            break;
        case 0x10061102: // [86]
            strncpy(tmp_str,"Audigy 2 (OEM) SB0245",KX_MAX_STRING); // audigy2 gateway oem
            break;
        case 0x10071102: // [84]
            strncpy(tmp_str,"Audigy 2 (OEM) SB0244",KX_MAX_STRING); // audigy2 oem [riva's]
            break;
        case 0x10081102: // [85]
            strncpy(tmp_str,"Audigy 2 Platinum EX SB0320",KX_MAX_STRING); // Audigy2 Platinum Ex (with ac97 codec!) + sb290
            is_a2ex=1;
            break;
        case 0x10091102: // [87]
            strncpy(tmp_str,"Audigy 2 SB0249",KX_MAX_STRING); // Audigy2 unk
            break;
            
        case 0x20011102: // [101]
            if(is_k8)
            {
                strncpy(tmp_str,"Audigy 2 ZS Mobile SB0530",KX_MAX_STRING); // audigy2 ZS Notebook
                is_zsnb=1;
                is_cardbus=1;
                lack_ac97=1;
                busName=CardBus(3);
                busName=NoteBook;
            }
            else // [90]
            {
                strncpy(tmp_str,"Audigy 2 ZS Platium Pro SB0360",KX_MAX_STRING); // audigy2 ZS Platinum Pro + sb290
                is_a2ex=1;
            }
            break;
        case 0x20211102: // [105]
            if(is_k8)
            {
                strncpy(tmp_str,"Audigy 2 ZS Mobile SB2021",KX_MAX_STRING); // audigy2 zs notebook [?]
                is_zsnb=1;
                is_cardbus=1;
                lack_ac97=1;
                busName=CardBus(3);
                busName=NoteBook;
            }
            break;
        case 0x20021102: // [91]
            strncpy(tmp_str,"Audigy 2 ZS SB0350",KX_MAX_STRING); // audigy2 ZS retail + sb250
            break;
        case 0x20031102: // [92]
            strncpy(tmp_str,"Audigy 2 ZS SB0352",KX_MAX_STRING); // audigy2 ZS ?
            break;
        case 0x20041102: // [95]
            strncpy(tmp_str,"Audigy 2 ZS SB035X",KX_MAX_STRING); // Audigy2 zs sb0350/sb0355 / Gateway OEM
            break;
        case 0x20051102: // [96]
            strncpy(tmp_str,"Audigy 2 ZS SB035X",KX_MAX_STRING); // Audigy2 zs SB0350/359
            break;
        case 0x20061102: // [94]
            strncpy(tmp_str,"Audigy 2 ZS SB035X",KX_MAX_STRING);
            break;
        case 0x20071102: // [93]
            strncpy(tmp_str,"Audigy 4 PRO SB0380",KX_MAX_STRING); // audigy4 PRO
            break;
            
        case 0x10001102: // [103]
            strncpy(tmp_str,"Audigy 2 SB1000",KX_MAX_STRING); // audigy2 value [?]
            break;
        case 0x10011102: // [100]
            strncpy(tmp_str,"Audigy 2 SB0400",KX_MAX_STRING); // audigy2 value
            // has ac97 codec
            break;
        case 0x10211102: // [102]
        {
            strncpy(tmp_str,"Audigy 4 SB0610",KX_MAX_STRING);
            is_a4=1;
            is_10k3 = 1;
            // has ac97
        }
            break;
            
            //FIXME find subsys id for audigy 4 and put them here
            /*
        case 0x10201102:
        {
            strncpy(tmp_str,"Sound Blaster Audigy 4 SB0660 10k2 CA10300",KX_MAX_STRING);
            is_a4=1;
        }*/
            
        case 0x10221102: // [104]
            strncpy(tmp_str,"Audigy 2 SB1022",KX_MAX_STRING); // Audigy2 value ?
            break;
        case 0x10241102:
            strncpy(tmp_str,"Audigy RX SB1550",KX_MAX_STRING); // audigy rx experimental
            // has ac97 codec
            is_10k3 = 1;
            is_10k2 = 1;
            is_a4=1;
            busName=CardBus(1);
            busName=PCIExpress;
            //ret=-1;
            break;
        case 0x11011102: // [106]
            strncpy(tmp_str,"Audigy SA SB1101",KX_MAX_STRING); // Audigy SA ??
            break;
            
        case 0x40021102:
            if(!is_k8)
                strncpy(tmp_str,"EMU E-0404",KX_MAX_STRING); // [111]
            else
                strncpy(tmp_str,"EMU E-0404",KX_MAX_STRING); // [120]
            
            is_edsp=1;
            lack_ac97=1;
            break;
        case 0x40031102:
            if(!is_k8)
                strncpy(tmp_str,"EMU E-1010",KX_MAX_STRING); // [112]
            else
                strncpy(tmp_str,"EMU E-1010",KX_MAX_STRING); // [121]
            
            is_edsp=1;
            lack_ac97=1;
            break;
        case 0x40041102: // [123]
            productName = EMU;
            
            if(is_k8)
            {
                strncpy(tmp_str,"EM8960",KX_MAX_STRING); // EM8960: 1010 PCI
                is_edsp=1;
                lack_ac97=1;
            }
            else
            {
                strncpy(tmp_str,"Un4004",KX_MAX_STRING); // ??
            }
            break;
        case 0x40051102:
            productName = EMU;
            if(is_k8)
            {
                strncpy(tmp_str,"EM8984",KX_MAX_STRING); // EM8984: 0404 PCIe
                is_edsp=1;
                lack_ac97=1;
                busName=CardBus(1);
                busName=PCIExpress;
            }
            else
            {
                strncpy(tmp_str,"Un4005",KX_MAX_STRING); // ??
            }
            break;
        case 0x40071102: // EM8982 - 1010 ('PCIe' series)
        {
            productName = EMU;
            if(is_k8)
            {
                strncpy(tmp_str,"EM8982",KX_MAX_STRING);
                is_edsp=1;
                lack_ac97=1;
            }
            else
                strncpy(tmp_str,"Un4007",KX_MAX_STRING);
        }
            busName=CardBus(1);
            busName=PCIExpress;
            break;
            
        case 0x42011102: // [122]
            productName = EMU;
            strncpy(tmp_str,"EM8950",KX_MAX_STRING); // 1616 CardBus v2
            is_edsp=1;
            lack_ac97=1;
            is_cardbus=1;
            break;
            
        default:
            productName = Generic;
            strncpy(tmp_str,"Generic card",KX_MAX_STRING);
            busName=CardBus(4);
            busName=other;
            ret=-1;
            break;
    }
    
    char tmp[KX_MAX_STRING];
    
    char product[KX_MAX_STRING];
    
    
    switch (productName) {
        case SoundBlaster:
            strncpy(product, "SB ", KX_MAX_STRING);
            break;
        case EMU:
            strncpy(product, "E-MU ", KX_MAX_STRING);
            break;
        default:
            strncpy(product, "KX Audio Driver ", KX_MAX_STRING);
            break;
    }
    
    strncpy(tmp, tmp_str, KX_MAX_STRING);
    
    //strncpy(tmp_str, "", KX_MAX_STRING);
    
    strncpy(tmp_str, product, KX_MAX_STRING);
    strncat(tmp_str, tmp, KX_MAX_STRING);
    
    return ret;
}


//usefoul to make simpler to implement get friendly name function
int kx_get_friendly_name_from_kx_object(kx_hw &hw, char &tmp_str);

int kx_get_friendly_name_from_kx_object(kx_hw &hw, char &tmp_str){
    
    return kx_get_friendly_name(hw.pci_device,hw.pci_subsys,hw.pci_chiprev,&tmp_str,
                                hw.is_51,hw.has_surdac,
                                hw.is_aps,hw.is_10k2,hw.is_10k3,hw.is_a2,hw.is_a2ex,hw.is_k8,hw.is_a4,hw.is_edsp,
                                hw.have_ac97,hw.lack_ac97,hw.is_zsnb,hw.is_cardbus,hw.busName);
};



//usefoul to initialize cards without the name
int kx_get_friendly_name_from_kx_object_no_string(kx_hw &hw);

int kx_get_friendly_name_from_kx_object_no_string(kx_hw &hw){
    char tmp_str[KX_MAX_STRING];
    
    return kx_get_friendly_name(hw.pci_device,hw.pci_subsys,hw.pci_chiprev,tmp_str,
                                hw.is_51,hw.has_surdac,
                                hw.is_aps,hw.is_10k2,hw.is_10k3,hw.is_a2,hw.is_a2ex,hw.is_k8,hw.is_a4,hw.is_edsp,
                                hw.have_ac97,hw.lack_ac97,hw.is_zsnb,hw.is_cardbus,hw.busName);
};
