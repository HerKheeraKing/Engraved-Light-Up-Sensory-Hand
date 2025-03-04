using UnityEngine;
using ArduinoBluetoothAPI;
using System;
using System.Linq;
using System.Collections;

public class BluetoothManager : MonoBehaviour {
    BluetoothHelper helper;
    readonly private string deviceName = "PanelMaster";
    public static BluetoothManager Instance { get; private set; }
    public bool connectionMade = false;

    void Awake() {
        if (Instance != null && Instance != this)
            Destroy(gameObject);
        else
            Instance = this;
    }

    public void EstablishConnection() {
        try {
            helper = BluetoothHelper.GetInstance(deviceName);
            helper.OnConnected += OnConnected;
            helper.OnConnectionFailed += OnConnectionFailed;
        }
        catch (Exception ex) { 
            print("Failed: " + ex);
        }
    }

    void OnConnected(BluetoothHelper helper) { 
        helper.StartListening();
        UserManager.Instance.Initialize();
        connectionMade = true;
        UserData user;
        if (IsDefaultUser()) {
            user = UserManager.Instance.users.allUsers[0];
            SetUser(user);
        }
        else
            user = GetUserOnArduino();
        AppManager.Instance.curUser = user;
        AppManager.Instance.curUserText.text = user.Name;
    }

    void OnConnectionFailed(BluetoothHelper helper) { print("Connection failed"); }

    void OnDestroy() { helper?.Disconnect(); }

    public bool SendData(string msg) {
        if (helper == null) 
            return false; 
        helper.SendData(msg);
        return true;
    }

    public bool IsDefaultUser() {
        SendData("IsSet");
        string response = helper.Read();
        return response == "NULL";
    }

    public void SetUser(UserData user) { SendData("USER" + user.FormatArgsForArduino()); }

    UserData GetUserOnArduino() {
        SendData("USER:NAME");
        string response = helper.Read();
        return UserManager.Instance.users.allUsers.FirstOrDefault(x => x.Name == response);
    }
}