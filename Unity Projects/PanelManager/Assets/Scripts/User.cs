public struct User {
    public string Name;
    public float MaxBrightness;
    public float MinBrightness;
    public float MaxVolume;
    public float MinVolume;

    public User(string Name, float MaxBrightness, float MinBrightness, float MaxVolume, float MinVolume) {
        this.Name = Name;
        this.MaxBrightness = MaxBrightness;
        this.MinBrightness = MinBrightness;
        this.MaxVolume = MaxVolume;
        this.MinVolume = MinVolume;
    }
};