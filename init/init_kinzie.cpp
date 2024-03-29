/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <android-base/properties.h>

#include <string.h>
#include <stdlib.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "property_service.h"

#include "vendor_init.h"

using android::init::property_set;

static void setSsim(void);
static void setMsim(void);
static void setLatMsim(void);

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void vendor_load_properties()
{
    std::string platform;
    std::string sku;
    std::string car;

    sku = android::base::GetProperty("ro.boot.hardware.sku", "");
    car = android::base::GetProperty("ro.boot.carrier", "");

    property_override("ro.product.model", sku.c_str());
    property_override("ro.build.product", "kinzie");
    property_override("ro.product.device", "kinzie");

    if (sku == "XT1580") {
	property_override("ro.product.name", "Moto X Force");
        property_set("ro.product.display", "Moto X Force");
        property_set("ro.telephony.default_network", "9,10");
        property_set("telephony.lteOnCdmaDevice", "0");
        // Region specifics
        if ((car == "reteu") || (car == "retgb")) {
            // EU
            setSsim();
            property_override("ro.build.description", "kinzie_reteu-user 6.0 MPK24.78-8 10 release-keys");
            property_override("ro.build.fingerprint", "motorola/kinzie_reteu/kinzie:6.0/MPK24.78-8/10:user/release-keys");
            property_set("ro.fsg-id", "emea");
        } else if ((car == "retbr") || (car == "retla") || (car == "tefbr") || (car == "timbr")) {
            /* Brazil -- LATAM*/
            setLatMsim();
            property_set("ro.build.version.full","Blur_Version.24.11.8.kinzie_retla_ds.retla.en.01");
            property_override("ro.build.description", "kinzie_retla_ds-user 6.0 MPK24.78-8 9 release-keys");
            property_override("ro.build.fingerprint", "motorola/kinzie_retla_ds/kinzie_uds:6.0/MPK24.78-8/9:user/release-keys");
        } else if (car == "retmx") {
            /* Mexico */
            setLatMsim();
            property_set("ro.build.version.full","Blur_Version.24.11.8.kinzie_retmx_ds.retla.en.01");
            property_override("ro.build.description", "kinzie_retmx_ds-user 6.0 MPK24.78-8 8 release-keys");
            property_override("ro.build.fingerprint", "motorola/kinzie_retmx_ds/kinzie_uds:6.0/MPK24.78-8/8:user/release-keys");
        } else {
            //default to EU
            setSsim();
            property_override("ro.build.description", "kinzie_reteu-user 6.0 MPK24.78-8 10 release-keys");
            property_override("ro.build.fingerprint", "motorola/kinzie_reteu/kinzie:6.0/MPK24.78-8/10:user/release-keys");
        }
    } else if ((sku == "XT1585") || (sku == "MOTXT1585")) {
        /* US */
        setSsim();
	property_override("ro.product.name", "Droid Turbo 2");
        property_set("ro.product.display", "Droid Turbo 2");
        property_set("ro.ril.force_eri_from_xml", "true");
        property_set("ro.telephony.get_imsi_from_sim", "true");
        property_set("ro.telephony.default_network", "10,10");
        property_set("telephony.lteOnCdmaDevice", "1");
        property_override("ro.build.description", "kinzie_verizon-user 6.0 MCK24.78-13.11 11 release-keys");
        property_override("ro.build.fingerprint", "motorola/kinzie_verizon/kinzie:6.0/MCK24.78-13.11/11:user/release-keys");
    } else if (sku == "XT1581") {
        /* China */
        setMsim();
	property_override("ro.product.name", "Moto X 極");
	property_set("ro.product.display", "Moto X Force");
        property_set("ro.telephony.default_network", "22,20");
        property_set("telephony.lteOnCdmaDevice", "1");
        property_set("persist.radio.mcfg_enabled", "1");
        property_set("persist.radio.force_on_dc", "true");
        property_set("persist.radio.rat_on", "combine");
        property_set("persist.radio.disable_flexmap", "0");
        property_set("gsm.sim.min.match", "8");
        property_set("ro.cdma.subscription", "0");
        property_set("ro.telephony.default_cdma_sub", "0");
        property_set("ril.subscription.types", "RUIM");
        property_set("persist.radio.force_get_pref", "1");
        property_override("ro.build.description", "kinzie_retcn_ds-user 5.1.1 LPK23.229-1 1 release-keys");
        property_override("ro.build.fingerprint", "motorola/kinzie_retcn_ds/kinzie_uds:5.1.1/LPK23.229-1/1:user/release-keys");
    }
}
static void setSsim(void)
{
    property_set("ro.media.enc.aud.fileformat", "qcp");
    property_set("ro.media.enc.aud.codec", "qcelp");
    property_set("ro.media.enc.aud.bps", "13300");
    property_set("ro.media.enc.aud.ch", "1");
    property_set("ro.media.enc.aud.hz", "8000");
    property_set("persist.rmnet.mux", "enabled");
    property_set("persist.sys.cnd.iwlan", "0");
    property_set("persist.cne.logging.qxdm", "0");
    property_set("persist.vt.supported", "0");
    property_set("persist.eab.supported", "0");
    property_set("persist.radio.snapshot_timer", "22");
    property_set("persist.radio.snapshot_enabled", "1");
    property_set("persist.ims.volte", "true");
    property_set("persist.ims.vt", "false");
    property_set("persist.ims.vt.epdg", "false");
    property_set("persist.ims.disableADBLogs", "2");
    property_set("persist.ims.disableDebugLogs", "0");
    property_set("persist.ims.disableQXDMLogs", "0");
    property_set("persist.ims.disableIMSLogs", "0");
    property_set("persist.rcs.supported", "0");
    property_set("persist.rcs.presence.provision", "0");
    property_set("persist.radio.calls.on.ims", "true");
    property_set("persist.radio.jbims", "1");
    property_set("persist.radio.domain.ps", "0");
    property_set("persist.radio.VT_ENABLE", "1");
    property_set("persist.radio.VT_HYBRID_ENABLE", "1");
    property_set("persist.radio.ROTATION_ENABLE", "1");
    property_set("persist.radio.REVERSE_QMI", "0");
    property_set("persist.radio.RATE_ADAPT_ENABLE", "1");
    property_set("persist.radio.VT_USE_MDM_TIME", "0");
    property_set("persist.radio.videopause.mode", "0");
    property_set("persist.data.iwlan.enable", "false");
    property_set("persist.radio.mcfg_enabled", "1");
    property_set("ro.mot.ignore_csim_appid", "true");
    property_set("persist.data.netmgrd.qos.enable", "true");
    property_set("ril.subscription.types", "RUIM");
}

