#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <cassert>
using namespace std;

class HH_model{
    private:
        //neuron parameters
        double El = 10.6e-3; //mV
        double EK = -12.0e-3; //mV
        double ENa = 115e-3; //mV
        double gl = 0.3e-3;  //msiemens
        double gK = 36e-3;  //msiemens
        double gNa = 120e-3; //msiemens
        double C = 1e-6; //ufarad


        double alpha_h() {
            return 0.07 * exp(-0.05 * v*1e3);
        }

        double alpha_m() {
            return 0.1*(25*1e-3-v)/(exp(2.5-0.1*v*1e3)-1)*1e3;
        }

        double alpha_n() {
            return 0.01*(10*1e-3-v)/(exp(1-0.1*v*1e3)-1)*1e3;
        }

        double beta_h() {
            return 1.0/(1+exp(3.0-0.1*v*1e3));
        }

        double beta_m() {
            return 4*exp(-0.0556*v*1e3);
        }

        double beta_n() {
            return 0.125*exp(-0.0125*v*1e3);
        }

        double membrane_I(double inp_I){
            return inp_I + (gNa*pow(m, 3)*h*(ENa-v) + gl*(El-v) + gK*pow(n, 4)*(EK-v));
        }
    public:
        double h = 0.6;
        double n = 0.32;
        double m = 0.05;
        double v = 0.0;
        double dh_t = 0;
        double dn_t = 0;
        double dm_t = 0;
        double dv_t = 0;
        void derivations(double inp_I){
            dh_t = alpha_h()*(1-h)-beta_h()*h;
            dm_t = alpha_m()*(1-m)-beta_m()*m;
            dn_t = alpha_n()*(1-n)-beta_n()*n;
            dv_t = membrane_I(inp_I)/C*1e-3;
            //cout << dh_t << " " << dm_t << " " << dn_t << " " << dv_t << "\n";
        }
};

class Current{
    public:
        vector<double> t;
        vector<double> I;
        double val(double cur_t){
            int i = 0;

            while((cur_t > t[i]) && (i < t.size()-1))
                i++;

            assert(i < t.size());
            return I[i];
        }
        void step_current(double t1, double t2, double t_end, double Im){
            t.clear();
            I.clear();
            for(int i = 0; i < t_end/0.001; i++){
                t.push_back(i*0.001);
                if((0.001*i > t1) &&(0.001*i < t2))
                    I.push_back(Im);
                else
                    I.push_back(0);
            }
        }
        void ramp_current(double t1, double t2, double t_end, double Im){
            t.clear();
            I.clear();
            for(int i = 0; i < t_end/0.001; i++){
                t.push_back(i*0.001);
                if((0.001*i > t1) &&(0.001*i < t2))
                    I.push_back(Im/(t2-t1) * (0.001*i - t1));
                else
                    I.push_back(0);
            }
        }

};
class Solver{
    private:
        double cur_t; //start of simulation
        double end_time; // end of simulation
        double time_step;
        vector<double> h;
        vector<double> m;
        vector<double> n;
        vector<double> v;
        vector<double> t;
        Current current;

    public:
        Solver(double end_time, double time_step, Current current1):end_time(end_time), time_step(time_step), current(current1){}
        void solve(HH_model neuron){
            cur_t = 0;
            h.clear();
            m.clear();
            n.clear();
            v.clear();
            t.clear();
            while(cur_t <= end_time){
                neuron.derivations(current.val(cur_t));

                neuron.h += neuron.dh_t * time_step;
                neuron.m += neuron.dm_t * time_step;
                neuron.n += neuron.dn_t * time_step;
                neuron.v += neuron.dv_t * time_step;
                h.push_back(neuron.h);
                m.push_back(neuron.m);
                n.push_back(neuron.n);
                v.push_back(neuron.v);
                t.push_back(cur_t);
                cur_t += time_step;
            }
        }
        void save_data(){
            ofstream fout;
            fout.open("data.txt");
            for(int i = 0; i < t.size(); i++){
                fout << t[i] << " " << h[i] << " " << m[i] << " " << n[i] << " " << v[i] << '\n';
            }
            fout << "\n";
            for(int i = 0; i < current.t.size(); i++){
                fout << current.t[i] << " " << current.I[i] << '\n';
            }
            fout.close();
        }
};


int main() {
    HH_model neuron;
    Current current;
    current.step_current(10, 25, 70 ,12e-6);
    Solver solver(70, 0.01, current);
    solver.solve(neuron);
    solver.save_data();

    return 0;
}