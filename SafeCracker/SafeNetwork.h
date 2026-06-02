#ifndef SAFE_NETWORK_H
#define SAFE_NETWORK_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>

#include "Config.h"
#include "State.h"

void initializeNetwork();
void updateNetwork();
void startWifiConfigPortal(bool clearCredentials);
void stopWifiConfigPortal();
void setWifiOfflineMode(bool offline);
void startWifiConnection();
bool isWifiConnected();
const char* getWifiStatusLabel();
String getDeviceId();
String getConfiguredWebhookUrl();
String htmlEscape(const String& value);
String jsonEscape(const String& value);
const char* getReplayEventName(int eventType);
void postScoreWebhook(
  int levelNumber,
  const char* levelName,
  unsigned long timeMs,
  int stability,
  int targetsHit,
  int targetCount
);

WebServer wifiConfigServer(80);
Preferences wifiPreferences;

String getDeviceId() {
  uint64_t chipId = ESP.getEfuseMac();
  char idBuffer[18];
  snprintf(idBuffer, sizeof(idBuffer), "SC-%04X", (uint16_t)(chipId & 0xFFFF));
  return String(idBuffer);
}

String getConfiguredWebhookUrl() {
  if (configuredWebhookUrl.length() == 0) {
    return String(SCORE_WEBHOOK_URL);
  }

  return configuredWebhookUrl;
}

String htmlEscape(const String& value) {
  String escaped = value;
  escaped.replace("&", "&amp;");
  escaped.replace("\"", "&quot;");
  escaped.replace("'", "&#39;");
  escaped.replace("<", "&lt;");
  escaped.replace(">", "&gt;");
  return escaped;
}

String jsonEscape(const String& value) {
  String escaped = value;
  escaped.replace("\\", "\\\\");
  escaped.replace("\"", "\\\"");
  escaped.replace("\n", "\\n");
  escaped.replace("\r", "\\r");
  return escaped;
}

const char* getReplayEventName(int eventType) {
  if (eventType == REPLAY_EVENT_START) {
    return "start";
  }

  if (eventType == REPLAY_EVENT_TARGET_HIT) {
    return "target_hit";
  }

  if (eventType == REPLAY_EVENT_FINISH) {
    return "finish";
  }

  return "move";
}

void sendWifiConfigPage() {
  String currentSsid = htmlEscape(configuredSsid);
  String currentWebhookUrl = htmlEscape(getConfiguredWebhookUrl());
  String passwordHint = configuredPassword.length() > 0 ? " placeholder='Leave blank to keep current password'" : "";
  String page = "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>";
  page += "<title>SafeCracker WiFi</title><style>";
  page += "body{margin:0;font-family:system-ui,-apple-system,Segoe UI,sans-serif;background:#111827;color:#f9fafb;display:grid;min-height:100vh;place-items:center}";
  page += "main{width:min(420px,calc(100vw - 32px));background:#1f2937;border:1px solid #374151;border-radius:8px;padding:24px;box-shadow:0 18px 60px #0008}";
  page += "h1{font-size:22px;margin:0 0 8px}p{color:#cbd5e1;line-height:1.45}label{display:block;margin:16px 0 6px;color:#e5e7eb}";
  page += "input{width:100%;box-sizing:border-box;border:1px solid #4b5563;border-radius:6px;background:#111827;color:#fff;padding:12px;font-size:16px}";
  page += "button,a{display:inline-flex;align-items:center;justify-content:center;border:0;border-radius:6px;padding:12px 14px;margin-top:18px;font-weight:700;text-decoration:none}";
  page += "button{background:#22c55e;color:#052e16;width:100%}a{background:#374151;color:#f9fafb;width:calc(100% - 28px)}";
  page += "</style></head><body><main><h1>SafeCracker WiFi</h1>";
  page += "<p>Odaberi WiFi mrezu i webhook na koji ESP32 salje scoreove. Ako webhook ostane prazan, koristi se default.</p>";
  page += "<form method='post' action='/save'><label>SSID</label><input name='ssid' maxlength='32' value='" + currentSsid + "' required autofocus>";
  page += "<label>Password</label><input name='password' type='password' maxlength='64'" + passwordHint + ">";
  page += "<label>Webhook URL</label><input name='webhook' maxlength='180' value='" + currentWebhookUrl + "'>";
  page += "<button type='submit'>Spoji se</button></form><a href='/offline'>Igraj offline</a>";
  page += "</main></body></html>";
  wifiConfigServer.send(200, "text/html", page);
}

