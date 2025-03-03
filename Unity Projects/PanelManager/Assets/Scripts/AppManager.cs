using UnityEngine;
using System.Collections.Generic;

public class AppManager : MonoBehaviour {
    public static AppManager Instance { get; private set; }
    List<User> users = new();

    void Awake() {
        if (Instance != null && Instance != this)
            Destroy(gameObject);
        else
            Instance = this;
    }

    void Start() {
        BluetoothManager.Instance.EstablishConnection();
    }
}
