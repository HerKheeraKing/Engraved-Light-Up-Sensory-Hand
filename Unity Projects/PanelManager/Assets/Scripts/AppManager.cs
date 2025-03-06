using UnityEngine;
using System.Collections;
using TMPro;
using UnityEngine.UI;
using UnityEngine.Android;

public class AppManager : MonoBehaviour {
    public static AppManager Instance { get; private set; }
    [SerializeField] float timeoutDelay;
    [SerializeField] GameObject connectionScreen, MainScreen, userSetting, adjustingUsers, addUser, arduinoNameScreen;
    [SerializeField] TMP_Text conText, userSettingHeader;
    [SerializeField] TMP_InputField maxBright, minBright, maxVol, minVol, newUser, arduinoName;
    [SerializeField] Transform content;
    [SerializeField] Color normalColor = Color.white;
    [SerializeField] Color highlightColor = new(0x00 / 255f, 0xFF / 255f, 0x67 / 255f);
    GameObject selectedRow;
    UserData selectedUser;
    public TMP_Text curUserText;
    public UserData curUser;

    void Awake() {
        if (Instance != null && Instance != this)
            Destroy(gameObject);
        else
            Instance = this;

        if (Application.platform == RuntimePlatform.Android) {
            if (!Permission.HasUserAuthorizedPermission("android.permission.ACCESS_FINE_LOCATION")
                  || !Permission.HasUserAuthorizedPermission("android.permission.ACCESS_COARSE_LOCATION")
                  || !Permission.HasUserAuthorizedPermission("android.permission.BLUETOOTH_SCAN")
                  || !Permission.HasUserAuthorizedPermission("android.permission.BLUETOOTH_CONNECT")) {
                Permission.RequestUserPermissions(new string[] {Permission.CoarseLocation,
                                                                Permission.FineLocation,
                                                                "android.permission.BLUETOOTH_SCAN",
                                                                "android.permission.BLUETOOTH_CONNECT",
                                                                "BLUETOOTH_ADVERTISE"});
            }
        }

    }
    IEnumerator WaitForConnection() {
        int dotCount = 0;
        string baseText = conText.text.Split('\n')[0];
        float timeStarted = Time.time;
        while (!BluetoothManager.Instance.connectionMade) {
            if (Time.time - timeStarted > timeoutDelay) {
                conText.text = "Connection Timed out.\nPlease restart the app and try again.";        
                break;
            }
            conText.text = baseText + new string('.', dotCount);
            dotCount = (dotCount + 1) % 4;
            yield return new WaitForSeconds(0.5f);
        }
        if (BluetoothManager.Instance.connectionMade) {
            connectionScreen.SetActive(false);
            MainScreen.SetActive(true);
        }
    }

    public void ArduioNameButton() {
        BluetoothManager.Instance.EstablishConnection(arduinoName.text);
        arduinoNameScreen.SetActive(false);
        connectionScreen.SetActive(true);
        StartCoroutine(WaitForConnection());
    }

    public void UserSettingsButton() {
        userSettingHeader.text = curUser.Name + "'s Settings";
        maxBright.text = curUser.MaxBrightness.ToString();
        minBright.text = curUser.MinBrightness.ToString();
        maxVol.text = curUser.MaxVolume.ToString();
        minVol.text = curUser.MinVolume.ToString();
        MainScreen.SetActive(false);
        userSetting.SetActive(true);
    }
    
    public void AdjustingUsers() {
        MainScreen.SetActive(false);
        adjustingUsers.SetActive(true);
        foreach (Transform child in content)
            Destroy(child.gameObject);
        foreach (var user in UserManager.Instance.users.allUsers) {
            GameObject row = new("Row_" + user.Name) { transform = { parent = content, localScale = Vector3.one } };
            row.AddComponent<Image>().color = normalColor;
            row.GetComponent<RectTransform>().sizeDelta = new Vector2(800, 50);
            TextMeshProUGUI text = new GameObject("Text").AddComponent<TextMeshProUGUI>();
            text.transform.SetParent(row.transform, false);
            text.text = user.Name;
            text.alignment = TextAlignmentOptions.Center;
            text.color = Color.black;
            text.fontSize = 20;
            row.AddComponent<Button>().onClick.AddListener(() => SelectRow(row, user));
        }
    }

    void SelectRow(GameObject row, UserData user) {
        if (selectedRow != null)
            selectedRow.GetComponent<Image>().color = normalColor;
        selectedRow = row;
        selectedUser = user;
        selectedRow.GetComponent<Image>().color = highlightColor;
    }

    public void BackButton() {
        adjustingUsers.SetActive(false);
        userSetting.SetActive(false);
        addUser.SetActive(false);
        MainScreen.SetActive(true);
        selectedUser = new();
        selectedRow = null;
    }

    public void AddUserButton() {
        MainScreen.SetActive(false);
        adjustingUsers.SetActive(false);
        addUser.SetActive(true);
    }

    public void ConfirmUserButton() {
        UserManager.Instance.AddUser(new UserData(newUser.text));
        addUser.SetActive(false);
        AdjustingUsers();
    }

    public void RemoveUserButton() {
        UserManager.Instance.RemoveUser(selectedUser);
        if (selectedUser.Name == curUser.Name) {
            curUser = UserManager.Instance.users.allUsers[0];
            BluetoothManager.Instance.SetUser(curUser);
            curUserText.text = curUser.Name;
        }
        selectedUser = new();
        AdjustingUsers();
    }

    public void SelectUserButton() {
        curUser = selectedUser;
        BluetoothManager.Instance.SetUser(curUser);
        curUserText.text = curUser.Name;
    }

    public void MaxBrightnessChanged(string val) {
        if (float.TryParse(val, out float newVal) && newVal <= 100 && newVal > curUser.MinBrightness) {
            curUser.MaxBrightness = newVal;
            UserManager.Instance.SaveUser(curUser);
            BluetoothManager.Instance.SetUserVariable(SettingTypes.MaxBright, newVal);
        }
        else
            maxBright.text = curUser.MaxBrightness.ToString();
    }

    public void MinBrightnessChanged(string val) {
        if (float.TryParse(val, out float newVal) && newVal >= 0 && newVal < curUser.MaxBrightness) {
            curUser.MinBrightness = newVal;
            UserManager.Instance.SaveUser(curUser);
            BluetoothManager.Instance.SetUserVariable(SettingTypes.MinBright, newVal);
        }
        else
            minBright.text = curUser.MinBrightness.ToString();
    }

    public void MaxVolumeChanged(string val) {
        if (float.TryParse(val, out float newVal) && newVal <= 100 && newVal > curUser.MinVolume) {
            curUser.MaxVolume = newVal;
            UserManager.Instance.SaveUser(curUser);
            BluetoothManager.Instance.SetUserVariable(SettingTypes.MaxVol, newVal);
        }
        else
            maxVol.text = curUser.MaxVolume.ToString();
    }

    public void MinVolumeChanged(string val) {
        if (float.TryParse(val, out float newVal) && newVal >= 0 && newVal < curUser.MaxVolume) {
            curUser.MinVolume = newVal;
            UserManager.Instance.SaveUser(curUser);
            BluetoothManager.Instance.SetUserVariable(SettingTypes.MinVol, newVal);
        }
        else
            minVol.text = curUser.MinVolume.ToString();
    }
}