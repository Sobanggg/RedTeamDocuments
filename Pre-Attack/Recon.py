import urllib2
import ssl
import threading

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
ActiveThreads = []
Target = []
def req():
    while True:
        if(len(Target) == 0):
            break
        url = Target.pop(0)
 	purl = ""
        for tp in ['http','https']:
		for pr in ['','4443','8443','10443','8080','7443']:
			try:
				if(pr != ''):
					purl = tp+'://'+url+':' +pr+'/'
					r = urllib2.urlopen(tp+'://'+url+':' +pr+'/',timeout = 5,context=ctx)
				else:
					purl = tp+'://'+url+'/'
					r = urllib2.urlopen(tp+'://'+url+'/',timeout = 5,context=ctx)
				
                		html = r.read()
				MAP = {'Telerik':'','ViewState':'','SharePoint':'','Exchange':'','PulseSecure':'','FortiOS':'','DialogHandler':''}
				
				if(html.find('Telerik') != -1):
					MAP['Telerik'] = True
				elif(html.find('VIEWSTATE') != -1):
					MAP['ViewState'] = True
				elif(html.find('_layouts') != -1):
					MAP['SharePoint'] = True
				elif(html.find('/owa/') != -1):
					MAP['Exchange'] = True
				elif(html.find('/remote/') != -1):
					MAP['FortiOS'] = True
				elif(html.find('/dana-na/') != -1):
					MAP['PulseSecure'] = True
				try:
					if(pr != ''):
				        	r_t1 = urllib2.urlopen(tp+'://'+url+':'+pr+'/Telerik.Web.UI.DialogHandler.aspx',timeout = 5,context=ctx)
					else:
						r_t1 = urllib2.urlopen(tp+'://'+url+'/Telerik.Web.UI.DialogHandler.aspx',timeout = 5,context=ctx)                    		
					if(r_t1.read().find('Loading the dialog') != -1):
		                		MAP['DialogHandler'] = True
				except:
					pass
				fp_o = open('OutPut.csv','a')
				fp_o.write(purl+',')
				print url,MAP
				
				fp_o.write(str(MAP['Telerik'])+','+str(MAP['ViewState'])+','+str(MAP['SharePoint'])+','+str(MAP['Exchange'])+','+str(MAP['FortiOS'])+','+str(MAP['PulseSecure'])+','+str(MAP['DialogHandler']))
				fp_o.write('\r\n')
				fp_o.close()
			except Exception as e:
				fp = open('Erro-Log.txt','a')
				fp.write(url+':::'+str(e)+'\n')
				fp.close()				



fp = open('target.txt','r')

while True:
    line = fp.readline()
    line = line.replace('\n','').replace('\r','')
    if(line == ''):
        break
    Target.append(line)
print "[+] Active Target: %d"%(len(Target))
fp_o = open('OutPut.csv','w')
fp_o.write('IP,Telerik,ViewState,SharePoint,Exchange,FortiOS,PulseSecure,DialogHandler\r\n')
fp_o.close()

for i in range(15):
    t = threading.Thread(target=req,args=())
    t.start()
    ActiveThreads.append(t)

for t in ActiveThreads:
    t.join()
