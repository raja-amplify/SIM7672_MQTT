/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
// This file contains your configuration used to connect to Cloud IoT Core

// Wifi network details.
//const char *ssid = "eyedentify";
//const char *password = "Tiger123";

/*
// Cloud iot details Niels.
const char *project_id = "akstotal-v2";
const char *location = "europe-west1";
const char *registry_id = "Air-Quality";
const char *device_id = "ESP_AQ_NEW";
const char *sub = "air-quality-data";
*/


// Cloud iot details Niels 2.
const char *project_id = "akstotal-v2";
const char *location = "europe-west1";
const char *registry_id = "Air-Quality";
const char *device_id = "ESP_AQ_001";
const char *sub = "air-quality-data";


//
/*
// Cloud iot details Amplify's.
  const char *project_id = "evre-iot-308216";
  const char *location = "europe-west1";
  const char *registry_id = "air-quality";
  const char *device_id = "ESP32NEW";
*/

/*
// Cloud iot details Amplify's_ESP32_NEW_2.
  const char *project_id = "evre-iot-308216";
  const char *location = "europe-west1";
  const char *registry_id = "air-quality";
  const char *device_id = "ESP32_NEW_2";
*/

// Configuration for NTP
const char* ntp_primary = "pool.ntp.org";
const char* ntp_secondary = "time.nist.gov";

//#ifndef LED_BUILTIN
//#define LED_BUILTIN 16
//#endif

// To get the private key run (where private-key.pem is the ec private key
// used to create the certificate uploaded to google cloud iot):
// openssl ec -in <private-key.pem> -noout -text
// and copy priv: part.
// The key length should be exactly the same as the key length bellow (32 pairs
// of hex digits). If it's bigger and it starts with "00:" delete the "00:". If
// it's smaller add "00:" to the start. If it's too big or too small something



// Amplify's-Niels
// is probably wrong with your key.
const char *private_key_str =
    "72:3d:7f:28:fa:33:5b:8f:d8:74:4f:af:dc:3f:d7:"
    "dd:d8:9f:aa:b9:fa:a6:42:d8:ab:35:e2:32:c3:c0:"
    "1a:61";


/*
const char *private_key_str =
    "b8:9e:77:bc:4d:68:2e:9e:1f:40:44:c7:03:ef:a1:"
    "89:75:a2:e3:70:2f:05:b1:a8:1f:8b:0d:b1:13:b9:"
    "e4:6e";
*/



// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 3600*24; 				// Maximum 24H (3600*24)

