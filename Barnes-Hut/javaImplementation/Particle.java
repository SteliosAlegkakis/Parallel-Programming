package javaImplementation;
public class Particle {
    double x;
    double y;
    double vx;
    double vy;
    double mass;
    String s;
    Particle(double x, double y, double vx, double vy, double mass, String s){
        this.x = x;
        this.y = y;
        this.mass = mass;
        this.vx = vx;
        this.vy = vy;
        this.s = s;
    }

    public String toString(){
        return "x: " + x + " y: " + y + " vx: " + vx + " vy: " + vy + " mass: " + mass + " s: " + s;
    }
}
