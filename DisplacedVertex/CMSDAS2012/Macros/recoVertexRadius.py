import math
import ROOT
from DataFormats.FWLite import Events, Handle

# Reminder: the generation is H0 -> A0 A0, with the A0 forced to decay to leptons. The H0 is 35 and the A0 is 36.
events = Events ('Pythia_H0_pyupda_cfi_py_GEN_SIM_DIGI_L1_RAW2DIGI_RECO.root')

# create handle outside of loop
handleTrack  = Handle ('std::vector<reco::Track>')
handleStandAlone  = Handle ('std::vector<reco::Track>')
handleMuon  = Handle ('std::vector<reco::Muon>')

# a label is just a tuple of strings that is initialized just like an edm::InputTag
labelTrack = ("generalTracks")
labelStandAlone = ("standAloneMuons")
labelMuon = ("muons")

# Disable drawing to the screen
# ROOT.gROOT.SetBatch()        # don't pop up canvases

# Create histograms, etc.
ROOT.gROOT.SetStyle('Plain') # white background
# Note that the genParticle distances are in mm
vertexRadiusTrackHist = ROOT.TH1F ("vertexRadiusTrackHist", "Vertex radius", 1000, 0, 200)
vertexRadiusStandAloneHist = ROOT.TH1F ("vertexRadiusStandAloneHist", "Vertex radius", 1000, 0, 200)
vertexRadiusMuonHist = ROOT.TH1F ("vertexRadiusMuonHist", "Vertex radius", 1000, 0, 200)

# loop over events
for event in events:
    # use getByLabel, just like in cmsRun
    event.getByLabel(labelTrack, handleTrack)
    event.getByLabel(labelStandAlone, handleStandAlone)
    event.getByLabel(labelMuon, handleMuon)
    
    # get the product
    tracks = handleTrack.product()
    standAloneMuons = handleStandAlone.product()
    muons = handleMuon.product()

    for track in tracks:
        vertexRadiusTrackHist.Fill( math.sqrt(track.vx()**2 + track.vy()**2) )
    for standAloneMuon in standAloneMuons:
        vertexRadiusStandAloneHist.Fill( math.sqrt(standAloneMuon.vx()**2 + standAloneMuon.vy()**2) )
    for muon in muons:
        vertexRadiusMuonHist.Fill( math.sqrt(muon.vx()**2 + muon.vy()**2) )

# make a canvas, draw, and save it
c1 = ROOT.TCanvas()
vertexRadiusTrackHist.Draw()
c1.Print ("vertexRadiusTrack.png")

c2 = ROOT.TCanvas()
vertexRadiusStandAloneHist.Draw()
c2.Print ("vertexRadiusStandAlone.png")

c3 = ROOT.TCanvas()
vertexRadiusMuonHist.Draw()
c3.Print ("vertexRadiusMuon.png")
