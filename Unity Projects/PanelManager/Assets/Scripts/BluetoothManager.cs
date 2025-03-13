using UnityEngine;
using ArduinoBluetoothAPI;
using System;
using System.Collections;

public class BluetoothManager : MonoBehaviour {
    BluetoothHelper helper;
    public static BluetoothManager Instance { get; private set; }
    public bool connectionMade = false;
    bool isDefaultUser = true;
    UserData curArduinoUser = new();
    float lastHeartbeat = 0.0f;
    readonly static float HEARTBEAT_TIMEOUT = 4.0f;

    void Awake() {
        if (Instance != null && Instance != this)
            Destroy(gameObject);
        else
            Instance = this;
    }

    void Update() {
        if (!UserManager.Instance.initialized || helper == null)
            return;

        if (helper.isConnected()) {
            string res = helper.Read();
            if (res.StartsWith("DefaultUser:"))
                isDefaultUser = res[^2] == '1';
            else if (res.StartsWith("USER:"))
                curArduinoUser = UserManager.Instance.users.FindUser(res[5..^1]);
            else if (res.StartsWith("Heartbeat"))
                lastHeartbeat = Time.time;
        }
        if (Time.time - lastHeartbeat >= HEARTBEAT_TIMEOUT && connectionMade)
            Disconnect();
    }

    public void EstablishConnection(string deviceName = "PanelMaster") {
        try {
            helper = BluetoothHelper.GetInstance(deviceName);
            helper.OnConnected += OnConnected;
            helper.OnConnectionFailed += OnConnectionFailed;
            helper.setTerminatorBasedStream("\n");
            helper.Connect();
        }
        catch (Exception ex) { 
            print("Failed: " + ex);
        }
    }

    void OnConnected(BluetoothHelper helper) {
        SendData("Init");
        helper.StartListening();
        UserManager.Instance.Initialize();
        StartCoroutine(DelayedConnected());
    }

    IEnumerator DelayedConnected() {
        yield return new WaitForSeconds(3.0f);
        UserData user = isDefaultUser ? UserManager.Instance.users.allUsers[0] : curArduinoUser;
        if (isDefaultUser)
            SetUser(user);
        AppManager.Instance.curUser = user;
        AppManager.Instance.curUserText.text = user.Name;
        connectionMade = true;
    }

    void Disconnect() {
        helper.Disconnect();
        connectionMade = false;
        AppManager.Instance.DisconnectMenuScreen();
    }

    void OnConnectionFailed(BluetoothHelper helper) { print("Connection failed"); }

    void OnDestroy() { helper?.Disconnect(); }

    public bool SendData(string msg) {
        if (helper == null) 
            return false; 
        helper.SendData(msg);
        return true;
    }

    public void SetUser(UserData user) { SendData("USER:" + user.FormatArgsForArduino()); }

    public void SetUserVariable(SettingTypes type, float value) { SendData("SETV:" + type.ToString() + "=" + value); }
}