void handleWifiSave() {
  String ssid = wifiConfigServer.arg("ssid");
  String password = wifiConfigServer.arg("password");
  String webhookUrl = wifiConfigServer.arg("webhook");
  ssid.trim();
  webhookUrl.trim();

  if (ssid.length() == 0) {
    wifiConfigServer.send(400, "text/plain", "SSID is required");
    return;
  }

  if (webhookUrl.length() == 0) {
    webhookUrl = SCORE_WEBHOOK_URL;
  }

  if (!webhookUrl.startsWith("http://") && !webhookUrl.startsWith("https://")) {
    wifiConfigServer.send(400, "text/plain", "Webhook URL must start with http:// or https://");
    return;
  }

  if (password.length() == 0 && ssid == configuredSsid && configuredPassword.length() > 0) {
    password = configuredPassword;
  }

  wifiPreferences.putString("ssid", ssid);
  wifiPreferences.putString("password", password);
  wifiPreferences.putString("webhook", webhookUrl);
  wifiPreferences.putBool("offline", false);
  configuredSsid = ssid;
  configuredPassword = password;
  configuredWebhookUrl = webhookUrl;
  wifiHasCredentials = true;
  wifiOfflineMode = false;

  wifiConfigServer.send(200, "text/html", "<!doctype html><html><body><h1>Saved</h1><p>SafeCracker se spaja na WiFi. Mozes zatvoriti ovu stranicu.</p></body></html>");
  stopWifiConfigPortal();
  startWifiConnection();
  gameScreen = GAME_SCREEN_LEVEL_SELECT;
}

void handleWifiOffline() {
  wifiConfigServer.send(200, "text/html", "<!doctype html><html><body><h1>Offline mode</h1><p>SafeCracker nece slati scoreove dok opet ne pokrenes WiFi setup.</p></body></html>");
  setWifiOfflineMode(true);
  gameScreen = GAME_SCREEN_LEVEL_SELECT;
}

void initializeNetwork() {
  wifiPreferences.begin("safe-wifi", false);
  configuredSsid = wifiPreferences.getString("ssid", "");
  configuredPassword = wifiPreferences.getString("password", "");
  configuredWebhookUrl = wifiPreferences.getString("webhook", SCORE_WEBHOOK_URL);
  if (configuredWebhookUrl.length() == 0) {
    configuredWebhookUrl = SCORE_WEBHOOK_URL;
  }
  wifiOfflineMode = wifiPreferences.getBool("offline", false);
  wifiHasCredentials = configuredSsid.length() > 0;

  if (wifiOfflineMode) {
    networkStatusMessage = "Offline";
    WiFi.mode(WIFI_OFF);
    return;
  }

  if (wifiHasCredentials) {
    startWifiConnection();
    return;
  }

  startWifiConfigPortal(false);
}

void startWifiConnection() {
  if (configuredSsid.length() == 0) {
    startWifiConfigPortal(false);
    return;
  }

  stopWifiConfigPortal();
  WiFi.mode(WIFI_STA);
  WiFi.begin(configuredSsid.c_str(), configuredPassword.c_str());
  wifiConnectStartedAt = millis();
  wifiConnectInProgress = true;
  networkStatusMessage = "Connecting " + configuredSsid;
}

void startWifiConfigPortal(bool clearCredentials) {
  if (clearCredentials) {
    wifiPreferences.remove("ssid");
    wifiPreferences.remove("password");
    configuredSsid = "";
    configuredPassword = "";
    wifiHasCredentials = false;
    WiFi.disconnect(true, true);
  }

  wifiPreferences.putBool("offline", false);
  wifiOfflineMode = false;
  wifiConnectInProgress = false;

  uint64_t chipId = ESP.getEfuseMac();
  char ssidBuffer[24];
  snprintf(ssidBuffer, sizeof(ssidBuffer), "SafeCracker-%04X", (uint16_t)(chipId & 0xFFFF));
  wifiPortalSsid = String(ssidBuffer);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(wifiPortalSsid.c_str(), WIFI_PORTAL_PASSWORD);

  wifiConfigServer.on("/", HTTP_GET, sendWifiConfigPage);
  wifiConfigServer.on("/save", HTTP_POST, handleWifiSave);
  wifiConfigServer.on("/offline", HTTP_GET, handleWifiOffline);
  wifiConfigServer.begin();

  wifiConfigPortalActive = true;
  networkStatusMessage = "Setup AP " + wifiPortalSsid;
  gameScreen = GAME_SCREEN_WIFI_CONFIG;
}

void stopWifiConfigPortal() {
  if (!wifiConfigPortalActive) {
    return;
  }

  wifiConfigServer.stop();
  WiFi.softAPdisconnect(true);
  wifiConfigPortalActive = false;
}