// To get the certificate for your region run:
//   openssl s_client -showcerts -connect mqtt.googleapis.com:8883
// for standard mqtt or for LTS:
//   openssl s_client -showcerts -connect mqtt.2030.ltsapis.goog:8883
// Copy the certificate (all lines between and including ---BEGIN CERTIFICATE---
// and --END CERTIFICATE--) to root.cert and put here on the root_cert variable.
/*
const char *root_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIErjCCA5agAwIBAgIQW+5sTJWKajts11BgHkBRwjANBgkqhkiG9w0BAQsFADBU\n"
    "MQswCQYDVQQGEwJVUzEeMBwGA1UEChMVR29vZ2xlIFRydXN0IFNlcnZpY2VzMSUw\n"
    "IwYDVQQDExxHb29nbGUgSW50ZXJuZXQgQXV0aG9yaXR5IEczMB4XDTE5MDYxMTEy\n"
    "MzE1OVoXDTE5MDkwMzEyMjAwMFowbTELMAkGA1UEBhMCVVMxEzARBgNVBAgMCkNh\n"
    "bGlmb3JuaWExFjAUBgNVBAcMDU1vdW50YWluIFZpZXcxEzARBgNVBAoMCkdvb2ds\n"
    "ZSBMTEMxHDAaBgNVBAMME21xdHQuZ29vZ2xlYXBpcy5jb20wggEiMA0GCSqGSIb3\n"
    "DQEBAQUAA4IBDwAwggEKAoIBAQDHuQUoDZWl2155WvaQ9AmhTRNC+mHassokdQK7\n"
    "NxkZVZfrS8EhRkZop6SJGHdvozBP3Ko3g1MgGIZFzqb5fRohkRKB6mteHHi/W7Uo\n"
    "7d8+wuTTz3llUZ2gHF/hrXFJfztwnaZub/KB+fXwSqWgMyo1EBme4ULV0rQZGFu6\n"
    "7U38HK+mFRbeJkh1SDOureI2dxkC4ACGiqWfX/vSyzpZkWGRuxK2F5cnBHqRbcKs\n"
    "OfmYyUuxZjGah+fC5ePgDbAntLUuYNppkdgT8yt/13ae/V7+rRhKOZC4q76HBEaQ\n"
    "4Wn5UC+ShVaAGuo7BtfoIFSyZi8/DU2eTQcHWewIXU6V5InhAgMBAAGjggFhMIIB\n"
    "XTATBgNVHSUEDDAKBggrBgEFBQcDATA4BgNVHREEMTAvghNtcXR0Lmdvb2dsZWFw\n"
    "aXMuY29tghhtcXR0LW10bHMuZ29vZ2xlYXBpcy5jb20waAYIKwYBBQUHAQEEXDBa\n"
    "MC0GCCsGAQUFBzAChiFodHRwOi8vcGtpLmdvb2cvZ3NyMi9HVFNHSUFHMy5jcnQw\n"
    "KQYIKwYBBQUHMAGGHWh0dHA6Ly9vY3NwLnBraS5nb29nL0dUU0dJQUczMB0GA1Ud\n"
    "DgQWBBSKWpFfG/yH1dkkJT05y/ZnRm/M4DAMBgNVHRMBAf8EAjAAMB8GA1UdIwQY\n"
    "MBaAFHfCuFCaZ3Z2sS3ChtCDoH6mfrpLMCEGA1UdIAQaMBgwDAYKKwYBBAHWeQIF\n"
    "AzAIBgZngQwBAgIwMQYDVR0fBCowKDAmoCSgIoYgaHR0cDovL2NybC5wa2kuZ29v\n"
    "Zy9HVFNHSUFHMy5jcmwwDQYJKoZIhvcNAQELBQADggEBAKMoXHxmLI1oKnraV0tL\n"
    "NzznlVnle4ljS/pqNI8LUM4/5QqD3qGqnI4fBxX1l+WByCitbTiNvL2KRNi9xau5\n"
    "oqvsuSVkjRQxky2eesjkdrp+rrxTwFhQ6NAbUeZgUV0zfm5XZE76kInbcukwXxAx\n"
    "lneyQy2git0voUWTK4mipfCU946rcK3+ArcanV7EDSXbRxfjBSRBD6K+XGUhIPHW\n"
    "brk0v1wzED1RFEHTdzLAecU50Xwic6IniM3B9URfSOmjlBRebg2sEVQavMHbzURg\n"
    "94aDC+EkNlHh3pOmQ/V89MBiF1xDHbZZ1gB0GszYKPHec9omSwQ5HbIDV3uf3/DQ\n"
    "his=\n"
    "-----END CERTIFICATE-----\n";

*/
const char *root_cert =
"-----BEGIN CERTIFICATE-----\n"
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n"
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n"
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n"
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n"
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n"
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n"
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n"
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n"
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n"
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n"
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n"
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n"
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n"
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n"
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n"
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n"
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n"
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n"
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n"
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n"
"-----END CERTIFICATE-----\n";

// In case we ever need extra topics
const int ex_num_topics = 0;
const char* ex_topics[ex_num_topics];
//const int ex_num_topics = 1;
//const char* ex_topics[ex_num_topics] = {
//  "/devices/my-device/tbd/#"
//};