static void setLatMsim(void)
{
    property_set("ro.gsm.data_retry_config","default_randomization=2000,max_retries=infinite,1000,1000,80000,125000,485000,905000");
    property_set("persist.radio.process_sups_ind","1");
    property_set("persist.radio.all_bc_msg","all");
    property_set("persist.radio.mot_ecc_enabled","1");
    property_set("persist.radio.force_get_pref","1");
    setMsim();
}

static void setMsim(void)
{
    property_set("persist.radio.multisim.config", "dsds");
    property_set("persist.radio.plmn_name_cmp", "1");
    property_set("ro.telephony.ril.config", "simactivation");
    property_set("ro.fsg-id", "emea_dsds");
    property_set("ro.media.enc.aud.fileformat", "amr");
    property_set("ro.media.enc.aud.codec", "amrnb");
    property_set("ril.subscription.types", "NV,RUIM");
    property_set("persist.ims.volte", "true");
    property_set("persist.ims.vt", "false");
    property_set("persist.ims.vt.epdg", "false");
    property_set("persist.ims.rcs", "false");
    property_set("ro.radio.imei.sv", "4");
    property_set("persist.radio.call.audio.output", "0");
    property_set("persist.radio.videopause.mode", "0");
    property_set("persist.vt.supported", "0");
    property_set("persist.eab.supported", "0");
    property_set("persist.rcs.supported", "0");
    property_set("persist.rcs.presence.provision", "0");
}