void setWifiOfflineMode(bool offline) {
  wifiPreferences.putBool("offline", offline);
  wifiOfflineMode = offline;

  if (!offline) {
    if (wifiHasCredentials) {
      startWifiConnection();
    } else {
      startWifiConfigPortal(false);
    }
    return;
  }

  stopWifiConfigPortal();
  wifiConnectInProgress = false;
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);
  networkStatusMessage = "Offline";
}

void updateNetwork() {
  if (wifiConfigPortalActive) {
    wifiConfigServer.handleClient();
  }

  if (!wifiConnectInProgress) {
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnectInProgress = false;
    networkStatusMessage = "WiFi " + WiFi.localIP().toString();
    return;
  }

  if (millis() - wifiConnectStartedAt >= WIFI_CONNECT_TIMEOUT_MS) {
    wifiConnectInProgress = false;
    networkStatusMessage = "WiFi failed";
    Serial.println("WiFi connection failed. Starting setup portal.");
    WiFi.disconnect(false, false);
    startWifiConfigPortal(false);
  }
}

bool isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

const char* getWifiStatusLabel() {
  if (wifiOfflineMode) {
    return "Offline";
  }

  if (wifiConfigPortalActive) {
    return "WiFi setup";
  }

  if (isWifiConnected()) {
    return "Online";
  }

  if (wifiConnectInProgress) {
    return "Connecting";
  }

  return "No WiFi";
}

void postScoreWebhook(
  int levelNumber,
  const char* levelName,
  unsigned long timeMs,
  int stability,
  int targetsHit,
  int targetCount
) {
  if (wifiOfflineMode || !isWifiConnected()) {
    Serial.println("Score skipped: WiFi offline or disconnected");
    return;
  }

  HTTPClient http;
  http.setTimeout(SCORE_POST_TIMEOUT_MS);

  String webhookUrl = getConfiguredWebhookUrl();
  WiFiClient plainClient;
  WiFiClientSecure secureClient;
  bool didBegin = false;

  if (webhookUrl.startsWith("https://")) {
    secureClient.setInsecure();
    didBegin = http.begin(secureClient, webhookUrl);
  } else {
    didBegin = http.begin(plainClient, webhookUrl);
  }

  if (!didBegin) {
    Serial.println("Score post failed: invalid URL");
    return;
  }

  String payload = "{";
  payload += "\"device_id\":\"" + getDeviceId() + "\",";
  payload += "\"level\":" + String(levelNumber) + ",";
  payload += "\"level_name\":\"" + jsonEscape(String(levelName)) + "\",";
  payload += "\"time_ms\":" + String(timeMs) + ",";
  payload += "\"stability\":" + String(stability) + ",";
  payload += "\"targets_hit\":" + String(targetsHit) + ",";
  payload += "\"target_count\":" + String(targetCount) + ",";

  payload += "\"targets\":[";
  for (int i = 0; i < currentRunTargetCount; i++) {
    if (i > 0) {
      payload += ",";
    }
    payload += String(currentRunTargets[i], 3);
  }
  payload += "],";

  payload += "\"target_stats\":[";
  for (int i = 0; i < currentRunTargetCount; i++) {
    if (i > 0) {
      payload += ",";
    }
    payload += "{";
    payload += "\"index\":" + String(i) + ",";
    payload += "\"target\":" + String(currentRunTargets[i], 3) + ",";
    payload += "\"stability\":" + String(targetStabilityScore[i]) + ",";
    payload += "\"samples\":" + String(targetStabilitySampleCount[i]) + ",";
    payload += "\"overshoots\":" + String(targetStabilityOvershootCount[i]) + ",";
    payload += "\"direction_changes\":" + String(targetStabilityDirectionChanges[i]);
    payload += "}";
  }
  payload += "],";

  payload += "\"replay\":[";
  for (int i = 0; i < replayEventCount; i++) {
    if (i > 0) {
      payload += ",";
    }
    payload += "{";
    payload += "\"t\":" + String(replayEventTimeMs[i]) + ",";
    payload += "\"fill\":" + String(replayEventFill[i], 3) + ",";
    payload += "\"target_index\":" + String(replayEventTargetIndex[i]) + ",";
    payload += "\"event\":\"" + String(getReplayEventName(replayEventType[i])) + "\"";
    payload += "}";
  }
  payload += "]";
  payload += "}";

  http.addHeader("Content-Type", "application/json");
  int responseCode = http.POST(payload);
  Serial.print("Score post response: ");
  Serial.println(responseCode);
  http.end();
}

#endif
